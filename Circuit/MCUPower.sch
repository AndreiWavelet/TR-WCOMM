EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 9
Title "WCOMM"
Date "2022-04-06"
Rev "1.0"
Comp "Andrei Alves Cardoso"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 3850 3000 0    50   Input ~ 0
3.3V
$Comp
L Device:C C?
U 1 1 624DBCC6
P 4000 3200
AR Path="/624C3E70/624DBCC6" Ref="C?"  Part="1" 
AR Path="/624C3E70/624D9402/624DBCC6" Ref="C1"  Part="1" 
F 0 "C1" H 4115 3246 50  0000 L CNN
F 1 "2u" H 4115 3155 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4038 3050 50  0001 C CNN
F 3 "~" H 4000 3200 50  0001 C CNN
	1    4000 3200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 624DBCCC
P 4400 3200
AR Path="/624C3E70/624DBCCC" Ref="C?"  Part="1" 
AR Path="/624C3E70/624D9402/624DBCCC" Ref="C2"  Part="1" 
F 0 "C2" H 4515 3246 50  0000 L CNN
F 1 "100n" H 4515 3155 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4438 3050 50  0001 C CNN
F 3 "~" H 4400 3200 50  0001 C CNN
	1    4400 3200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 624DBCD2
P 4800 3200
AR Path="/624C3E70/624DBCD2" Ref="C?"  Part="1" 
AR Path="/624C3E70/624D9402/624DBCD2" Ref="C3"  Part="1" 
F 0 "C3" H 4915 3246 50  0000 L CNN
F 1 "100n" H 4915 3155 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4838 3050 50  0001 C CNN
F 3 "~" H 4800 3200 50  0001 C CNN
	1    4800 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 3000 4000 3000
Wire Wire Line
	4000 3000 4000 3050
Wire Wire Line
	4000 3000 4400 3000
Wire Wire Line
	4400 3000 4400 3050
Connection ~ 4000 3000
Wire Wire Line
	4400 3000 4800 3000
Wire Wire Line
	4800 3000 4800 3050
Connection ~ 4400 3000
Wire Wire Line
	4000 3350 4000 3400
Wire Wire Line
	4000 3400 4400 3400
Wire Wire Line
	4800 3400 4800 3350
Wire Wire Line
	4400 3350 4400 3400
Connection ~ 4400 3400
Wire Wire Line
	4400 3400 4800 3400
Text HLabel 4400 3500 3    50   Input ~ 0
GND
Wire Wire Line
	4400 3500 4400 3400
Wire Wire Line
	6500 3400 6700 3400
$Comp
L Device:C C?
U 1 1 624DD54C
P 6500 3200
AR Path="/624C3E70/624DD54C" Ref="C?"  Part="1" 
AR Path="/624C3E70/624D9402/624DD54C" Ref="C4"  Part="1" 
F 0 "C4" H 6615 3246 50  0000 L CNN
F 1 "2u" H 6615 3155 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 6538 3050 50  0001 C CNN
F 3 "~" H 6500 3200 50  0001 C CNN
	1    6500 3200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 624DD552
P 6900 3200
AR Path="/624C3E70/624DD552" Ref="C?"  Part="1" 
AR Path="/624C3E70/624D9402/624DD552" Ref="C5"  Part="1" 
F 0 "C5" H 7015 3246 50  0000 L CNN
F 1 "100n" H 7015 3155 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6938 3050 50  0001 C CNN
F 3 "~" H 6900 3200 50  0001 C CNN
	1    6900 3200
	1    0    0    -1  
$EndComp
Text HLabel 6000 3000 0    50   Input ~ 0
3.3V
Wire Wire Line
	6900 3000 6900 3050
Wire Wire Line
	6900 3000 6700 3000
Wire Wire Line
	6500 3000 6500 3050
Wire Wire Line
	6500 3350 6500 3400
Wire Wire Line
	6900 3400 6900 3350
Text HLabel 6700 3500 3    50   Input ~ 0
GND
Wire Wire Line
	6700 3500 6700 3400
Connection ~ 6700 3400
Wire Wire Line
	6700 3400 6900 3400
Connection ~ 6500 3000
Text HLabel 4900 3000 2    50   Output ~ 0
MCU_VDD
Wire Wire Line
	4900 3000 4800 3000
Connection ~ 4800 3000
Text HLabel 7000 3000 2    50   Output ~ 0
MCU_VDDA
Wire Wire Line
	7000 3000 6900 3000
Connection ~ 6900 3000
$Comp
L power:PWR_FLAG #FLG01
U 1 1 624FC456
P 6700 2900
F 0 "#FLG01" H 6700 2975 50  0001 C CNN
F 1 "PWR_FLAG" H 6700 3073 50  0000 C CNN
F 2 "" H 6700 2900 50  0001 C CNN
F 3 "~" H 6700 2900 50  0001 C CNN
	1    6700 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 2900 6700 3000
Connection ~ 6700 3000
Wire Wire Line
	6700 3000 6500 3000
Wire Wire Line
	6400 3000 6500 3000
Wire Wire Line
	6000 3000 6100 3000
$Comp
L Device:L L?
U 1 1 624DD55E
P 6250 3000
AR Path="/624C3E70/624DD55E" Ref="L?"  Part="1" 
AR Path="/624C3E70/624D9402/624DD55E" Ref="L1"  Part="1" 
F 0 "L1" V 6440 3000 50  0000 C CNN
F 1 "10u" V 6349 3000 50  0000 C CNN
F 2 "Inductor_SMD:L_0805_2012Metric" H 6250 3000 50  0001 C CNN
F 3 "~" H 6250 3000 50  0001 C CNN
	1    6250 3000
	0    -1   -1   0   
$EndComp
Text Notes 4650 4150 0    50   ~ 0
Place decoupling capacitors near uC power pins
$EndSCHEMATC
