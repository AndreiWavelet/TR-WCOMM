EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 9
Title "WCOMM"
Date "2022-04-06"
Rev "1.0"
Comp "Andrei Alves Cardoso"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_ST_STM32L0:STM32L051K8Tx U1
U 1 1 624C46A6
P 5600 3400
F 0 "U1" H 6050 2250 50  0000 C CNN
F 1 "STM32L051K8Tx" H 6050 2350 50  0000 C CNN
F 2 "Package_QFP:LQFP-32_7x7mm_P0.8mm" H 5100 2500 50  0001 R CNN
F 3 "http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00152023.pdf" H 5600 3400 50  0001 C CNN
	1    5600 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 2500 5600 2400
Wire Wire Line
	5600 2400 5500 2400
Wire Wire Line
	5500 2400 5500 2500
Wire Wire Line
	5500 4400 5500 4500
Wire Wire Line
	5500 4500 5550 4500
Wire Wire Line
	5600 4500 5600 4400
Text HLabel 5550 4600 3    50   Input ~ 0
GND
Wire Wire Line
	5550 4600 5550 4500
Connection ~ 5550 4500
Wire Wire Line
	5550 4500 5600 4500
$Comp
L Connector_Generic:Conn_01x04 J2
U 1 1 624D4544
P 1550 1700
F 0 "J2" H 1468 1275 50  0000 C CNN
F 1 "Conn_01x04" H 1468 1366 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 1550 1700 50  0001 C CNN
F 3 "~" H 1550 1700 50  0001 C CNN
	1    1550 1700
	-1   0    0    1   
$EndComp
Text Label 4900 2700 2    50   ~ 0
~RST
Wire Wire Line
	4900 2700 5000 2700
Text Label 1850 1500 0    50   ~ 0
~RST
Wire Wire Line
	1850 1500 1750 1500
Text Label 1850 1800 0    50   ~ 0
SWCLK
Text Label 1850 1700 0    50   ~ 0
SWDIO
Wire Wire Line
	1850 1600 1750 1600
Wire Wire Line
	1750 1700 1850 1700
Wire Wire Line
	1850 1800 1750 1800
Text HLabel 1850 1600 2    50   Input ~ 0
GND
Text Label 6200 4000 0    50   ~ 0
SWDIO
Wire Wire Line
	6200 4000 6100 4000
Text Label 6200 4100 0    50   ~ 0
SWCLK
Wire Wire Line
	6200 4100 6100 4100
$Comp
L Device:R R4
U 1 1 624DBCCE
P 4300 2900
F 0 "R4" V 4093 2900 50  0000 C CNN
F 1 "10k" V 4184 2900 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4230 2900 50  0001 C CNN
F 3 "~" H 4300 2900 50  0001 C CNN
	1    4300 2900
	0    1    1    0   
$EndComp
Wire Wire Line
	4450 2900 5000 2900
Text HLabel 4050 2900 0    50   Input ~ 0
GND
Wire Wire Line
	4050 2900 4150 2900
$Sheet
S 5200 900  800  400 
U 624D9402
F0 "MCUPower" 50
F1 "MCUPower.sch" 50
F2 "3.3V" I L 5200 1050 50 
F3 "GND" I L 5200 1150 50 
F4 "MCU_VDD" O R 6000 1050 50 
F5 "MCU_VDDA" O R 6000 1150 50 
$EndSheet
Text HLabel 5100 1050 0    50   Input ~ 0
3.3V
Wire Wire Line
	5100 1050 5200 1050
Text HLabel 5100 1150 0    50   Input ~ 0
GND
Wire Wire Line
	5100 1150 5200 1150
Text Label 6100 1050 0    50   ~ 0
VDD
Text Label 6100 1150 0    50   ~ 0
VDDA
Wire Wire Line
	6100 1050 6000 1050
Wire Wire Line
	6000 1150 6100 1150
Text Label 5500 2300 1    50   ~ 0
VDD
Text Label 5700 2300 1    50   ~ 0
VDDA
Wire Wire Line
	5500 2300 5500 2400
Connection ~ 5500 2400
Wire Wire Line
	5700 2300 5700 2500
Text HLabel 6200 3100 2    50   Output ~ 0
~RADIO_CS
Wire Wire Line
	6100 3100 6200 3100
Text HLabel 6200 3200 2    50   Output ~ 0
SPI_SCK
Wire Wire Line
	6200 3200 6100 3200
Text HLabel 6200 3300 2    50   Input ~ 0
SPI_MISO
Text HLabel 6200 3400 2    50   Output ~ 0
SPI_MOSI
Text HLabel 6200 3500 2    50   Output ~ 0
~SD_EN
Text HLabel 6200 3600 2    50   Input ~ 0
SD_DET
Text HLabel 4900 3700 0    50   Output ~ 0
~SD_CS
Wire Wire Line
	4900 3700 5000 3700
Wire Wire Line
	6100 3600 6200 3600
Wire Wire Line
	6100 3500 6200 3500
Wire Wire Line
	6100 3400 6200 3400
Wire Wire Line
	6100 3300 6200 3300
Text HLabel 6200 2700 2    50   Input ~ 0
BAT_FB
Text HLabel 6200 2800 2    50   Output ~ 0
~BAT_FB_EN
Wire Wire Line
	6200 2800 6100 2800
Wire Wire Line
	6100 2700 6200 2700
Text HLabel 4900 4200 0    50   Output ~ 0
LED_R
Wire Wire Line
	4900 4200 5000 4200
Text HLabel 4900 4100 0    50   Output ~ 0
LED_Y
Text HLabel 4900 4000 0    50   Output ~ 0
LED_G
Wire Wire Line
	4900 4000 5000 4000
Wire Wire Line
	4900 4100 5000 4100
Text HLabel 6200 4200 2    50   Output ~ 0
PWR_HOLD
Wire Wire Line
	6200 4200 6100 4200
Text HLabel 6200 3000 2    50   Input ~ 0
RADIO_IRQ
Wire Wire Line
	6200 3000 6100 3000
Text HLabel 6200 2900 2    50   Output ~ 0
RADIO_EN
Wire Wire Line
	6200 2900 6100 2900
Text HLabel 4900 3900 0    50   Input ~ 0
~PWR_BTN
Wire Wire Line
	4900 3900 5000 3900
NoConn ~ 4900 3300
NoConn ~ 4900 3400
NoConn ~ 4900 3600
NoConn ~ 4900 3800
NoConn ~ 6200 3900
NoConn ~ 6200 3800
NoConn ~ 6200 3700
Wire Wire Line
	4900 3800 5000 3800
Wire Wire Line
	5000 3400 4900 3400
Wire Wire Line
	4900 3300 5000 3300
Wire Wire Line
	4900 3600 5000 3600
Wire Wire Line
	6200 3900 6100 3900
Wire Wire Line
	6200 3800 6100 3800
Wire Wire Line
	6200 3700 6100 3700
$EndSCHEMATC
