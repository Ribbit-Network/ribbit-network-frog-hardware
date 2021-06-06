# Greenhouse Gas Sensor Cloud

The plan is to deploy a large network of open source, low cost, CO2 Gas Detection Sensors. These sensor units will be sold as a kit and will optionally upload their data to a centralized, but anonymous database. This will enable many to contribute to the work on climate by contributing to the world's largest greenhouse gas database.

## Background Information
The cost of "true" (measured P.P.M.) CO2 sensors is now low. This enables a large grassroots greenhouse gas monitoring network to inspire climate awareness and action while generating a large scientific data set.

There is a precedent for GHG networks in academia: http://beacon.berkeley.edu/overview/. The BEACO2N Project uses extremely expensive sensors, making cost a significant barrier to deployment at scale.

## Hardware

The current prototype status is shown below. It consists of a Raspberry Pi 4 and a SCD30 sensor.

![image](https://user-images.githubusercontent.com/2559382/120937105-670f2b80-c6c0-11eb-8dce-8297ccd2693e.png)

## Software
My current software stack is comprised of the following:

- The raspberry pi runs a docker container inside of [Balena OS + Balena Cloud](https://www.balena.io/cloud/). This makes it easy to deploy software remotely to the PI and will be useful once there is a large fleet of sensors for managing software updates.
- Inside of the Docker container is a simple Python script that periodically reads data from the sensor and publishes the data to Influx DB.
![image](https://user-images.githubusercontent.com/2559382/120937240-4bf0eb80-c6c1-11eb-8fc8-f3e94653d5b4.png)


## Sensors
The current prototype uses the [Sensirion SCD30 CO2 Sensor](https://www.digikey.com/en/products/detail/sensirion-ag/SCD30/8445334?utm_adgroup=Gas%20Sensors&utm_source=google&utm_medium=cpc&utm_campaign=Shopping_Product_Sensors%2C%20Transducers&utm_term=&utm_content=Gas%20Sensors&gclid=Cj0KCQjw2NyFBhDoARIsAMtHtZ4YeCuIrZjKrrVm8ohZNMRxEB0_QjV2AzrOf9Rp4tfHgq8otFP3-v0aAjflEALw_wcB).  It's possible that the newer sensor, the [SCD4x](https://www.sensirion.com/en/environmental-sensors/carbon-dioxide-sensors/carbon-dioxide-sensor-scd4x/) is a better sensor, but this needs to be evaluated.
![image](https://user-images.githubusercontent.com/2559382/120937190-eb61ae80-c6c0-11eb-8ef7-9f8a7e22a168.png)

## To Do List
See the [Issues](https://github.com/keenanjohnson/ghg-gas-cloud/issues) section of this project for the work that I've currently scoped out to be done. Reach out to me if you are interested in helping out!

## Project Lead
Hi I'm [Keenan](https://www.keenanjohnson.com/) and I work on this project! Reach out if you are interested or want to support me in some way :)
