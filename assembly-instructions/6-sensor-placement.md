# Sensor Placement

Now it's time to place the sensor in it's home outside your home.

You can use the loop on the top of the sensor to hang the sensor somewhere or use the slot on the backside to hang the sensor on a nail.

The location will need to be near a power outlet to ensure there is power for the sensor. Ideally it should be as high as possible and as far away from structures like walls or corners to ensure maximum airflow. It also shouldn't be placed too close to a tree, if possible.

Try to ensure the space above the sensor is largely unobstructed so that the GPS unit can obtain an accurate position (don't place the sensor in a shed for instance).

![placement](images/sensor_placement.jpg)

## Power Cable
Connect the USB C power supply to the USB C connector on the bottom of the Raspberry Pi carrier board and plug the sensor into a normal wall outlet.

## WiFi Connection

If you didn't enter wifi information previously, or you purchased the Frog, you'll need to connect it to your WiFi network.

After the Frog Sensor is connected to power, it will create a Hotspot called "WiFi Connect". Connect to that network to enter your home WiFi credentials.

![IMG_9529](https://user-images.githubusercontent.com/2559382/132726575-3b223221-369a-4b77-a6eb-4cfcf5464836.PNG)

Now you'll see this screen below where you can select the WiFi SSID and enter the Passphrase.

![IMG_9530](https://user-images.githubusercontent.com/2559382/132726695-275c0557-6907-45ca-b88f-dedde213ec59.PNG)

Once you have entered the information, the sensor will disconnect from your device, stop broadcasting the hotspot and connect to your home wifi. You should see the Frog sensor appear on the [web page here shortly](https://ribbit-network.herokuapp.com/). If there is any connection problem, the sensor will restart and broadcast the Wifi Connect hotspot again.

## Next Step
[View the data!](7-done.md)

## Previous Step
[Go Back](5-software.md)

## Table of Contents
[Return to the Beginning](0-start-here.md)

## Need Help?
No problem! The Ribbit Netowrk team is here for you! We have lots of ways to connect. Jump in and ask your question or provide a suggestion!
* [Start a dicussion here](https://github.com/Ribbit-Network/ribbit-network-frog-sensor/discussions/new)
* [Join the Developer Discord](https://discord.gg/vq8PkDb2TC)
