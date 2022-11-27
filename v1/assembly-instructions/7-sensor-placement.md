# Sensor Placement

Now it's time to place the sensor in its home outside your home.

You can use the loop on the top of the sensor to hang the sensor somewhere or use the slot on the backside to hang the sensor on a nail.

The location will need to be near a power outlet to ensure there is power for the sensor. Ideally it should be as high as possible and out of direct sunlight as much as possible to prevent rapid temperature changes. Try to ensure the space above the sensor is largely unobstructed so that the GPS unit can obtain an accurate position (don't place the sensor in a shed for instance).

<img src="images/sensor_placement.jpg" width="400">

## Power Cable
Connect the USB C power supply to the USB C connector on the bottom of the Raspberry Pi carrier board and plug the sensor into a normal wall outlet. If your USB C power supply is connected to an extension cord, you may want to protect that connection from the weather in an enclosure. Search online for weatherproof extension cord covers, outdoor electrical boxes, or build your own out of a waterproof container. Make sure that all the cables enter through an opening in bottom of the enclosure so that water can’t collect inside.

## WiFi Connection

If you didn't enter your home network WiFi information previously, or you purchased the Frog pre-assembled, you'll need to connect it to your home WiFi network:

* After the Frog Sensor is connected to power, it will create a Hotspot called "WiFi Connect". Connect to that hotspot network from a computer or smartphone to continue the setup.

<img src="https://user-images.githubusercontent.com/2559382/132726575-3b223221-369a-4b77-a6eb-4cfcf5464836.PNG" width="400">

* Upon connecting to the hotspot, or by opening a web browser, you may be redirected to the page pictured below. Otherwise, open a web browser and navigate to https://192.168.42.1 
* On this page, select your home WiFi network (SSID) and enter the Passphrase. This will connect the frog sensor to your home WiFi network.
  * If you do not see your home WiFi network (SSID) name listed in the drop-down menu, try refreshing the page.
  * If refreshing the webpage does not work, unplug the power cable from the frog sensor, wait 30 seconds, then plug it back in to force it to restart. Then try the above steps again.
  * If neither of these work, your sensor may be too far from your home WiFi network. Try placing the frog sensor in another location. (See additional help resources [here](#need-help))

<img src="https://user-images.githubusercontent.com/2559382/132726695-275c0557-6907-45ca-b88f-dedde213ec59.PNG" width="400">

* Once you have entered your WiFi information, you should see the following message appear on the page: *"Note! Applying changes... Your device will soon be online. If connection is unsuccessful, the Access Point will be back up in a few minutes, and reloading this page will allow you to try again."*
  * If you receive the error message: *"Danger! Fail to connect to the network. NetworkError when attempting to fetch resource.* Unplug the power cable from the frog sensor, wait 30 seconds, then plug it back in to force it to restart. Then try the above steps again. (See additional help resources [here](#need-help))
* The sensor will now disconnect from your device, stop broadcasting the "WiFi Connect" hotspot, and connect to your home WiFi network. 
* You should see the Frog sensor appear on the [web page here shortly](https://ribbit-network.herokuapp.com/). 

If there are any connection problems, the sensor will restart and broadcast the Wifi Connect hotspot again.

## Next Step
[View the data!](8-done.md)

## Previous Step
[Go Back](6-software.md)

## Table of Contents
[Return to the Beginning](0-start-here.md)

## Need Help?
No problem! The Ribbit Network team is here for you! We have lots of ways to connect. Jump in and ask your question or provide a suggestion!
* [Start a discussion here](https://github.com/Ribbit-Network/ribbit-network-frog-sensor/discussions/new)
* [Join the Developer Discord](https://discord.gg/vq8PkDb2TC)
* [Email](mailto:keenan.johnson@gmail.com)
