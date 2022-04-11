EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 7 9
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
L Connector:Micro_SD_Card_Det J4
U 1 1 624FA7CB
P 5950 3450
F 0 "J4" H 5900 4267 50  0000 C CNN
F 1 "Micro_SD_Card_Det" H 5900 4176 50  0000 C CNN
F 2 "Connector_Card:microSD_HC_Hirose_DM3D-SF" H 8000 4150 50  0001 C CNN
F 3 "https://www.hirose.com/product/en/download_file/key_name/DM3/category/Catalog/doc_file_id/49662/?file_category_id=4&item_id=195&is_series=1" H 5950 3550 50  0001 C CNN
	1    5950 3450
	1    0    0    -1  
$EndComp
$Sheet
S 5650 1250 700  400 
U 625068E2
F0 "SDCardPower" 50
F1 "SDCardPower.sch" 50
F2 "SD_3.3V" O R 6350 1350 50 
F3 "3.3V" I L 5650 1350 50 
F4 "GND" I L 5650 1450 50 
F5 "~EN" I L 5650 1550 50 
$EndSheet
Text HLabel 5000 1350 0    50   Input ~ 0
3.3V
Text HLabel 5000 1450 0    50   Input ~ 0
GND
Text HLabel 5000 1550 0    50   Input ~ 0
~EN
Wire Wire Line
	5000 1550 5650 1550
Wire Wire Line
	5000 1450 5650 1450
Wire Wire Line
	5000 1350 5650 1350
Text Label 6950 1350 0    50   ~ 0
SD_3.3V
Wire Wire Line
	6950 1350 6350 1350
Text Label 4850 3350 2    50   ~ 0
SD_3.3V
Text HLabel 4850 3550 0    50   Input ~ 0
GND
Wire Wire Line
	4850 3550 5050 3550
Text HLabel 4850 3650 0    50   Output ~ 0
DO
Wire Wire Line
	4850 3650 5050 3650
Text HLabel 4850 3450 0    50   Input ~ 0
SCLK
Text HLabel 4850 3250 0    50   Input ~ 0
DI
Wire Wire Line
	4850 3450 5050 3450
Wire Wire Line
	5050 3250 4850 3250
Text HLabel 4850 3150 0    50   Input ~ 0
CS
Wire Wire Line
	4850 3150 5050 3150
Text HLabel 6850 3950 2    50   Input ~ 0
GND
Wire Wire Line
	6850 3950 6750 3950
$Comp
L Device:R R12
U 1 1 62511ABF
P 4850 4350
F 0 "R12" H 4920 4396 50  0000 L CNN
F 1 "10k" H 4920 4305 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4780 4350 50  0001 C CNN
F 3 "~" H 4850 4350 50  0001 C CNN
	1    4850 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 4200 4850 3950
Text HLabel 4850 4600 3    50   Input ~ 0
GND
Wire Wire Line
	4850 4600 4850 4500
Text Label 4850 3850 2    50   ~ 0
SD_3.3V
Wire Wire Line
	4850 3950 5050 3950
Text HLabel 4750 3950 0    50   Output ~ 0
SD_Det
Wire Wire Line
	4750 3950 4850 3950
Connection ~ 4850 3950
Wire Wire Line
	4850 3850 5050 3850
Text Label 3800 3750 2    50   ~ 0
SD_3.3V
$Comp
L Device:R R11
U 1 1 624FF92C
P 4050 3750
F 0 "R11" V 4257 3750 50  0000 C CNN
F 1 "10k" V 4166 3750 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3980 3750 50  0001 C CNN
F 3 "~" H 4050 3750 50  0001 C CNN
	1    4050 3750
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3800 3750 3900 3750
Wire Wire Line
	5050 3750 4200 3750
Wire Wire Line
	4850 3350 5050 3350
Text Label 3800 3050 2    50   ~ 0
SD_3.3V
$Comp
L Device:R R10
U 1 1 62501F64
P 4050 3050
F 0 "R10" V 4257 3050 50  0000 C CNN
F 1 "10k" V 4166 3050 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3980 3050 50  0001 C CNN
F 3 "~" H 4050 3050 50  0001 C CNN
	1    4050 3050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3800 3050 3900 3050
Wire Wire Line
	5050 3050 4200 3050
$EndSCHEMATC
