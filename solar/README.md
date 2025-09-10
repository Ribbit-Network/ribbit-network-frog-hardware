# Solar + Cellular Overview
The solar + cellular iterations of the frog sensor allow the sensor to operate in a wider variety of environments, as they do not require a power and wifi source. This is intended to enable a wider range of data gathering

# Versions
| Version | Description |
| :---: | :--- |
| Solar V1.0 | Desmond’s final submission to Hackster contest. Technically functional and testable, but worth making a few updates before production. |
| Solar V1.1 [WIP] | Slightly updated version of Desmond’s submission. Changes include (non-exhaustive): <br> - Updated footprint mapping for Q5 <br><br>Objectives: Gather useful data, test |
| Solar V1.2? [future] | TBD. Solves a more specific problem as defined by stakeholders and longer-term project objectives, e.g. data gathered is of 'scientific rigor,' enables 1 sensor per satellite pixel, deployable with targeted strategy in mind (e.g. doesn't require soldering if that's not realistic for target group, swappable sensors if that increases uptake) |

# Solar V1.0 Overview
V1.0 is the winning submission from the Power Hop Challenge on Hackster in early 2025. See documentation at the following links.  
Project overview: [The Power Hop Challenge](https://www.hackster.io/contests/ribbithop)  
Hackster Pt 1: [Ribbit Power Hop Challenge - Design Proposal](https://www.hackster.io/contests/ribbithop)  
Hackster Pt 2: [Ribbit Power Hop Challenge - Layout and Bring-up](https://www.hackster.io/desmondjgood/ribbit-power-hop-challenge-layout-and-bring-up-9ce33b)  

# Solar V 1.1 Overview
## Power Budget
The solar V1.1 power system:
- Should support the tests outlined in the test plan
- Should operate with the as-designed 1x 18650 LiFiPo battery on-board storage
- Should take a solar input that is compatible with the 3.5V-48V input jack (_is there also a range for the power mgmt system?_)
- May use any solar panel input that meets these requirements

The [V1.0 power budget](https://www.hackster.io/desmondjgood/ribbit-power-hop-challenge-design-proposal-83d578#toc-system-power-budget-4) estimates a __1.5mA current draw__ and assuming:
- 3-min active period per hour, 30s sampling interval when active (5% duty cycle, 5-6 samples)
- 75% DC-DC conversion efficiency
- Estimated average current draws of component parts (see Desmond's list)
  - Frog Board Avg. Current: 80 uA
  - Sensor Bus Avg. Current: 854 uA

For 1.5mA at 3.3V (_battery voltage(3.2V)? USB voltage (5V)? Bus voltage (3.3V)?_), this is 5mW of power. So for this setup, even the tiny [5V/0.3W adafruit solar panel](https://www.adafruit.com/product/5855) provides suffient input current to consistently maintain a 90-100% state of charge, with 60x margin (at full sun?).  
_(This specific panel may be connected by soldering cut/free leads of a [DC barrel plug](https://www.adafruit.com/product/1329) to the back of the panel. Could also do something [like this](https://www.amazon.com/Kanayu-Panels3V-Polycrystalline-Photovoltaic-Flashlight/dp/B0F2N2Y9J2/ref=sr_1_14?dib=eyJ2IjoiMSJ9.BO0LDHWpcz6lu-0cINqNXTSFJq9hmFPpJo4c5hZR8Wc3oMh3IEXXgs3VTrCD7n56oOkyUpHWXiPHYtoMc92PT7HiCuDxdC85uQQuIZnvGjdIp6DTkB6zwGtRD52pZeaFPgqwZGJ7IR7SLsPsDkhV4EdQVhQ6Nry6_45e8oMzkeDrEHzlU9jGdBrf5TpjGlFP7moKuzDi3BAwgls2-c6LEJD2JWR1ezP4tShUAfbHmHY.hea1Cp4MXNaj6MR2c5aIa2zDrEi5s75LSvI5Isuft7s&dib_tag=se&keywords=0.3w%2Bsolar%2Bpanel&qid=1757539879&sr=8-14&th=1) that already has them integrated and is smaller per-unit cost.)_

## BOM
### Assembly Bill of Materials (estimated)					
| Description |	Qty	| Unit cost	| Total cost | Manufacturer/Supplier |	Manufacturer's Part Number/Link |
|-|-|-|-|-|-|
| PCB (custom) | 	1	| $10.00	| $10.00 | Estimate, JLC | |
| Antenna and cable, RF ANT LTE RIGID PCB ADHES UFL	| 1	| $5.36	| $5.36 |	TE Connectivity Linx | ANT-LTE-RPC-UFL |
| Battery, LFP 18650	| 1	| $5.85	| $5.85 |	Generic, see example mfrr part | PCIFR18650-1500 |
| SIM card (optional)	| 1	| $5.00	| $5.00 |	Generic | |	
| CO2 sensor	| 1	| $58.95	| $58.95 | SCD-30 - NDIR CO2 Temperature and Humidity Sensor |	https://www.adafruit.com/product/4867 |
| Barometer	| 1	| $6.95	| $6.95 |	Adafruit DPS310 Precision Barometric Pressure / Altitude Sensor |	https://www.adafruit.com/product/4494 |
| Solar panel	| 1	| $5.95	|	$5.95	|	Adafruit 5V 0.3W Mini Solar Panel - ETFE - Voltaic P122	| https://www.adafruit.com/product/5855 |
| Solar panel adapter	 |1 |	$1.95 |	$1.95 |	2.1mm DC Barrel Plug to Alligator Clips |	https://www.adafruit.com/product/1329 |	
| Other potential adapters etc ||||||
| USBC Adapter	| 1	| $3.95	| $3.95 | | |	
| STEMMA QT / Qwiic JST SH 4-pin Cable - 50mm Long	| 2	| $0.95	| $1.90	| | |
| STEMMA QT / Qwiic JST SH 4-pin Cable - 100mm Long	| 1	| $0.95	| $0.95 | | |
					
		Total cost	$106.81		
### Board BOM
<details>
  <summary>Click to expand</summary>
  
_(to finish formatting later)_ 
| Description |	Qty |	Manufacturer |	Manufacturer's Part Number |	Reference |
|--|--|--|--|--|
| CAP CER 0.1UF 16V X7R 0201 |	10 |	Murata Electronics |	GRM033Z71C104KE14J |	C1,C4,C7,C9,C12,C14,C15,C45,C51,C53 |
| CAP CER 1UF 10V X7R 0402 |	1 |	Murata Electronics |	GRM155Z71A105KE01D |	C2 |
| CAP CER 47UF 10V X5R 080 5 |	3 |	Murata Electronics |	GRM21BR61A476ME15K |	C3,C5,C35 |
| CAP CER 4.7UF 10V X5R 0402 |	4 |	Murata Electronics |	GRM155R61A475MEAAD |	C6,C8,C27,C42 |
| CAP CER 100PF 50V C0G/NP0 0201 |	3 |	Murata Electronics |	GRM0335C1H101JA01D |	C10,C11,C55 |
| CAP CER 10000PF 25V X7R 0402 |	3 |	YAGEO |	CC0402KRX7R8BB103 |	C13,C41,C47 |
| CAP CER 10UF 25V X5R 0805 |	9 |	Murata Electronics |	GRM21BR61E106KA73L |	C17,C18,C21,C22,C33,C34,C37,C59,C60 |
| CAP CER 0.1UF 50V X7R 0402 |	7 |	Murata Electronics |	GRM155R71H104KE14D |	C19,C20,C23,C24,C32,C43,C61 |
| CAP CER 1000PF 25V X7R 0201 |	3 |	Murata Electronics |	GRM033R71E102KA01D |	C25,C26,C40 |
| CAP CER 10UF 10V X5R 0805 |	1 |	KEMET |	C0805C106K8PACTU |	C28 |
| CAP CER 0.047UF 25V X7R 0402	2	Murata Electronics	GRM155R71E473KA88D	C29,C30 |
| CAP CER 1000PF 50V C0G/NP0 0402	1	Murata Electronics	GRM1555C1H102JA01D	C31 |
| CAP CER 4.7UF 6.3V X7R 0603	3	Samsung Electro-Mechanics	CL10B475KQ8NQNC	C44,C46,C49 |
| CAP CER 1UF 10V X7S 0402	2	Murata Electronics	GCM155C71A105KE38D	C48,C50 |
| CAP CER 0.047UF 10V X7S 0201	1	TDK Corporation	C0603X7S1A473K030BC	C52 |
| CAP CER 12PF 50V C0G/NP0 0201	2	Murata Electronics	GRM0335C1H120GA01D	C56,C57 |
| CAP CER 1.2PF 50V C0G/NP0 0201	1	Murata Electronics	GRM0335C1H1R2WA01D	C58 |
| CONN RCP USB2.0 TYP C 24P SMD RA	1	GCT	USB4105-GF-A	CN1 |
| LED GREEN DIFFUSED 0402 SMD	1	Broadcom Limited	HSMG-C280	D1 |
| LED RED DIFFUSED SMD	1	Broadcom Limited	HSMS-C280	D2 |
| LED YELLOW DIFFUSED SMD	1	Broadcom Limited	HSMY-C280	D3 |
| BATTERY CONTACT CLIP 18650 SMD	2	Keystone Electronics	254	J1,J2
| CONN JACK R/A PCB 3.5X1.1MM	1	Tensility International Corp	54-00125	J3
| CONN HEADER SMD 2POS 2.54MM	7	Amphenol ICC (FCI)	10129380-902001BLF	J4,J5,J6,J7,J8,J9,J14
| CONN HEADER SMD 4 POS 1 MM	1	JST Sales America Inc.	BM04B-SRSS-TB	J10
| CONN UMC JACK STR SMD	1	TE Connectivity Linx	CONMHF1-SMD-G-T	J11
FTSH-105-01-L-DV-K	2	Samtec Inc.	FTSH-105-01-L-DV-K	J12,J15
CONN NANO SIM CARD R/A SMD	1	JAE Electronics	SF72S006VBDR2500	J13
CONN JUMPER SHORTING .100" GOLD	7	Sullins Connector Solutions	QPC02SXGN-RC	JMP1,JMP2,JMP3,JMP4,JMP5,JMP6,JMP7
FERRITE BEAD 120 OHM 0402 1LN	1	Murata Electronics	BLM15PX121SN1D	L1
FIXED IND 9.1NH 540MA 0.14OHM SM	1	Murata Electronics	LQW15AN9N1J00D	L2
FIXED IND 1UH 2.1A 36 MOHM SMD	2	Taiyo Yuden	LSXND3030QKT1R0NNG	L3,L4
FERRITE BEAD 50 OHM 1206 4LN	1	Murata Electronics	BLM31PG500SN1L	L5
FIXED IND 15NH 450MA 0.32OHM SMD	1	Murata Electronics	LQG15HS15NJ02D	L6
FIXED IND 10UH 300MA 600MOHM SMD	1	TDK Corporation	MLZ1608N100LT000	L7
FIXED IND 4.7NH 900MA 140MOHM SM	1	Murata Electronics	LQP03PN4N7J02D	L8
FIXED IND 2.2NH 600MA 150MOHM SM	1	Murata Electronics	LQP03TN2N2B02D	L9
MOSFET N CH 12V 11A U-DFN2020-6E	4	Diodes Incorporated	DMN1019UFDE-7	Q1,Q2,Q3,Q4
MOSFET P-CH 20V 6A SOT23F	1	Toshiba Semiconductor and Storage	SSM3J355R,LF	Q5
MOSFET 2N-CH 20V 23A 6WLCSP	1	onsemi	EFC3J018NUZTDG	Q6
RES SMD 0 OHM JUMPER 1/10W 0402	6	Panasonic Electronic Components	ERJ-2GE0R00X	R1,R3,R7,R9,R33,R34
RES SMD 10 kΩ ±1 % 1/10 W 0402	5	Panasonic Electronic Components	ERJ-2RKF1002X	R2,R18,R22,R23,R24
RES SMD 0 OHM JUMPER 1/20 W 0201	2	Panasonic Electronic Components	ERJ-1GN0R00C	R5,R39
RES SMD 1 MΩ ±1 % 1/10 W 0402	1	Panasonic Electronic Components	ERJ-2RKF1004X	R8
RES SMD 4.7 kΩ ±1 % 1/10 W 0402	2	Panasonic Electronic Components	ERJ-2RKF4701X	R10,R12
RES SMD 1 kΩ ±1 % 1/10 W 0402	2	Panasonic Electronic Components	ERJ-2RKF1001X	R14,R35
RES SMD 3 kΩ ±1 % 1/10 W 0402	1	Panasonic Electronic Components	ERJ-2RKF3001X	R15
RES SMD 294 Ω ±1 % 1/10 W 0402	2	Panasonic Electronic Components	ERJ-2RKF2940X	R16,R17
RES SMD 100 Ω ±1 % 1/10 W 0402	1	Panasonic Electronic Components	ERJ-2RKF1000X	R19
RES 511 kΩ ±1 % 1/16 W 0402	1	Vishay Dale	CRCW0402511KFKEDC	R20
RES SMD 91 kΩ ±1 % 1/10 W 0402	1	Panasonic Electronic Components	ERJ-2RKF9102X	R21
RES SMD 590 Ω ±1 % 1/10 W 0402	3	Panasonic Electronic Components	ERJ-2RKF5900X	R25,R36,R37
RES 215 kΩ ±1 % 1/16 W 0402	1	YAGEO	RC0402FR-07215KL	R26
RES 59 kΩ ±1 % 1/16 W 0402	1	YAGEO	RC0402FR-0759KL	R27
RES 30.1 kΩ ±1 % 1/16 W 0402	1	YAGEO	RC0402FR-0730K1L	R28
RES 5.23 kΩ ±1 % 1/16 W 0402	1	YAGEO	RC0402FR-075K23L	R29
THERMISTOR NTC 10KOHM 3380K 0402	1	TDK Corporation	NTCG103JF103FT1	R30
RES SMD 5.1K OHM 1% 1/10W 0402	2	Panasonic Electronic Components	ERJ-2RKF5101X	R31,R32
RES SMD 20 OHM 1% 1/10W 0402	1	Panasonic Electronic Components	ERJ-2RKF20R0X	R41
RES SMD 300 OHM 1% 1/10W 0402	1	Panasonic Electronic Components	ERJ-2RKF3000X	R42
RES 0.003 OHM 1% 1/2W 0805	1	Rohm Semiconductor	PMR10EZPFV3L00	R43
RES SMD 2K OHM 1% 1/10W 0402	1	Panasonic Electronic Components	ERJ-2RKF2001X	R44
RES 100K OHM 1% 1/16W 0402	1	YAGEO	RC0402FR-07100KL	R45
SWITCH TACTILE SPST-NO 0.05A 12V	2	C&K	PTS636SM50JSMTR LFS	SW1,SW2
PC TEST POINT	6	KOA Speer Electronics, Inc.	RCWCTE	TP1,TP2,TP3,TP4,TP5,TP6
IC RF TXRX + MCU CELL	1	Nordic Semiconductor ASA	NRF9151-LACA-R	U1
GPS LNA MODULE	1	Skyworks Solutions Inc.	SKY65943-11	U2
FILTER RC(PI) 47 OHMSESD SMD	1	STMicroelectronics	EMIF03-SIM02M8	U3
IC FLASH 8 Mbit SPI 104 MHz 8-USON (NOR memory)	1	Macronix	MX25V8035FZUI	U4
IC CONTROLLED, 1–4-CELL, 5-A BUC (Charger IC)	1	Texas Instruments	BQ25798RQMR	U5
IC REG BUCK BST ADJ 1.5 A 8 DSBGA	1	Texas Instruments	TPS631010YBGR	U6
5.5-V, 3.5 A, 20-mΩ on-resistance load switch (6-WQFN-HR)	1	Texas Instruments	TPS22995RZFR	U7
TVS DIODE 5.25 VWM 17 VC SOT-23-6	1	STMicroelectronics	USBLC6-4SC6	U9
IC RF TXRX + MCU 802.15.4 40 QFN	1	Nordic Semiconductor ASA	NRF52833-QDAA-B-R7	U10
IC BATT PROT LI-ION 1 CELL 8X2QFN	1	Texas Instruments	BQ298000RUGT	U11
32 MHz GSX‑223/1P3MF SM Crystal (32 MHz ±10 ppm, 8 pF, 50 Ω, 4‑SMD, no lead)	1	Golledge Electronics Ltd	MP06003	XTAL1
</details>

## Bring-Up Code
_Need to pull in from Desmond's files in hackster_

## Test Plan
The following tests are potentially desirable for the V1.1 iteration: (rough ideas - these might not all make sense but are a brainstorm)
| Test Type | Metric | Variable | Appx setup |
| --- | --- | --- | --- |
| Actual power draw | State of charge (SoC) over time | Environment (cloudy/shady/dark, rainy/wet, hot/cold) | Deploy alongside known conditions and compare data(?) |
||| Length of active period and sampling interval, continuous operation | Change periods in software and compare data |
||| Solar/no solar; period and sampling interval | Deploy fully charged without a solar panel and see how long it lasts |
||| SoC on deploy | Deploy with no/partial charge and see if reaches full charge (does this work for LiFePo batteries?)
| CO2/Sensor calibration | Accuracy, precision of sensor data | Time in field | Deploy alongside known concentrations (e.g. a fancier sensor) for a sufficiently long period of time |
||| Number of sensors? | Deploy multiple sensors at known concentration and see if math makes data better (processed) |
| Cellular connectivity | Latency, errors, other issues | Location - clear/obstructed path to sky | Put (same sensor?) in different spots and test connection |
||| Location - latitude/longitude | Put statistically significant number of sensors around the globe and see how connections fare |
||| Antenna selection | Try different antennas and see if there's a difference |
| Physical robustness | Visual inspection | Time, environmental conditions, all previous variables | For all above tests, take photos before, during, after tests and inspect for physical failures |








