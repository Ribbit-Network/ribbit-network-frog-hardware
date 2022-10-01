A folder to hold software related to an ESP32 version of the Frog Sensor.

### Build Instructions

First, setup the environment. This step assumes you've installed esp-idf to `~/esp/esp-idf`

```sh
source ~/esp/esp-idf/export.sh
```

Next, `cd` to one of the examples, where you can build/flash/monitor:

```
cd software_esp32
idf.py build
idf.py flash
idf.py monitor
```

# Golioth Basics Example

This example will connect to Golioth and demonstrate Logging, Over-the-Air (OTA)
firmware updates, and sending data to both LightDB state and LightDB Stream.
Please see the comments in app_main.c for a thorough explanation of the expected
behavior.

## Adding credentials

After building and flashing this app, you will need to add WiFi and Golioth
credentials using the device shell or BLE.

### Shell Credentials

```console
settings set wifi/ssid "YourWiFiAccessPointName"
settings set wifi/psk "YourWiFiPassword"
settings set golioth/psk-id "YourGoliothDevicePSK-ID"
settings set golioth/psk "YourGoliothDevicePSK"
```

Type `reset` to restart the app with the new credentials.

### BLE Credentials

Use the web app to provision:

https://github.com/goliothlabs/ble_prov_web
