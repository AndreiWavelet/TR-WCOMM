EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 8 9
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
L Device:R R?
U 1 1 6250A146
P 4950 3650
AR Path="/624F5223/6250A146" Ref="R?"  Part="1" 
AR Path="/624F5223/625068E2/6250A146" Ref="R13"  Part="1" 
F 0 "R13" V 4743 3650 50  0000 C CNN
F 1 "100k" V 4834 3650 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4880 3650 50  0001 C CNN
F 3 "~" H 4950 3650 50  0001 C CNN
	1    4950 3650
	0    1    1    0   
$EndComp
Wire Wire Line
	5100 3650 5200 3650
Wire Wire Line
	5200 3650 5200 3550
Wire Wire Line
	4800 3650 4700 3650
Wire Wire Line
	4700 3650 4700 3250
Wire Wire Line
	4700 3250 5000 3250
$Comp
L Device:C C?
U 1 1 6250A154
P 5600 3500
AR Path="/624F5223/6250A154" Ref="C?"  Part="1" 
AR Path="/624F5223/625068E2/6250A154" Ref="C23"  Part="1" 
F 0 "C23" H 5715 3546 50  0000 L CNN
F 1 "2u" H 5715 3455 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 5638 3350 50  0001 C CNN
F 3 "~" H 5600 3500 50  0001 C CNN
	1    5600 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 3350 5600 3250
Wire Wire Line
	5600 3250 5400 3250
$Comp
L Device:C C?
U 1 1 6250A15C
P 6000 3500
AR Path="/624F5223/6250A15C" Ref="C?"  Part="1" 
AR Path="/624F5223/625068E2/6250A15C" Ref="C24"  Part="1" 
F 0 "C24" H 6115 3546 50  0000 L CNN
F 1 "100n" H 6115 3455 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6038 3350 50  0001 C CNN
F 3 "~" H 6000 3500 50  0001 C CNN
	1    6000 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 3250 6000 3350
Connection ~ 5600 3250
Wire Wire Line
	5600 3750 5800 3750
Wire Wire Line
	6000 3750 6000 3650
Wire Wire Line
	5600 3650 5600 3750
Connection ~ 6000 3250
$Comp
L Device:C C22
U 1 1 6250A5D9
P 4250 3500
F 0 "C22" H 4365 3546 50  0000 L CNN
F 1 "100n" H 4365 3455 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4288 3350 50  0001 C CNN
F 3 "~" H 4250 3500 50  0001 C CNN
	1    4250 3500
	1    0    0    -1  
$EndComp
$Comp
L Device:C C21
U 1 1 6250ABF3
P 3850 3500
F 0 "C21" H 3965 3546 50  0000 L CNN
F 1 "2u" H 3965 3455 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 3888 3350 50  0001 C CNN
F 3 "~" H 3850 3500 50  0001 C CNN
	1    3850 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 3250 4250 3250
Wire Wire Line
	4250 3250 4250 3350
Connection ~ 4700 3250
Wire Wire Line
	4250 3250 3850 3250
Wire Wire Line
	3850 3250 3850 3350
Connection ~ 4250 3250
Wire Wire Line
	3850 3650 3850 3750
Wire Wire Line
	3850 3750 4050 3750
Wire Wire Line
	4250 3750 4250 3650
Text HLabel 6350 3250 2    50   Output ~ 0
SD_3.3V
Wire Wire Line
	6000 3250 6350 3250
Text HLabel 3750 3250 0    50   Input ~ 0
3.3V
Wire Wire Line
	3750 3250 3850 3250
Connection ~ 3850 3250
Text HLabel 4050 3850 3    50   Input ~ 0
GND
Text HLabel 5800 3850 3    50   Input ~ 0
GND
Wire Wire Line
	5800 3850 5800 3750
Wire Wire Line
	4050 3850 4050 3750
Connection ~ 4050 3750
Wire Wire Line
	4050 3750 4250 3750
Connection ~ 5800 3750
Wire Wire Line
	5800 3750 6000 3750
Text HLabel 5200 3850 3    50   Input ~ 0
~EN
Wire Wire Line
	5200 3850 5200 3650
Connection ~ 5200 3650
$Comp
L Transistor_FET:DMG2301L Q2
U 1 1 62530805
P 5200 3350
F 0 "Q2" V 5542 3350 50  0000 C CNN
F 1 "DMG2301L" V 5451 3350 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 5400 3275 50  0001 L CIN
F 3 "https://www.diodes.com/assets/Datasheets/DMG2301L.pdf" H 5200 3350 50  0001 L CNN
	1    5200 3350
	0    1    -1   0   
$EndComp
Wire Wire Line
	5600 3250 5800 3250
$Comp
L power:PWR_FLAG #FLG04
U 1 1 62502D52
P 5800 3150
F 0 "#FLG04" H 5800 3225 50  0001 C CNN
F 1 "PWR_FLAG" H 5800 3323 50  0000 C CNN
F 2 "" H 5800 3150 50  0001 C CNN
F 3 "~" H 5800 3150 50  0001 C CNN
	1    5800 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 3150 5800 3250
Connection ~ 5800 3250
Wire Wire Line
	5800 3250 6000 3250
$EndSCHEMATC
