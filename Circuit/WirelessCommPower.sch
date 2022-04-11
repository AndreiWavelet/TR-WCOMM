EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 6 9
Title "WCOMM"
Date "2022-04-06"
Rev "1.0"
Comp "Andrei Alves Cardoso"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 4150 3000 0    50   Input ~ 0
3.3V
$Comp
L Device:C C17
U 1 1 62544396
P 4500 3250
F 0 "C17" H 4615 3296 50  0000 L CNN
F 1 "2u" H 4615 3205 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4538 3100 50  0001 C CNN
F 3 "~" H 4500 3250 50  0001 C CNN
	1    4500 3250
	1    0    0    -1  
$EndComp
$Comp
L Device:C C18
U 1 1 62544C95
P 5000 3250
F 0 "C18" H 5115 3296 50  0000 L CNN
F 1 "100n" H 5115 3205 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5038 3100 50  0001 C CNN
F 3 "~" H 5000 3250 50  0001 C CNN
	1    5000 3250
	1    0    0    -1  
$EndComp
$Comp
L Device:C C19
U 1 1 625454E3
P 5500 3250
F 0 "C19" H 5615 3296 50  0000 L CNN
F 1 "100n" H 5615 3205 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5538 3100 50  0001 C CNN
F 3 "~" H 5500 3250 50  0001 C CNN
	1    5500 3250
	1    0    0    -1  
$EndComp
$Comp
L Device:C C20
U 1 1 62545DE6
P 6000 3250
F 0 "C20" H 6115 3296 50  0000 L CNN
F 1 "100n" H 6115 3205 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6038 3100 50  0001 C CNN
F 3 "~" H 6000 3250 50  0001 C CNN
	1    6000 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 3000 4500 3000
Wire Wire Line
	4500 3000 4500 3100
Wire Wire Line
	4500 3000 5000 3000
Wire Wire Line
	5000 3000 5000 3100
Connection ~ 4500 3000
Wire Wire Line
	5000 3000 5500 3000
Wire Wire Line
	5500 3000 5500 3100
Connection ~ 5000 3000
Wire Wire Line
	5500 3000 6000 3000
Wire Wire Line
	6000 3000 6000 3100
Connection ~ 5500 3000
Wire Wire Line
	4500 3400 4500 3500
Wire Wire Line
	4500 3500 5000 3500
Wire Wire Line
	5000 3500 5000 3400
Wire Wire Line
	5000 3500 5250 3500
Wire Wire Line
	5500 3500 5500 3400
Connection ~ 5000 3500
Wire Wire Line
	5500 3500 6000 3500
Wire Wire Line
	6000 3500 6000 3400
Connection ~ 5500 3500
Text HLabel 5250 3600 3    50   Input ~ 0
GND
Wire Wire Line
	5250 3600 5250 3500
Connection ~ 5250 3500
Wire Wire Line
	5250 3500 5500 3500
Text HLabel 6500 3000 2    50   Output ~ 0
WCOMM_VDD
Wire Wire Line
	6500 3000 6000 3000
Connection ~ 6000 3000
Text Notes 4400 4150 0    50   ~ 0
Place decoupling capacitors near NRF power pins
$EndSCHEMATC
