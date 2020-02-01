
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
float PreviousVoltageCh1 = 0;
float PreviousVoltageCh2 = 0;
float PreviousVoltageBat = 0;

float Ch1AmpsRefVoltage =507.0;




uint8_t Fan1Phase = 0; 
uint8_t Fan2Phase = 0; 

#define Pltr1Pin 1
#define Fan1Pin 2

void setup() {
  Serial.begin(9600);
  //pinMode(2,OUTPUT);
  pinMode(Pltr1Pin,OUTPUT);
  pinMode(Fan1Pin,OUTPUT);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A6,INPUT);
  pinMode(A7,INPUT);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forevert
  }
  analogReference(DEFAULT);
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
  drawText(0,1,"Ch1:");
  drawText(0,3,"Ch2:");
  digitalWrite(2, HIGH); 
  ClearPixels(Fan1IconXOffset, Fan1IconYOffset, Fan1IconWidth, Fan1IconHeight);
  drawbitmap(Fan1IconXOffset, Fan1IconYOffset, Fan1IconWidth, Fan1IconHeight, FANOff_bmp);
  CurrentTimeuSec = micros();
  TwentymSecCycle = micros();
}

void loop() {
  // put your main code here, to run repeatedly:
  // Show measured data on display 

  // Timer Overflow protection
  if ( micros() < TwentymSecCycle)
  {
    GeneralCounter++;
    TwentymSecCycle = 20000 - (4294967295 - TwentymSecCycle);
  }


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


  DisplayCh1();
  DisplayFan();
  // DisplayCh2();

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

void DisplayCh1()
{
  if ( (PreviousVoltageCh1 * 1.01 < V1In) || (PreviousVoltageCh1 * 0.99 > V1In) )
  {
    PreviousVoltageCh1 = V1In;
    ClearDigits(5, 1, 6);
    drawNumber(5, 1, V1In);
    drawText(10,1,"C");
    // ClearDigits(5, 2, 10);
    // drawNumber(5, 2, -1*float( ((Amps1Average - Ch1AmpsRefVoltage)*5.0/1023.0)/0.1 ));
    // drawText(10,2,"A");
  }
}

void DisplayCh2()
{
  if ((PreviousVoltageCh2 * 1.01 < float(V2In * RefVoltage / 1023.0)) || (PreviousVoltageCh2 * 0.99 > float(V2In * RefVoltage / 1023.0)))
  {
    PreviousVoltageCh2 = float(V2In * RefVoltage / 1023.0);
    ClearDigits(5, 3, 5);
    drawNumber(5, 3, float(V2In * RefVoltage / 1023.0));
    drawText(10,3,"V");
    ClearDigits(5, 4, 10);
    drawNumber(5, 4, -1*float( ((Amps2Average - Ch1AmpsRefVoltage)*RefVoltage/1023.0)/0.1 ));
    drawText(10,4,"A");
  }
}

void ManageChannel1Temp()
{
  if (V1In > 15.0) // After SteadState acheived
  {
    if (V1In < MinTempChannel1)
    {
      digitalWrite(Pltr1Pin, HIGH);
      IsPLTR1On = true;
    }
    else if (V1In > MaxTempChannel1)
    {
      digitalWrite(Pltr1Pin, LOW);
      IsPLTR1On = false;
    }
    if ( (V1In > 45.0) && (IsPLTR1On) && (!IsFan1ManualMode) )
    {
      digitalWrite(Fan1Pin, HIGH);
      IsFan1On = true;
    }
    else // TODO: Add fan PWM manual modes if needed
    {
      digitalWrite(Fan1Pin, LOW);
      IsFan1On = false;
    }
    
  }
}