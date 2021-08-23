
## CO2 Sensor Selection
The current prototype uses the [Sensirion SCD30 CO2 Sensor](https://www.digikey.com/en/products/detail/sensirion-ag/SCD30/8445334?utm_adgroup=Gas%20Sensors&utm_source=google&utm_medium=cpc&utm_campaign=Shopping_Product_Sensors%2C%20Transducers&utm_term=&utm_content=Gas%20Sensors&gclid=Cj0KCQjw2NyFBhDoARIsAMtHtZ4YeCuIrZjKrrVm8ohZNMRxEB0_QjV2AzrOf9Rp4tfHgq8otFP3-v0aAjflEALw_wcB).  It's possible that the newer sensor, the [SCD4x](https://www.sensirion.com/en/environmental-sensors/carbon-dioxide-sensors/carbon-dioxide-sensor-scd4x/) is a better sensor, but this needs to be evaluated.
![image](https://user-images.githubusercontent.com/2559382/120937190-eb61ae80-c6c0-11eb-8ef7-9f8a7e22a168.png)

A list of other available sensors is being compiled on this [Google Sheet](https://docs.google.com/spreadsheets/d/1RpaTCX8PqUjehLUbW_36fwdgL5wZA7mf5JP_UX7CvC8/edit?usp=sharing) (Contact [Eren](https://github.com/eren-rudy) if you want to contribute to this spreadsheet!)


## CO2 Sensor Background Research

**Last modified:** @eren-rudy on Sunday, August 22, 2021

A lot of the commercially available sensors seem to be designed for indoor CO2 monitoring. The goal of this document is to provide some more context regarding the theory behind environmental CO2 measurement (including high-density distributed monitoring).

### [BEACO2N instrumentation methodology](https://acp.copernicus.org/articles/16/13449/2016/acp-16-13449-2016.pdf) 

Overview: This paper provides a very detailed description of the design and testing of a [BEACO2N](http://beacon.berkeley.edu/metadata/) node. Hopefully it can be useful for understanding how to evaluate the Frog sensor later on.

- Introduction
	- Addresses why high-density monitoring is useful
		- There is no method for verifying the compliance to CO2 emissions regulations at a regional scale
		- Lower precision measurements may be useful with a high enough density, since transient variations in CO2 associated with emissions sources are much larger than that of more stable atmospheric conditions that would be measured in the upper atmosphere and in remote locations 
	- 	Cost, reliability, precision, and systematic uncertainty of their system
- BEACO2N node design
	- Lists exact parts used for sensing and reporting data
	- Calibration theory 
	- Deployment methodology (where and how sensors are installed)
		- "Installing sensors near the surface and/or built environment does ensure heightened sensitivity to individual, ground-level emissions phenomena, but it is currently unknown whether a well-reasoned combination of these locally sensitive signals from a high volume of sensors could nonetheless yield reliable information about the integrated region."
- Evaluation of node performance (TBC)