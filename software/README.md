## High Level Overview

- The raspberry pi runs a docker container inside of [Balena OS + Balena Cloud](https://www.balena.io/cloud/). This makes it easy to deploy software remotely to the PI and will be useful once there is a large fleet of sensors for managing software updates.
- Inside of the Docker container is a simple Python script that periodically reads data from the sensor and publishes the data to Influx DB.

## Main Software Folders / Docker Images
- [co2](/co2) - This folder contains the main python script responsible for connecting to the CO2 sensor and publishing the data to the InfluxDB database.
- [gpsd](/gpsd) - This folder contains the dockerfile that configures [gpsd](https://gpsd.gitlab.io/gpsd/) to connect to a gps device for recieving the device location.

## Dashboard Information
See the [dashboard repo](https://github.com/Ribbit-Network/ribbit-network-dashboard) for more information on the public facing data dashboard.

## Main Readme

[Head back to the main readme for more info!](https://github.com/Ribbit-Network/ribbit-network-sensor)
