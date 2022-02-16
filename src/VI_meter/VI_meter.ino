
//********************** HEADER ***********************
/*
  Name : MAX471_PVI_METER
  Author: Gavin Lyons
  Date: Dec 2021
  URL: https://github.com/gavinlyonsrepo/MAX471_PVI_METER
*/

//******************** LIBRARIES ******************
//https://github.com/gavinlyonsrepo/MAX471
#include "MAX471.h"

// https://github.com/gavinlyonsrepo/ERM19264_UC1609
// Fonts 1-3 must be enabled
#include "ERM19264_UC1609.h"

//******************** GLOBALS + DEFINES********************
// Max471
#define VT_PIN A0
#define AT_PIN A1
#define TEST_DELAY delay(2000);
// 1. VCC_BAT = calibrated battery voltage ATmega328p
MAX471 myMax471(ADC_10_bit , VCC_BAT, AT_PIN, VT_PIN);

// LCD
#define LCDcontrast 0x30 // 0x49 default, Lower contrast works better on Blue LCD 
#define MYLCDHEIGHT 64
#define MYLCDWIDTH  192
// GPIO 5-wire SPI interface HW SPI
#define CD 10 // GPIO pin number pick any you want 
#define RST 9 // "
#define CS 8  // "
// GPIO pin number SCK(UNO 13) , HW SPI , SCK
// GPIO pin number SDA(UNO 11) , HW SPI , MOSI

// define a buffer to cover one third screen , We divide the screen into 3 buffers, re-use buffer
uint8_t  screenBuffer[(MYLCDWIDTH * (MYLCDHEIGHT / 8)) / 3]; // 512 bytes

ERM19264_UC1609  mylcd(CD, RST, CS); // instantiate LCD object

// Serial, optionally comment in for serial
#define SERIAL_DEBUG_ON

// ******************* FUNCTION HEADERS *********
void Display_Buffer_Left(void);
void Display_Buffer_Middle(void);
void Display_Buffer_Right(float, float, float);
void Draw_Shapes(void);
void Legend_Font(uint8_t);
void Numbers_Font(int8_t);

// ******************** SETUP ************************
void setup() {

  // Serial
#ifdef SERIAL_DEBUG_ON
  Serial.begin(38400);
  Serial.println("== Comms Up==");
  Serial.println();
#endif

  // LCD
  mylcd.LCDbegin(LCDcontrast); // initialize the OLED
  mylcd.LCDFillScreen(0x00, 0); // Clear screen
  Display_Buffer_Left();
}

// ******************* MAIN LOOP *****************
void loop() {

  Display_Buffer_Middle();
  TEST_DELAY

}// End of main

// ********************* FUNCTIONS ************

// Function to display LHS side of screen (Screen is divided three parts/buffers)
// The LHS wil contain Text labels and will not change
void Display_Buffer_Left(void)
{

  // Declare a buffer struct and intialise.
  MultiBuffer left_side;
  // Intialise that struct (&struct, buffer, w, h, x_offset, y-offset)
  mylcd.LCDinitBufferStruct(&left_side, screenBuffer, MYLCDWIDTH / 3, MYLCDHEIGHT, 0, 0);

  // Set the Active buffer to Struct.
  mylcd.ActiveBuffer = &left_side;
  mylcd.LCDclearBuffer();

  // Draw a rectangle around buffer
  Draw_Shapes();

  //  Print Text legend for meter
  mylcd.setTextColor(FOREGROUND);
  mylcd.setFontNum(UC1609Font_Thick); // Thick Font
  mylcd.setCursor(5, 10);
  mylcd.print(F("VOLTAGE"));
  mylcd.setCursor(5, 30);
  mylcd.print(F("CURRENT"));
  mylcd.setCursor(5, 50);
  mylcd.print(F("POWER"));
  mylcd.LCDupdate();
}

// Function to display middle part of screen (Screen is divided three parts/buffers)
// The middle parts will contain measurements in Volts , Amps and Watts
void Display_Buffer_Middle(void)
{
  float voltageReading = 0.0;
  float currentReading = 0.0;
  float power = 0.0;

  // Read sensors
  voltageReading = myMax471.VoltageRead();
  currentReading = myMax471.CurrentRead();
  power = voltageReading * currentReading;

  // Declare a buffer struct and intialise.
  MultiBuffer middle_side;  // Declare a multi buffer struct for left side of screen
  // Intialise that struct (&struct, buffer, w, h, x_offset, y-offset)
  mylcd.LCDinitBufferStruct(&middle_side, screenBuffer, MYLCDWIDTH / 3, MYLCDHEIGHT, MYLCDWIDTH / 3, 0);
  
  // Set the Active buffer to Struct.
  mylcd.ActiveBuffer = &middle_side;
  mylcd.LCDclearBuffer();

  // Draw a rectangle around buffer
  Draw_Shapes();

  //  Print  for meter readings for Volts Amps and Watts
  Numbers_Font(4);
  mylcd.print(voltageReading, 2);
  Legend_Font(10);
  mylcd.print(F("V"));

  Numbers_Font(26);
  mylcd.print(currentReading, 2);
  Legend_Font(32);
  mylcd.print(F("A"));

  Numbers_Font(46);
  mylcd.print(power, 2);
  Legend_Font(52);
  mylcd.print(F("W"));

  mylcd.LCDupdate();

  Display_Buffer_Right(voltageReading, currentReading, power);

#ifdef SERIAL_DEBUG_ON
  Serial.print("Voltage Volts:  ");
  Serial.println(voltageReading, 4);
  Serial.print("Ampere Amps:  ");
  Serial.println(currentReading, 4);
  Serial.print("Power W:  ");
  Serial.println(power , 4);
  Serial.println();
#endif

}


// Function to display RHS of screen (Screen is divided three parts/buffers)
// The RHS will contain measurements in mV , mA and mW
void Display_Buffer_Right(float voltage, float current , float power)
{

  MultiBuffer right_side;
  mylcd.LCDinitBufferStruct(&right_side, screenBuffer, MYLCDWIDTH/3, MYLCDHEIGHT, (MYLCDWIDTH/3)*2, 0);

  mylcd.ActiveBuffer = &right_side;
  mylcd.LCDclearBuffer();

  // Draw a rectangle and lines around buffer
  Draw_Shapes();

  //  Print  for meter readings for mV mA and mW
  mylcd.setCursor(5, 10);
  mylcd.print(voltage * 1000, 0);
  mylcd.setCursor(44, 10);
  mylcd.print(F(" mV"));
 
  mylcd.setCursor(5, 30);
  mylcd.print(current * 1000, 1);
  mylcd.setCursor(44, 30);
  mylcd.print(F(" mA"));
  
  mylcd.setCursor(5, 50);
  mylcd.print(power * 1000, 0);
  mylcd.setCursor(44, 50);
  mylcd.print(F(" mW"));

  mylcd.LCDupdate();

}

void Draw_Shapes(void) {
  mylcd.drawRect(0, 0, 64, 64, FOREGROUND);
  mylcd.drawFastHLine(0, 21, 63, FOREGROUND);
  mylcd.drawFastHLine(0, 42, 63, FOREGROUND);
}

void Legend_Font(uint8_t y_offset)
{
  mylcd.setCursor(56, y_offset);
  mylcd.setFontNum(UC1609Font_Default );
  mylcd.setTextSize(1);
}

void Numbers_Font(int8_t y_offset)
{
  mylcd.setCursor(4, y_offset);
  mylcd.setFontNum(UC1609Font_Seven_Seg);
  mylcd.setTextSize(2);
}

//******************* EOF *****************
