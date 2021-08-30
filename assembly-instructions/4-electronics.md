# Electronics

Now to assemble the electronics. The first step is to attach all of the electronics to the 3D printed bracket.

## Electronics Standoffs

To do this, attach the brass standoffs to the bracket which we will attach the electronics to. This is easiest to do with a 4.5 hex socket. Be careful not to over tighten the standoff in the bracket!

https://user-images.githubusercontent.com/2559382/131203360-0a97f4ac-2007-4650-8da2-7b24859d4129.MOV

There are 11 standoffs to install. See the images below for the locations.

There are 7 standoffs on the side of the bracket with the antennae loop:
![Sensor Side](images/standoffs_sensors.png)

There are 4 standoffs on the opposite side where the Raspberry Pi will attach:
![RPI Side](images/standoffs_rpi.png)

## Attach Electronics
Now it's time to attach the electronics to the standoffs. Attach the Raspberry Pi Carrier Board on one side, the SCD30, Barometer, and GPS on the other side as shown in the photos below:

![Electronics RPI Side](images/electronics_rpi_side.jpg)
![Electronics Sensor Side](images/electronics_sensor_side.jpg)

## Install Rasperry Pi Compute Module

The raspberry pi compute module 4 should just click into the connectors on the carrier board you installed in the previous step.

## Soldering Raspberry Pi QWIIC Adapter
The qwiic adapter to connect the CO2 sensor to the raspberry comes without headers soldered on.

![qwiic no headers](images/qwiic_shim_no_headers.jpeg)

You'll need to solder on headers like so:
![qwiic headers](images/qwiic_shim_headers.png)

## GPS Antennae Clip
Now it's time to install the GPS Antennae clip into the main bracket: 

![GPS Clip](images/gps_clip.JPG) Once the clip is installed, you insert the antennae into the clip and then connect the connector to the connector on the GPS board. See the image below for the orientation. Remember to orient the antennae so the cable comes out on the side to prevent it from catching on the enclosure later.

![gps antennae close up](images/gps_antennae_close_up.png)

## Cables
There are three cables that need to be installed as shown in the pictures above:
* The USB cable - This goes from the raspberry pi to the GPS module as shown in the pictures above. Notice the loop which keeps the cable managed nicely.
* STEMMA QT / Qwiic JST SH 4-pin Cable - 50mm Long - The Cable from the SCD30 CO2 sensor to the barometer
* STEMMA QT / Qwiic JST SH 4-pin Cable - 100mm Long - The Cable from the SCD30 CO2 sensor to the Raspberry Pi
* Wifi antennae - This gets screwed into the loop on the sensor side of the bracket and goes to the Raspberry Pi on the other side

Everything should look like this when you are done:

![Electronics RPI Side](images/electronics_rpi_side.jpg)
![Electronics Sensor Side](images/electronics_sensor_side.jpg)

## Install Electronics Into Enclosure

The final step is to slide the completed electronics bracket into the enclosure. The GPS Antennae side of the bracket should go into the enclosure first, so that the USB C power connect on the bottom of the Rapsberry Pi Carrier board is accessible at the bottom. See this image with the body of the enclosure cut away:

![orientation](images/bracket_orientation.png)

## Assembly complete!

The electronics assembly is now complete! The only piecdes you should have left are the SD card and power cord which we will install in the next steps.

## Next Step
[Software Installation](5-software.md)

## Previous Step
[Go Back](3-enclosure.md)

## Table of Contents
[Return to the Beginning](0-start-here.md)

## Need Help?
No problem! The Ribbit Netowrk team is here for you! We have lots of ways to connect. Jump in and ask your question or provide a suggestion!
* [Start a dicussion here](https://github.com/Ribbit-Network/ribbit-network-frog-sensor/discussions/new)
* [Join the Developer Discord](https://discord.gg/vq8PkDb2TC)
