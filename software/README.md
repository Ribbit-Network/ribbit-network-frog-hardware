## High Level Overview

- The raspberry pi runs a docker container inside of [Balena OS + Balena Cloud](https://www.balena.io/cloud/). This makes it easy to deploy software remotely to the PI and will be useful once there is a large fleet of sensors for managing software updates.
- Inside of the Docker container is a simple Python script that periodically reads data from the sensor and publishes the data to Influx DB.

![image](https://user-images.githubusercontent.com/2559382/128450769-5bc59039-b0de-4313-9170-043455f93940.png)


## Main Software Folders / Docker Images
- [co2](/co2) - This folder contains the main python script responsible for connecting to the CO2 sensor and publishing the data to the InfluxDB database.
- [gps](/gps) - This folder contains the dockerfile that configures [gpsd](https://gpsd.gitlab.io/gpsd/) to connect to a gps device for recieving the device location.
- [wifi-connect](/wifi-connect) - This is a git submodule to the wifi connect repo. This allows easy reconfiguration of the sensor's wifi connection.

## Local Development

The `co2` and `gpsd` folders are Python projects with identical setup. To work on them locally (including helping your IDE understand the dependencies):

1. Install the Poetry package manager as described [here](https://python-poetry.org/docs/master/#installing-with-the-official-installer), then in either `gpsd` or `co2`:
1. `poetry install` to create a virtualenv with all production *and* development dependencies
1. `poetry run ./check.sh` to execute all the linters and tests

Some further notes:
* You can run `poetry run <whatever>` to execute commands in the virtualenv, or `poetry shell` to enter a shell with the virtualenv activated.
* If your IDE needs help finding the location of the virtualenv, `poetry env info` will tell you what you need.
* You can add / remove dependencies with `poetry add` and `poetry remove`. Similarly, `poetry update` can be used to bump the version of dependencies.

## Deployment
The software is deployed via the [Balena CLI](https://www.balena.io/docs/reference/balena-cli/) with the following command:

```
balena push ribbit-network
```

Note you must have the right permissions to actually deploy to the whole ribbit network. Replace "ribbit-network" with your own Balena Application name if you are deploying elsewhere.

## Dashboard Information
See the [dashboard repo](https://github.com/Ribbit-Network/ribbit-network-dashboard) for more information on the public facing data dashboard.

## Main Readme

[Head back to the main readme for more info!](https://github.com/Ribbit-Network/ribbit-network-sensor)
