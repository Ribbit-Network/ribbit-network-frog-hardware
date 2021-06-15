# The MIT License (MIT)
#
# Copyright (c) 2021 Keenan Johnson
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

import time
import board
from datetime import datetime
import adafruit_scd30
import paho.mqtt.client as mqtt
import os
import requests
import json

scd = adafruit_scd30.SCD30(board.I2C())

from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS

bucket = "co2"

def mqtt_detect():
    
    # Use the supervisor api to get services
    # See https://www.balena.io/docs/reference/supervisor/supervisor-api/
    
    address = os.getenv('BALENA_SUPERVISOR_ADDRESS', '')
    api_key = os.getenv('BALENA_SUPERVISOR_API_KEY', '')
    app_name = os.getenv('BALENA_APP_NAME', '')

    url = "{0}/v2/applications/state?apikey={1}".format(address, api_key)

    try:
        r = requests.get(url).json()
    except Exception as e:
        print("Error looking for MQTT service: {0}".format(str(e)))
        return False
    else:
        services = r[app_name]['services'].keys()

        if "mqtt" in services:
            return True
        else:
            return False

#
# config ini file
#
# [influx2]
# url=http://localhost:8086
# org=my-org
# token=my-token
# timeout=6000
# verify_ssl=False
#
client = InfluxDBClient.from_config_file("influx_config.ini")
org = "keenan.johnson@gmail.com"

write_api = client.write_api(write_options=SYNCHRONOUS)

mqtt_address = os.getenv('MQTT_ADDRESS', 'none')

if mqtt_detect() and mqtt_address == "none":
    mqtt_address = "mqtt"

mqtt_client = mqtt.Client("1")
print(mqtt_address)
mqtt_client.connect("127.0.0.1", 1883, 60)

while True:
    # since the measurement interval is long (2+ seconds) we check for new data before reading
    # the values, to ensure current readings.
    if scd.data_available:
        print("Data Available!")
        print("CO2:", scd.CO2, "PPM")
        print("Temperature:", scd.temperature, "degrees C")
        print("Humidity:", scd.relative_humidity, "%%rH")
        print("")
        print("Waiting for new data...")
        print("")

        # Publish to Influx DB Cloud
        point = Point("ghg_point").tag("host", "host1").field("co2", scd.CO2).time(datetime.utcnow(), WritePrecision.NS).field("temperature", scd.temperature).field("humidity", scd.relative_humidity)
        write_api.write(bucket, org, point)

        # Publish to Local MQTT Broker
        data = {}
        data['CO2'] = scd.CO2
        data['Temperature'] = scd.temperature
        data['Relative_Humidity'] = scd.relative_humidity

        mqtt_client.publish("sensors",json.dumps(data))
        print(json.dumps(data))

    time.sleep(0.5)