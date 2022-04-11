EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 9
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
L Regulator_Linear:TLV75533PDBV U2
U 1 1 624EAA2A
P 6250 1750
F 0 "U2" H 6250 2092 50  0000 C CNN
F 1 "TLV75533PDBV" H 6250 2001 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 6250 2075 50  0001 C CIN
F 3 "http://www.ti.com/lit/ds/symlink/tlv755p.pdf" H 6250 1800 50  0001 C CNN
	1    6250 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 1750 5850 1750
Wire Wire Line
	5850 1750 5850 1650
Wire Wire Line
	5850 1650 5950 1650
$Comp
L Device:C C6
U 1 1 624ED09A
P 3600 1900
F 0 "C6" H 3715 1946 50  0000 L CNN
F 1 "2u" H 3715 1855 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 3638 1750 50  0001 C CNN
F 3 "~" H 3600 1900 50  0001 C CNN
	1    3600 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 1650 3600 1750
Text HLabel 1600 1650 0    50   Input ~ 0
VBAT+
Text HLabel 1600 2150 0    50   Input ~ 0
VBAT-
Text HLabel 1850 2150 2    50   Output ~ 0
GND
Wire Wire Line
	1850 2150 1700 2150
Text Label 1850 1650 0    50   ~ 0
VBAT
Wire Wire Line
	1850 1650 1600 1650
Text Label 2900 1650 2    50   ~ 0
VBAT
Text HLabel 3600 2250 3    50   Output ~ 0
GND
Text HLabel 6250 2150 3    50   Output ~ 0
GND
Wire Wire Line
	6250 2150 6250 2050
Wire Wire Line
	2900 1650 3600 1650
Wire Wire Line
	3600 2050 3600 2250
$Comp
L Device:C C9
U 1 1 624F3FFD
P 6800 1900
F 0 "C9" H 6915 1946 50  0000 L CNN
F 1 "2u" H 6915 1855 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 6838 1750 50  0001 C CNN
F 3 "~" H 6800 1900 50  0001 C CNN
	1    6800 1900
	1    0    0    -1  
$EndComp
Text HLabel 6800 2250 3    50   Output ~ 0
GND
Wire Wire Line
	6800 2050 6800 2250
Wire Wire Line
	6800 1750 6800 1650
Wire Wire Line
	6800 1650 6550 1650
Text HLabel 7350 1650 2    50   Output ~ 0
3.3V
Wire Wire Line
	7350 1650 6800 1650
Text Label 4050 4150 2    50   ~ 0
VBAT
$Comp
L Device:C C7
U 1 1 62519BBA
P 5200 4400
F 0 "C7" H 5315 4446 50  0000 L CNN
F 1 "1n" H 5315 4355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5315 4309 50  0001 L CNN
F 3 "~" H 5200 4400 50  0001 C CNN
	1    5200 4400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 6251AC18
P 4300 4150
F 0 "R5" V 4093 4150 50  0000 C CNN
F 1 "200k" V 4184 4150 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4230 4150 50  0001 C CNN
F 3 "~" H 4300 4150 50  0001 C CNN
	1    4300 4150
	0    1    1    0   
$EndComp
$Comp
L Device:R R7
U 1 1 6251B8F6
P 4550 4400
F 0 "R7" H 4620 4446 50  0000 L CNN
F 1 "100k" H 4620 4355 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4480 4400 50  0001 C CNN
F 3 "~" H 4550 4400 50  0001 C CNN
	1    4550 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 4150 4550 4150
Wire Wire Line
	4550 4150 4550 4250
Wire Wire Line
	5200 4150 5200 4250
Wire Wire Line
	4550 4550 4550 4650
Wire Wire Line
	5200 4650 5200 4550
Wire Wire Line
	4050 4150 4150 4150
Text HLabel 5400 4150 2    50   Output ~ 0
BAT_FB
Wire Wire Line
	4550 4650 4900 4650
Text HLabel 4900 4750 3    50   Input ~ 0
~BAT_FB_EN
Wire Wire Line
	4900 4750 4900 4650
Connection ~ 4900 4650
Wire Wire Line
	4900 4650 5200 4650
Wire Wire Line
	4550 4150 5200 4150
Connection ~ 4550 4150
Wire Wire Line
	5400 4150 5200 4150
Connection ~ 5200 4150
$Comp
L Device:R R8
U 1 1 624F3E36
P 4750 2050
F 0 "R8" V 4543 2050 50  0000 C CNN
F 1 "570K" V 4634 2050 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4680 2050 50  0001 C CNN
F 3 "~" H 4750 2050 50  0001 C CNN
	1    4750 2050
	0    1    1    0   
$EndComp
Wire Wire Line
	4900 2050 5000 2050
Wire Wire Line
	5000 2050 5000 1950
Wire Wire Line
	4400 2050 4400 1650
$Comp
L Device:C C8
U 1 1 624F7A3F
P 5500 1900
F 0 "C8" H 5615 1946 50  0000 L CNN
F 1 "2u" H 5615 1855 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 5538 1750 50  0001 C CNN
F 3 "~" H 5500 1900 50  0001 C CNN
	1    5500 1900
	1    0    0    -1  
$EndComp
Text HLabel 5500 2250 3    50   Output ~ 0
GND
Wire Wire Line
	5500 2050 5500 2250
Wire Wire Line
	5200 1650 5500 1650
Wire Wire Line
	5500 1650 5500 1750
Wire Wire Line
	5500 1650 5850 1650
Connection ~ 5500 1650
Connection ~ 5850 1650
Connection ~ 3600 1650
Text HLabel 5000 2650 3    50   Input ~ 0
~PWR_HOLD
Connection ~ 5000 2050
$Comp
L Device:R R6
U 1 1 62511CB2
P 4400 2300
F 0 "R6" H 4470 2346 50  0000 L CNN
F 1 "10k" H 4470 2255 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4330 2300 50  0001 C CNN
F 3 "~" H 4400 2300 50  0001 C CNN
	1    4400 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 2150 4400 2050
Connection ~ 4400 2050
Wire Wire Line
	5000 2050 5000 2550
$Comp
L Diode:BAS19 D4
U 1 1 6251592C
P 4650 2550
F 0 "D4" V 4696 2470 50  0000 R CNN
F 1 "BAS19" V 4605 2470 50  0000 R CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 4650 2375 50  0001 C CNN
F 3 "https://www.diodes.com/assets/Datasheets/Ds12004.pdf" H 4650 2550 50  0001 C CNN
	1    4650 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 2050 4600 2050
Wire Wire Line
	4400 1650 4800 1650
Wire Wire Line
	4400 2450 4400 2550
Wire Wire Line
	4400 2550 4500 2550
Wire Wire Line
	4800 2550 5000 2550
Connection ~ 5000 2550
Wire Wire Line
	5000 2550 5000 2650
$Comp
L Switch:SW_Push SW1
U 1 1 6251E471
P 4400 2850
F 0 "SW1" H 4400 3135 50  0000 C CNN
F 1 "SW_Push" H 4400 3044 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm_H13mm" H 4400 3050 50  0001 C CNN
F 3 "~" H 4400 3050 50  0001 C CNN
	1    4400 2850
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4400 2650 4400 2550
Connection ~ 4400 2550
Text HLabel 4400 3150 3    50   Input ~ 0
GND
Wire Wire Line
	4400 3150 4400 3050
Text HLabel 4300 2550 0    50   Output ~ 0
~PWR_BTN
Wire Wire Line
	4300 2550 4400 2550
Connection ~ 6800 1650
Wire Wire Line
	3600 1650 4400 1650
Connection ~ 4400 1650
$Comp
L Transistor_FET:DMG2301L Q1
U 1 1 6252D070
P 5000 1750
F 0 "Q1" V 5342 1750 50  0000 C CNN
F 1 "DMG2301L" V 5251 1750 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 5200 1675 50  0001 L CIN
F 3 "https://www.diodes.com/assets/Datasheets/DMG2301L.pdf" H 5000 1750 50  0001 L CNN
	1    5000 1750
	0    1    -1   0   
$EndComp
$Comp
L power:PWR_FLAG #FLG02
U 1 1 624FD188
P 1700 2250
F 0 "#FLG02" H 1700 2325 50  0001 C CNN
F 1 "PWR_FLAG" H 1700 2423 50  0000 C CNN
F 2 "" H 1700 2250 50  0001 C CNN
F 3 "~" H 1700 2250 50  0001 C CNN
	1    1700 2250
	-1   0    0    1   
$EndComp
Wire Wire Line
	1700 2250 1700 2150
Connection ~ 1700 2150
Wire Wire Line
	1700 2150 1600 2150
$Comp
L power:PWR_FLAG #FLG03
U 1 1 624FE098
P 5500 1550
F 0 "#FLG03" H 5500 1625 50  0001 C CNN
F 1 "PWR_FLAG" H 5500 1723 50  0000 C CNN
F 2 "" H 5500 1550 50  0001 C CNN
F 3 "~" H 5500 1550 50  0001 C CNN
	1    5500 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 1550 5500 1650
Text Notes 4100 3650 0    50   ~ 0
Connect all signals to 5V-tolerant uC pin
$EndSCHEMATC
