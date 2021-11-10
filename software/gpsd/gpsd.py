#!/bin/python3

import json
import os
import subprocess
import time
import pathlib

from balena import Balena

"""
Not a definitive list, but models should be added here as they're tested since UART configuration
(specifically the required DT overlay) is not uniform across Raspberry Pi models.

For more information, see:
https://www.raspberrypi.com/documentation/computers/configuration.html#configuring-uarts
"""
SUPPORTED_MODELS = [
    "Raspberry Pi 3"
]

UART0_DEV = "/dev/ttyAMA0"
ACM_CDC_DEV = "/dev/ttyACM0"

def detect_supported_hardware():
    """Determine if UART for this Raspbery Pi is supported"""
    with open("/proc/device-tree/model") as file:
        model_string = file.readline().rstrip()

    for model in SUPPORTED_MODELS:
        if model in model_string:
            print(f"Detected supported Pi model: {model}")
            return model

    print(f"UART not supported on Pi model. model_string: {model_string}")
    return None

def disable_dev_console():
    """
    `disable-bt` designates UART0 as primary UART. Meaning that serial0 will now be a symbolic
    link to ttyAMA0.

    Use dbus to disable serial-getty@serial0.service which contends UART device.

    This only applies to dev mode, but there's currently not a good way to detect if dev mode is
    enabled.
    """
    print("Sending dbus commands to disable the development mode console (if enabled)")
    process = subprocess.Popen(
        [
            'dbus-send ' \
            '--system ' \
            '--print-reply ' \
            '--dest=org.freedesktop.systemd1 ' \
            '/org/freedesktop/systemd1 ' \
            'org.freedesktop.systemd1.Manager.MaskUnitFiles ' \
            'array:string:"serial-getty@serial0.service" ' \
            'boolean:true ' \
            'boolean:true'
        ],
        shell=True,
        env={'DBUS_SYSTEM_BUS_ADDRESS': os.getenv('DBUS_SYSTEM_BUS_ADDRESS')}
    )
    process.wait()

    process = subprocess.Popen(
        [
            'dbus-send ' \
            '--system ' \
            '--print-reply ' \
            '--dest=org.freedesktop.systemd1 ' \
            '/org/freedesktop/systemd1 ' \
            'org.freedesktop.systemd1.Manager.StopUnit ' \
            'string:"serial-getty@serial0.service" ' \
            'string:replace'
        ],
        shell=True,
        env={'DBUS_SYSTEM_BUS_ADDRESS': os.getenv('DBUS_SYSTEM_BUS_ADDRESS')}
    )
    process.wait()

    baud = os.getenv("GPS_CUSTOM_BAUD", "9600")
    print(f"Setting a baud rate of {baud} on {UART0_DEV}...")
    stty = subprocess.Popen(["stty", "-F", UART0_DEV, baud])
    stty.wait()

    # Give a moment for changes to take effect
    time.sleep(1)

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

    device_config = balena.models.config_variable.device_config_variable
    all_variables = device_config.get_all(device_uuid)

    uart_overlay = "disable-bt"

    current_dt_overlays = None
    custom_baud = None
    for variable in all_variables:
        if variable["name"] in ["BALENA_HOST_CONFIG_dtoverlay", "RESIN_HOST_CONFIG_dtoverlay"]:
            dt_overlay_var_id = str(variable["id"])
            current_dt_overlays = set([val.strip('"') for val in variable["value"].split(",")])

    if current_dt_overlays:
        if control.lower() == "enable":
            if "disable-bt" in current_dt_overlays:
                print("Overlay already set, nothing to be done")
                return

            current_dt_overlays |= {uart_overlay}

        else:
            if "disable-bt" not in current_dt_overlays:
                print("Overlay not set, nothing to be done")
                return

            current_dt_overlays -= {uart_overlay}

        dt_overlay_string = ','.join([f'"{dt_overlay}"' for dt_overlay in current_dt_overlays if dt_overlay])
        res = device_config.update(dt_overlay_var_id, dt_overlay_string)
        if res != b'OK':
            print("Error updating dtoverlay!")

    else:
        res = device_config.create(device_uuid, "BALENA_HOST_CONFIG_dtoverlay", uart_overlay)
        if res != b'OK':
            print("Error creating dtoverlay")

    print(f"UART0 {control}d.")

def detect_serial_device():
    if pathlib.Path(ACM_CDC_DEV).is_char_device():
        print("USB CDC device detected.")
        control_uart("disable")
        return ACM_CDC_DEV

    print("USB CDC device not found!")
    print("Detecting if hardware UART is supported on this device.")
    supported_hardware = detect_supported_hardware()
    if supported_hardware:
        print(f"Hardware UART supported. Falling back to UART0 at {UART0_DEV}.")
        control_uart("enable")
        return UART0_DEV
    else:
        print("Supported hardware not found.")
        return None

if __name__ == '__main__':
    try:
        gps_serial_dev = detect_serial_device()
    except Exception as e:
        print(f"An error occured during detecting the correct serial interface: {e}")
        print(f"Defaulting to {ACM_CDC_DEV}")
        gps_serial_dev = ACM_CDC_DEV

    if gps_serial_dev is None:
        print("Exiting in 10 seconds...")
        time.sleep(10)
        quit()

    if gps_serial_dev != ACM_CDC_DEV:
        disable_dev_console()

    print(f"Starting gpsd attached to {gps_serial_dev}...")
    gpsd = subprocess.Popen([f'gpsd -Nn -G {gps_serial_dev}'], shell=True)
    return_code = gpsd.wait()
    if return_code:
        print(f"gpsd returned non-zero exit code: {return_code} Waiting 10 seconds before shutting down.")
        time.sleep(10)

