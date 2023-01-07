# Software

In order for your Frog to be useful, you'll need to install some software first. You will need a computer for this step.

## Install the Software on your Frog

Connect the esp32 board to your computer via a USB C Cable. 

Hold the "boot" button and press the "reset" button (shown below). This puts the esp32 into a flashing mode.

<img width="604" alt="esp32_buttons" src="https://user-images.githubusercontent.com/2559382/211125493-80c9d8c8-10fd-491d-b279-cbd1278bbd04.jpg">

Vist this page https://ribbit-network.github.io/ribbit-network-frog-software/ to install the latest software.

Hit the "Connect Button" 

<img width="489" alt="image" src="https://user-images.githubusercontent.com/2559382/210677925-f321f429-7811-44c5-a263-5ad1e9addf4d.png">

You will see a pop-up window like the one shown below. There should be an option labeled USB JTAG. Select that and hit connect.

<img width="468" alt="image" src="https://user-images.githubusercontent.com/2559382/211125557-afb00ccf-5dd1-42c9-a3e3-8014af955c51.png">

Your screen should now show the following: 

<img width="604" alt="image" src="https://user-images.githubusercontent.com/2559382/211125585-281289fa-52fc-44ef-9745-a1ac9d7dacfb.png">

Hit the install button and your Frog sensor will get it's software installed.

<img width="437" alt="image" src="https://user-images.githubusercontent.com/2559382/211125632-bc373ddc-92c2-47c8-a2a4-ecb458706f67.png">

Once the installation is complete, you should see the following:

<img width="502" alt="image" src="https://user-images.githubusercontent.com/2559382/211125661-0d43bb53-27af-49c4-b9b1-e626a5ad1d6b.png">

Congrats your software is now installed!

## Configure Wifi Connection

In order for your Frog Sensor to connect to the internet, you will need to tell it what Wifi Credentials to use.

First, unplug and replug in your Frog. You should see lights start to flash on your board.

Hit the connect button again and you should now see an option labeled "Ribbit Frog Sensor V4"

<img width="441" alt="image" src="https://user-images.githubusercontent.com/2559382/211125712-40ee53d2-5da5-40b7-b9af-daa34bd1697d.png">

Hit connect and then select the Connect to Wifi option. The Frog will now scan for wifi networks. Select the correct network and enter your password in the form. This password is stored securely on your Frog and will be remembered, so your Frog can always connect to the internet, receive updates, and transmit data.

<img width="474" alt="image" src="https://user-images.githubusercontent.com/2559382/211125784-1bdd2662-393d-4c03-867a-0dc4e2dc07f7.png">

Once the connection is successful, you see the success message. 

<img width="355" alt="image" src="https://user-images.githubusercontent.com/2559382/211125850-842b02a8-f76b-4fe6-b6f9-354166cce306.png">

## Connect the Frog to our "Fleet" of Frogs

In order for your Frog to join the Ribbit Network "Fleet" we need to tell it a name and password so that it can securely connect. 


### Get Credentials

First, reach out to the Ribbit Network Core team in Discord ([Join the Developer Discord](https://discord.gg/vq8PkDb2TC)) and send Keenan Johnson a message in the #build-a-sensor channel. He will get you a set of credentials you can use in the next step. The team is working hard to make this self service soon :).

### Find the IP address of your Frog
From the install screen, click the "Visit Device" button

<img width="293" alt="image" src="https://user-images.githubusercontent.com/2559382/211126145-65b4f4e3-34df-45b0-9f72-fdb938d5b219.png">

This will take you to a website that is hosted on your esp32 board. Take note of the IP address (example:192.168.86.128) shown in the address bar, as you will need that next.

<img width="360" alt="image" src="https://user-images.githubusercontent.com/2559382/211126179-142984dc-60c1-4aa5-8901-5784cee559ae.png">

### Send the credentials to your Frog

Copy the following command and update it with your IP address you noted above and the username and password given to you by the Ribbit Network team.

```
curl http://192.168.86.92/api/config -X PATCH -H "Content-Type: application/json" -d '{"golioth.user": "<your device ID>", "golioth.password": "<your device PSK>"}'
```

Open a termainl program on your computer, paste the command that you edited above with your correct IP address and credentials into the terminal and hit enter. This will send the command to your Frog configuring the credentials. At this point, the Ribbit Network software team can verify your device connection. Message Keenan in discord.

Note: curl should be installed on most linux, mac, or windows computers.

## Complete

Well done! Now you can disconnect your board from the computer and begin the physical assembly.

## Next Step
[Assembly](4-assembly.md)

## Table of Contents
[Return to the Beginning](0-start-here.md)

## Need Help?
No problem! The Ribbit Network team is here for you! We have lots of ways to connect. Jump in and ask your question or provide a suggestion!
* [Start a discussion here](https://github.com/Ribbit-Network/ribbit-network-frog-software/discussions/new)
* [Join the Developer Discord](https://discord.gg/vq8PkDb2TC)
