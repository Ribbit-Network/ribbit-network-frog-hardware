#!/bin/python3

import json
import logging
import os
import pathlib
import subprocess
import sys
import time

from balena import Balena  # type: ignore

SUPPORTED_MODELS = ["Raspberry Pi 3"]
"""
Not a definitive list, but models should be added here as they're tested since UART configuration
(specifically the required DT overlay) is not uniform across Raspberry Pi models.

For more information, see:
https://www.raspberrypi.com/documentation/computers/configuration.html#configuring-uarts
"""

UART0_DEV = "/dev/ttyAMA0"
ACM_CDC_DEV = "/dev/ttyACM0"

UART_OVERLAY = "disable-bt"

LOGGER = logging.getLogger(__name__)


def detect_supported_hardware():
    """Determine if UART for this Raspbery Pi is supported"""
    with open("/proc/device-tree/model", encoding="ascii") as file:
        model_string = file.readline().rstrip()

    for model in SUPPORTED_MODELS:
        if model in model_string:
            LOGGER.info("Detected supported Pi model: %s", model)
            return model

    LOGGER.info("UART not supported on Pi model. model_string: %s", model_string)
    return None


def disable_dev_console():
    """
    `disable-bt` designates UART0 as primary UART. Meaning that serial0 will now be a symbolic
    link to ttyAMA0.

    Use dbus to disable serial-getty@serial0.service which contends UART device.

    This only applies to dev mode, but there's currently not a good way to detect if dev mode is
    enabled.
    """
    LOGGER.info(
        "Sending dbus commands to disable the development mode console (if enabled)"
    )
    subprocess.check_call(
        [
            "dbus-send "
            "--system "
            "--print-reply "
            "--dest=org.freedesktop.systemd1 "
            "/org/freedesktop/systemd1 "
            "org.freedesktop.systemd1.Manager.MaskUnitFiles "
            'array:string:"serial-getty@serial0.service" '
            "boolean:true "
            "boolean:true"
        ],
        shell=True,
        env={"DBUS_SYSTEM_BUS_ADDRESS": os.getenv("DBUS_SYSTEM_BUS_ADDRESS")},
    )

    subprocess.check_call(
        [
            "dbus-send "
            "--system "
            "--print-reply "
            "--dest=org.freedesktop.systemd1 "
            "/org/freedesktop/systemd1 "
            "org.freedesktop.systemd1.Manager.StopUnit "
            'string:"serial-getty@serial0.service" '
            "string:replace"
        ],
        shell=True,
        env={"DBUS_SYSTEM_BUS_ADDRESS": os.getenv("DBUS_SYSTEM_BUS_ADDRESS")},
    )

    baud = os.getenv("GPS_CUSTOM_BAUD", "9600")
    LOGGER.info("Setting a baud rate of %s on %s...", baud, UART0_DEV)
    subprocess.check_call(["stty", "-F", UART0_DEV, baud])

    # Give a moment for changes to take effect
    time.sleep(1)


def find_dtoverlay_config(variable_list):
    """
    Find value and id of BALENA_HOST_CONFIG_dtoverlay or RESIN_HOST_CONFIG_dtoverlay.
    Return None if it doesn't exist.
    """
    current_dt_overlay = None
    dt_overlay_var_id = None

    for variable in variable_list:
        if variable["name"] in [
            "BALENA_HOST_CONFIG_dtoverlay",
            "RESIN_HOST_CONFIG_dtoverlay",
        ]:
            dt_overlay_var_id = str(variable["id"])
            # If contains quotes, parse as a JSON lists
            if variable["value"].startswith('"') and variable["value"].endswith('"'):
                current_dt_overlay = set(json.loads("[" + variable["value"] + "]"))
            else:
                current_dt_overlay = set([variable["value"]])

    return current_dt_overlay, dt_overlay_var_id


def control_uart(control):
    """
    Use the Balena SDK to programatically set the disable-bt overlay.

    TODO: Accept the desired dt_overlay as a parameter to this function, as it differs for
          different versions of RPi hardware.

    Does nothing if already enabled (or disabled).
    """

    if control not in ["enable", "disable"]:
        raise ValueError(f"Unrecognized control parameter: {control}")

    balena = Balena()
    # Accessing API key from container requires io.balena.features.balena-api: '1'
    balena.auth.login_with_token(os.getenv("BALENA_API_KEY"))
    device_uuid = os.getenv("BALENA_DEVICE_UUID")
    app_id = os.getenv("BALENA_APP_ID")

    device_config = balena.models.config_variable.device_config_variable
    all_device_vars = device_config.get_all(device_uuid)
    device_dt_overlays, dt_overlay_var_id = find_dtoverlay_config(all_device_vars)
    LOGGER.info("Device dt overlay: %s", device_dt_overlays)

    app_config = balena.models.config_variable.application_config_variable
    all_app_vars = app_config.get_all(app_id)
    app_dt_overlays = find_dtoverlay_config(all_app_vars)[0]
    LOGGER.info("Fleet override dt overlay: %s", app_dt_overlays)

    device_overlay_exists = False
    if device_dt_overlays is not None:
        device_overlay_exists = True
        current_dt_overlays = device_dt_overlays
    elif app_dt_overlays is not None:
        current_dt_overlays = app_dt_overlays
    else:
        current_dt_overlays = None

    if control.lower() == "enable":
        new_dt_overlay = current_dt_overlays | {UART_OVERLAY}
    else:
        new_dt_overlay = current_dt_overlays - {UART_OVERLAY}

    if new_dt_overlay == current_dt_overlays:
        LOGGER.info("DT overlay config doesn't need to be updated")
        return

    dt_overlay_string = ",".join(
        [f'"{dt_overlay}"' for dt_overlay in new_dt_overlay if dt_overlay]
    )

    if device_overlay_exists:
        device_config.update(dt_overlay_var_id, dt_overlay_string)
        LOGGER.info("Updated device overlay: %s", dt_overlay_string)

    else:
        device_config.create(
            device_uuid, "BALENA_HOST_CONFIG_dtoverlay", dt_overlay_string
        )
        LOGGER.info("Created BALENA_HOST_CONFIG_dtoverlay=%s", dt_overlay_string)

    LOGGER.info("UART0 %sd.", control)


def detect_serial_device():
    if pathlib.Path(ACM_CDC_DEV).is_char_device():
        LOGGER.info("USB CDC device detected.")
        control_uart("disable")
        return ACM_CDC_DEV

    LOGGER.info("USB CDC device not found!")
    LOGGER.info("Detecting if hardware UART is supported on this device.")
    supported_hardware = detect_supported_hardware()
    if supported_hardware:
        LOGGER.info("Hardware UART supported. Falling back to UART0 at %s.", UART0_DEV)
        control_uart("enable")
        return UART0_DEV
    LOGGER.warning("Supported hardware not found.")
    return None


def main() -> None:
    logging.basicConfig(
        format="%(asctime)s %(levelname)s %(message)s", level=logging.INFO
    )

    try:
        gps_serial_dev = detect_serial_device()
    except Exception:  # pylint: disable=broad-except
        LOGGER.exception(
            "An error occurred during detecting the correct serial interface"
        )
        LOGGER.warning("Defaulting to %s", ACM_CDC_DEV)
        gps_serial_dev = ACM_CDC_DEV

    if gps_serial_dev is None:
        LOGGER.error("Exiting in 10 seconds...")
        time.sleep(10)
        sys.exit()

    if gps_serial_dev != ACM_CDC_DEV:
        disable_dev_console()

    LOGGER.info("Starting gpsd attached to %s...", gps_serial_dev)
    return_code = subprocess.run(
        [f"gpsd -Nn -G {gps_serial_dev}"], shell=True, check=False
    ).returncode
    if return_code:
        LOGGER.error(
            "gpsd returned non-zero exit code: %d Waiting 10 seconds before shutting down.",
            return_code,
        )
        time.sleep(10)
        sys.exit(return_code)


if __name__ == "__main__":
    main()
