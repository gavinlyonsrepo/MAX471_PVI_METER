

Table of contents
---------------------------

  * [Overview](#overview)
  * [Libraries](#libraries)
  * [Output](#output)
  * [Schematic](#schematic)
  
Overview
--------------------
* Name : MAX471_VI_Meter
* Title : Arduino Project , Voltage and current meter based on MAX471 sensor 
* Description : 

1. An Arduino Nano uses a MAX471 current and voltage sensor to measure a Load
and displays output to an ERM19264  UC1609 LCD
2. Current reading  range  0-3 A 
3. Voltage reading  range 3-25 V
4. ATmega328 battery internal reference voltage used for analog measurements
5. Output to LCD and  serial monitor

* Author: Gavin Lyons
* Arduino IDE: 1.8.16
* Copyright: GNU GPL v3

Libraries
------------------------

Libraries used , both written by Author and on github.

1. [MAX471](https://github.com/gavinlyonsrepo/MAX471)
2. [ERM19264_UC1609]( https://github.com/gavinlyonsrepo/ERM19264_UC1609)

Notes:
When using the LCD library fonts 1-3 must be enabled in font file and multi-buffer mode
selected. re MAX471 library, If deciding to use a different IC not based on ATmega328p  the Vref would have to changed from VCC_BAT in constructor.


Output
--------------------------

![o](https://github.com/gavinlyonsrepo/MAX471_PVI_METER/blob/main/extras/image/output.jpg)

Schematic
-------------------------------

![s](https://github.com/gavinlyonsrepo/MAX471_PVI_METER/blob/main/extras/image/sch.png)
