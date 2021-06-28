# Greenhouse Gas Sensor Cloud

This project will create the world's largest Greenhouse Gas Emissions dataset that will empower anyone to join in the work on climate and provide informed data for climate action.

The Greenhouse Gas Sensor Cloud is a large network of open-source, low-cost, Greenhouse Gas (CO2 and possibly methane) Detection Sensors. These sensor units will be sold as a kit and will upload their data to the cloud, creating the world's most complete Greenhouse Gas dataset.

Imagine [PurpleAir](https://www.purpleair.com/map?q=purpleair.com#11/47.6356/-122.3605) and [Folding@Home](https://foldingathome.org/) combined as an open-source solution for climate.

The "Cloud" name is a bit of a pun in that we are measuring the atmospheric content (where clouds live) and then pushing that data into an interet database (The Cloud) 😊

<img width="554" alt="Screen Shot 2021-06-08 at 9 13 10 AM" src="https://user-images.githubusercontent.com/2559382/122119420-037eaf80-cdde-11eb-8ed7-de7aa96c0001.png">

## Why do this?
Rising to the climate crisis challenge is a once-in-a-generation opportunity. There are two significant reasons to build and deploy this sensor network at a global scale:
- Greenhouse gas emissions have been identified as one of the largest contributing factors in climate change. However, there is an alarming lack of direct measurements of these greenhouse gasses. [As Prof. Dr.-Ing. Jia Chen states](https://www.eurekalert.org/pub_releases/2021-03/tuom-usn030821.php): "Instead, the emissions numbers we hear in the news are based on calculations." NOAA does indeed track atmospheric Greenhouse Gas levels, but only at one observatory: Mauna Loa Observatory, Hawaii. That one location is useful, but it’s not localized enough to tie the emissions to specific sources. Research has shown (see background info below) that sensor networks can be used to quantify emission sources from specific geographic locations, allowing scientists, corporate leaders, regulators, and policy makers to take informed action. We just need to deploy the sensors!
- Most people feel powerless to do much to change the course of society in regards to climate change. A citizen science solution like this will give agency to the citizens of our planet by enabling them to make a meaningful contribution to the work on climate, bringing much-needed hope into the dialog for the majority of people. The success of citizen science projects can't be ignored (Ex: [Folding@Home](https://test.foldingathome.org/2021/01/05/2020-in-review-and-happy-new-year-2021/?lng=en-US) and [zooniverse](https://www.zooniverse.org/)) and this project has received significant interest from individuals interseted in deploying a monitoring station of their own.

## Why Open Source?
- I strongly believe that for this data to be trusted, the hardware and software that collects the data must be fully transparent and accessible to all.
- The community model allows anyone to contribute if they find the project particularly compelling. This is exciting because it means the project can also become an advocacy and teaching tool for anyone looking to learn about this planet and the state of its climate.

## Current Status
I am currently developing the hardware and software for the first unit that could be could be sold as a kit. The plan is to have the first kits ready for shipment by the end of July.

The first prototype sensor is up and running in Seattle. [Here is some real data from that sensor](https://ghg-cloud.herokuapp.com/) (Note this dashboard is still experimental and may be down occasionally).

<img width="1662" alt="Screen Shot 2021-06-13 at 11 32 37 PM" src="https://user-images.githubusercontent.com/2559382/122121159-25793180-cde0-11eb-84c9-84f402b7a814.png">

## Background Information
The cost of "true" (measured P.P.M.) CO2 sensors are now low. This enables a large grassroots greenhouse gas monitoring network to inspire climate awareness and action while generating a large scientific data set.

There is a precedent for GHG networks in academia: 
- http://beacon.berkeley.edu/overview/. The BEACO2N Project uses extremely expensive sensors, making cost a significant barrier to deployment at scale.
- [MUCCnet](https://www.eurekalert.org/pub_releases/2021-03/tuom-usn030821.php) - Research done in Munich showed that incredible insights can be gained from sensor networks like this one in identifying the source of emissions: "Chen explains the principle in simple terms: "We set up one sensor upwind from the city and the second downwind. So any increase in gases between the first sensor and the second must have been generated from inside the city." To cover as many wind directions as possible, there is a sensor in each cardinal direction. With the input of the sensor data and meteorological parameters, high-performance computers can create a spatially resolved emission map of the city."

## Technical Documentation
[Check out the Wiki for the technical documentation of how this project works!](https://github.com/keenanjohnson/ghg-gas-cloud/wiki)

## Contributing
See the [Issues](https://github.com/keenanjohnson/ghg-gas-cloud/issues) section of this project for the work that I've currently scoped out to be done. Reach out to me if you are interested in helping out!

## Developer
Hi, I'm [Keenan](https://www.keenanjohnson.com/) and I work on this project! Reach out if you are interested or just want to chat.

Consider sponsoring me on [Ko-fi](https://ko-fi.com/W7W14VTU8) or through [Github Sponsors](https://github.com/sponsors/keenanjohnson) to help cover the research and development costs.

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/W7W14VTU8)
