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
import adafruit_dps310
import paho.mqtt.client as mqtt
import os
import json
import gpsd

from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS

bucket = "co2"
device_uuid = os.getenv('BALENA_DEVICE_UUID')

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

i2c_bus = board.I2C()
scd = adafruit_scd30.SCD30(i2c_bus)
scd.reset()
dps310 = adafruit_dps310.DPS310(i2c_bus)

# Enable self calibration mode
scd.temperature_offset = 3
scd.self_calibration_enabled = True


gps_valid = False

while True:
    if not gps_valid:
        try:
            gpsd.connect()
        except Exception as e:
            print("Error connecting to GPS: %s" % e)
            # TODO: log GPS failures to database?
        finally:
            gps_valid = True
    else:
        # since the measurement interval is long (2+ seconds) we check for new data before reading
        # the values, to ensure current readings.
        if scd.data_available:
            try:
                # Get gps position
                packet = gpsd.get_current()
            except Exception as e:
                # Unable to get current GPS position, log error and attempt to reconnect GPS
                gps_valid = False
                # TODO: log GPS failures to database?
                print("Error getting current GPS position: %s" % e)
                continue

            # Read GPS Data
            latitude = packet.position()[0]
            longitude = packet.position()[1]
            altitude = packet.altitude()

            # Set SCD Pressure from Barometer
            #
            # See Section 1.4.1 in SCD30 Interface Guide
            # https://www.sensirion.com/fileadmin/user_upload/customers/sensirion/Dokumente/9.5_CO2/Sensirion_CO2_Sensors_SCD30_Interface_Description.pdf
            #
            # The Reference pressure must be greater than 0.
            if dps310.pressure > 0:
                scd.ambient_pressure = dps310.pressure

            # Publish to Influx DB Cloud
            point = Point("ghg_point").tag("host", device_uuid) \
                .field("co2", scd.CO2).time(datetime.utcnow(), WritePrecision.NS) \
                .field("temperature", scd.temperature) \
                .field("humidity", scd.relative_humidity) \
                .field("lat", latitude) \
                .field("lon", longitude) \
                .field("alt", altitude) \
                .field("baro_pressure", dps310.pressure)
            
            write_api.write(bucket, org, point)

            # Publish to Local MQTT Broker
            data = {}
            data['CO2'] = scd.CO2
            data['Temperature'] = scd.temperature
            data['Relative_Humidity'] = scd.relative_humidity
            data['Latitude'] = latitude
            data['Longitude'] = longitude
            data['Altitude'] = altitude
            data['scd_temp_offset'] = scd.temperature_offset
            data['baro_temp'] = dps310.temperature
            data['baro_pressure_hpa'] = dps310.pressure

            print(json.dumps(data))

    time.sleep(0.5)