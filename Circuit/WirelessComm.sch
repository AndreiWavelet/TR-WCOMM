EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 9
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
L RF:nRF24L01P U3
U 1 1 6254245A
P 5300 3550
F 0 "U3" H 5700 4400 50  0000 C CNN
F 1 "nRF24L01P" H 5700 4300 50  0000 C CNN
F 2 "Package_DFN_QFN:QFN-20-1EP_4x4mm_P0.5mm_EP2.5x2.5mm" H 5500 4350 50  0001 L CIN
F 3 "http://www.nordicsemi.com/eng/content/download/2726/34069/file/nRF24L01P_Product_Specification_1_0.pdf" H 5300 3650 50  0001 C CNN
	1    5300 3550
	1    0    0    -1  
$EndComp
$Comp
L Device:Crystal Y1
U 1 1 62542461
P 6400 4450
F 0 "Y1" H 6400 4718 50  0000 C CNN
F 1 "16M" H 6400 4627 50  0000 C CNN
F 2 "Crystal:Crystal_SMD_HC49-SD" H 6400 4450 50  0001 C CNN
F 3 "https://suntsu.com/wp-content/uploads/2020/09/SXTHM2-Series.pdf" H 6400 4450 50  0001 C CNN
F 4 "SXTHM212AA164-16.000MT" H 6400 4450 50  0001 C CNN "PN"
	1    6400 4450
	1    0    0    -1  
$EndComp
$Comp
L Device:C C11
U 1 1 62542467
P 6050 4800
F 0 "C11" H 5935 4754 50  0000 R CNN
F 1 "24p NPO" H 5935 4845 50  0000 R CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6088 4650 50  0001 C CNN
F 3 "~" H 6050 4800 50  0001 C CNN
	1    6050 4800
	-1   0    0    1   
$EndComp
$Comp
L Device:C C14
U 1 1 6254246D
P 6750 4800
F 0 "C14" H 6865 4846 50  0000 L CNN
F 1 "24p NPO" H 6865 4755 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6788 4650 50  0001 C CNN
F 3 "~" H 6750 4800 50  0001 C CNN
	1    6750 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 4650 6050 4450
Wire Wire Line
	6050 4450 6250 4450
Wire Wire Line
	6750 4650 6750 4450
Wire Wire Line
	6750 4450 6550 4450
Wire Wire Line
	6050 4950 6050 5050
Wire Wire Line
	6050 5050 6400 5050
Wire Wire Line
	6750 5050 6750 4950
$Sheet
S 7750 950  850  300 
U 62543FA0
F0 "WirelessCommPower" 50
F1 "WirelessCommPower.sch" 50
F2 "3.3V" I L 7750 1050 50 
F3 "GND" I L 7750 1150 50 
F4 "WCOMM_VDD" O R 8600 1050 50 
$EndSheet
Text HLabel 7650 1050 0    50   Input ~ 0
3.3V
Text HLabel 7650 1150 0    50   Input ~ 0
GND
Wire Wire Line
	7650 1150 7750 1150
Wire Wire Line
	7650 1050 7750 1050
Text Label 8700 1050 0    50   ~ 0
WCOMM_VDD
Wire Wire Line
	8700 1050 8600 1050
Text Label 5300 2550 1    50   ~ 0
WCOMM_VDD
Wire Wire Line
	5200 2750 5200 2650
Wire Wire Line
	5200 2650 5300 2650
Wire Wire Line
	5400 2650 5400 2750
Wire Wire Line
	5300 2750 5300 2650
Connection ~ 5300 2650
Wire Wire Line
	5300 2650 5400 2650
Wire Wire Line
	5300 2650 5300 2550
Wire Wire Line
	5200 4350 5200 4450
Wire Wire Line
	5200 4450 5300 4450
Wire Wire Line
	5500 4450 5500 4350
Wire Wire Line
	5400 4350 5400 4450
Connection ~ 5400 4450
Wire Wire Line
	5400 4450 5500 4450
Wire Wire Line
	5300 4350 5300 4450
Connection ~ 5300 4450
Wire Wire Line
	5300 4450 5350 4450
Text HLabel 5350 4550 3    50   Input ~ 0
GND
Wire Wire Line
	5350 4550 5350 4450
Connection ~ 5350 4450
Wire Wire Line
	5350 4450 5400 4450
$Comp
L Device:C C10
U 1 1 62549C8D
P 4600 4300
F 0 "C10" H 4715 4346 50  0000 L CNN
F 1 "33n" H 4715 4255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4638 4150 50  0001 C CNN
F 3 "~" H 4600 4300 50  0001 C CNN
	1    4600 4300
	1    0    0    -1  
$EndComp
Text HLabel 4450 4650 3    50   Input ~ 0
GND
Wire Wire Line
	4600 4550 4600 4450
Wire Wire Line
	4600 4150 4600 4050
Wire Wire Line
	4600 4050 4700 4050
$Comp
L Device:R R9
U 1 1 6254B0F8
P 4300 4300
F 0 "R9" H 4370 4346 50  0000 L CNN
F 1 "22k" H 4370 4255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" V 4230 4300 50  0001 C CNN
F 3 "~" H 4300 4300 50  0001 C CNN
	1    4300 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 4150 4300 3850
Wire Wire Line
	4300 3850 4700 3850
Wire Wire Line
	4300 4450 4300 4550
Wire Wire Line
	4300 4550 4450 4550
Wire Wire Line
	4450 4650 4450 4550
Connection ~ 4450 4550
Wire Wire Line
	4450 4550 4600 4550
Wire Wire Line
	5900 4050 6050 4050
Wire Wire Line
	6050 4050 6050 4450
Connection ~ 6050 4450
Wire Wire Line
	5900 3850 6750 3850
Wire Wire Line
	6750 3850 6750 4450
Connection ~ 6750 4450
Text HLabel 6400 5150 3    50   Input ~ 0
GND
Wire Wire Line
	6400 5150 6400 5050
Connection ~ 6400 5050
Wire Wire Line
	6400 5050 6750 5050
$Comp
L pspice:INDUCTOR L3
U 1 1 6255147C
P 7400 3350
F 0 "L3" V 7446 3306 50  0000 R CNN
F 1 "8.2n" V 7355 3306 50  0000 R CNN
F 2 "Inductor_SMD:L_0603_1608Metric" H 7400 3350 50  0001 C CNN
F 3 "~" H 7400 3350 50  0001 C CNN
	1    7400 3350
	0    -1   -1   0   
$EndComp
$Comp
L pspice:INDUCTOR L2
U 1 1 625524EB
P 7400 2650
F 0 "L2" V 7446 2606 50  0000 R CNN
F 1 "2.7n" V 7355 2606 50  0000 R CNN
F 2 "Inductor_SMD:L_0603_1608Metric" H 7400 2650 50  0001 C CNN
F 3 "~" H 7400 2650 50  0001 C CNN
	1    7400 2650
	0    -1   -1   0   
$EndComp
$Comp
L pspice:INDUCTOR L4
U 1 1 62553B6D
P 7950 3700
F 0 "L4" H 7950 3915 50  0000 C CNN
F 1 "3.9n" H 7950 3824 50  0000 C CNN
F 2 "Inductor_SMD:L_0603_1608Metric" H 7950 3700 50  0001 C CNN
F 3 "~" H 7950 3700 50  0001 C CNN
	1    7950 3700
	1    0    0    -1  
$EndComp
$Comp
L Device:C C13
U 1 1 62556504
P 6750 2550
F 0 "C13" H 6865 2596 50  0000 L CNN
F 1 "4.7p NPO" H 6865 2505 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6788 2400 50  0001 C CNN
F 3 "~" H 6750 2550 50  0001 C CNN
	1    6750 2550
	1    0    0    -1  
$EndComp
$Comp
L Device:C C12
U 1 1 62556F5E
P 6300 2550
F 0 "C12" H 6415 2596 50  0000 L CNN
F 1 "2.2n" H 6415 2505 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6338 2400 50  0001 C CNN
F 3 "~" H 6300 2550 50  0001 C CNN
	1    6300 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 3250 6500 3250
Wire Wire Line
	6500 3250 6500 3000
Wire Wire Line
	7400 3000 7400 3100
Wire Wire Line
	5900 3450 6500 3450
Wire Wire Line
	7400 3000 7400 2900
Connection ~ 7400 3000
Wire Wire Line
	7400 2400 7400 2300
Wire Wire Line
	7400 3600 7400 3700
Wire Wire Line
	7400 3700 7700 3700
Wire Wire Line
	6500 3450 6500 3700
Connection ~ 7400 3700
Wire Wire Line
	6000 2300 6000 3050
Wire Wire Line
	6000 3050 5900 3050
Wire Wire Line
	6300 2300 6300 2400
Connection ~ 6300 2300
Wire Wire Line
	6300 2300 6000 2300
Wire Wire Line
	6750 2400 6750 2300
Connection ~ 6750 2300
Wire Wire Line
	6750 2300 6300 2300
Wire Wire Line
	6300 2700 6300 2800
Wire Wire Line
	6300 2800 6750 2800
Wire Wire Line
	6750 2800 6750 2700
Text HLabel 6300 2900 3    50   Input ~ 0
GND
Wire Wire Line
	6300 2900 6300 2800
Connection ~ 6300 2800
Wire Wire Line
	6500 3700 7400 3700
Wire Wire Line
	6500 3000 7400 3000
Wire Wire Line
	6750 2300 7400 2300
$Comp
L Device:C C15
U 1 1 6256C707
P 8450 3700
F 0 "C15" V 8198 3700 50  0000 C CNN
F 1 "1.5p NPO" V 8289 3700 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 8488 3550 50  0001 C CNN
F 3 "~" H 8450 3700 50  0001 C CNN
	1    8450 3700
	0    1    1    0   
$EndComp
$Comp
L Device:C C16
U 1 1 6256CD0D
P 8700 3950
F 0 "C16" H 8815 3996 50  0000 L CNN
F 1 "1p NPO" H 8815 3905 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 8738 3800 50  0001 C CNN
F 3 "~" H 8700 3950 50  0001 C CNN
	1    8700 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	8200 3700 8300 3700
Wire Wire Line
	8600 3700 8700 3700
Wire Wire Line
	8700 3700 8700 3800
Text HLabel 8700 4200 3    50   Input ~ 0
GND
Wire Wire Line
	8700 4200 8700 4100
$Comp
L Connector:Conn_Coaxial J3
U 1 1 62570DEF
P 9700 3700
F 0 "J3" H 9800 3675 50  0000 L CNN
F 1 "Conn_Coaxial" H 9800 3584 50  0000 L CNN
F 2 "Connector_Coaxial:U.FL_Hirose_U.FL-R-SMT-1_Vertical" H 9700 3700 50  0001 C CNN
F 3 " ~" H 9700 3700 50  0001 C CNN
	1    9700 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	8700 3700 9500 3700
Connection ~ 8700 3700
Text HLabel 9700 4000 3    50   Input ~ 0
GND
Wire Wire Line
	9700 4000 9700 3900
Text HLabel 4600 3650 0    50   Output ~ 0
IRQ
Text HLabel 4600 3550 0    50   Input ~ 0
CE
Text HLabel 4600 3350 0    50   Input ~ 0
~CS
Text HLabel 4600 3250 0    50   Input ~ 0
SCK
Text HLabel 4600 3150 0    50   Output ~ 0
MISO
Text HLabel 4600 3050 0    50   Input ~ 0
MOSI
Wire Wire Line
	4600 3650 4700 3650
Wire Wire Line
	4600 3550 4700 3550
Wire Wire Line
	4600 3350 4700 3350
Wire Wire Line
	4600 3250 4700 3250
Wire Wire Line
	4600 3150 4700 3150
Wire Wire Line
	4600 3050 4700 3050
$EndSCHEMATC
