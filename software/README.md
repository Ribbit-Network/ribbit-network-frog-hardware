## Current software stack

- The raspberry pi runs a docker container inside of [Balena OS + Balena Cloud](https://www.balena.io/cloud/). This makes it easy to deploy software remotely to the PI and will be useful once there is a large fleet of sensors for managing software updates.
- Inside of the Docker container is a simple Python script that periodically reads data from the sensor and publishes the data to Influx DB.
![image](https://user-images.githubusercontent.com/2559382/120937240-4bf0eb80-c6c1-11eb-8fc8-f3e94653d5b4.png)

