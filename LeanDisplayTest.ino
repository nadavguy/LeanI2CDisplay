
#include <SPI.h>
#include <Wire.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "MainLogic.h"
#include "DisplaySupport.h"
#include "PowerImages.h"


// #define SCREEN_WIDTH 128 // OLED display width, in pixels
// #define SCREEN_HEIGHT 32 // OLED display height, in pixels

// // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// #define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
unsigned long CurrentTimeuSec = 0;
unsigned long Fan1DisplayCycle = 0;
unsigned long Fan2DisplayCycle = 0;
unsigned long TwentymSecCycle = 0;


uint8_t Fan1Phase = 0; 
uint8_t Fan2Phase = 0; 
void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forevert
  }
  
  watchdogSetup();

  //Test SCreen Will Be removed
  ClearScreen();
  // drawbitmap(BatteryIconXOffset ,BatteryIconYOffset , BatteryIconWidth, BatteryIconHeight, Battery0Percent_bmp);
  // drawbitmap(MosfetIconXOffset ,MosfetIconYOffset , MosfetIconWidth, MosfetIconHeight, MOSFETOn_bmp);
  // drawbitmap(MosfetIconXOffset-20 ,MosfetIconYOffset , MosfetIconWidth, MosfetIconHeight, MOSFETOff_bmp);
  // drawText(0,1,"Empty Cell");
  // drawText(0,2,"Full Cell");
  // drawText(0,3,"Third Line");
  // drawText(0,4,"Fourth Line");
  // drawNumber(13,3,float(10));
  // ClearDigits(1,1,4);
  // display.setTextColor(SSD1306_BLACK);
  //  display.setCursor(0, 0);
	// 	//display.write(' ');
	// 	display.println("E");
  //   display.display();
  ClearScreen();
  ClearPixels(Fan1IconXOffset, Fan1IconYOffset, Fan1IconWidth, Fan1IconHeight);
  drawbitmap(Fan1IconXOffset, Fan1IconYOffset, Fan1IconWidth, Fan1IconHeight, FANOff_bmp);
  CurrentTimeuSec = micros();
  TwentymSecCycle = micros();
}

void loop() {
  // put your main code here, to run repeatedly:
  // Show measured data on display 

  // Update value of TwentymSecCycle every 20mSec for PWM
  if ( micros() - TwentymSecCycle > 20000)
  {
    GeneralCounter++;
    TwentymSecCycle = micros();
  }

  if ( micros()>= 4294967295 - (unsigned long) 50 *20000  )
  {
    /* Close PLTR, Set PWM to 1.5mSec */
  }

  if ( micros() < TwentymSecCycle)
  {
    GeneralCounter++;
    TwentymSecCycle = 20000 - (4294967295 - TwentymSecCycle);
  }


  DisplayFan();

  // delay(500);
  // ClearPixels(MosfetIconXOffset ,MosfetIconYOffset , MosfetIconWidth, MosfetIconHeight);
  // drawbitmap(MosfetIconXOffset ,MosfetIconYOffset , MosfetIconWidth, MosfetIconHeight, FANOn2_bmp);
  // delay(500);
}

void DisplayFan()
{
  if (IsFan1On)
  {
    if ((micros() - Fan1DisplayCycle > 500000) && (Fan1Phase == 0))
    {
      ClearPixels(Fan1IconXOffset, Fan1IconYOffset, Fan1IconWidth, Fan1IconHeight);
      drawbitmap(Fan1IconXOffset, Fan1IconYOffset, Fan1IconWidth, Fan1IconHeight, FANOn1_bmp);
      Fan1Phase = 1;
      Fan1DisplayCycle = micros();
      // ClearDigits(0, 1, 4);
      // drawNumber(0, 1, float(FanPhase));
    }

    if ((micros() - Fan1DisplayCycle > 500000) && (Fan1Phase == 1))
    {
      ClearPixels(Fan1IconXOffset, Fan1IconYOffset, Fan1IconWidth, Fan1IconHeight);
      drawbitmap(Fan1IconXOffset, Fan1IconYOffset, Fan1IconWidth, Fan1IconHeight, FANOn2_bmp);
      Fan1Phase = 0;
      Fan1DisplayCycle = micros();
      // ClearDigits(0, 1, 4);
      // drawNumber(0, 1, float(FanPhase));
    }
  }
  else 
  {
    if ((micros() - Fan1DisplayCycle > 5000000) )
    {
      ClearPixels(Fan1IconXOffset, Fan1IconYOffset, Fan1IconWidth, Fan1IconHeight);
      drawbitmap(Fan1IconXOffset, Fan1IconYOffset, Fan1IconWidth, Fan1IconHeight, FANOff_bmp);
      Fan1DisplayCycle = micros();
    }
  }
}