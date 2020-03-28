#include <SPI.h>
#include <Wire.h>
#include <avr/io.h>
// #include <avr/sleep.h>
#include <avr/interrupt.h>
#include <FastLED.h>
#include "MainLogic.h"
// #include "DisplaySupport.h"
// #include "PowerImages.h"

#define Charge 3
#define Relay 4
#define LED1_PIN 5
#define LED2_PIN 6
#define NUM_LEDS 1

// #define SCREEN_WIDTH 128 // OLED display width, in pixels
// #define SCREEN_HEIGHT 32 // OLED display height, in pixels

// // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// #define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
unsigned long CurrentTimeuSec = 0;
unsigned long Fan1DisplayCycle = 0;
unsigned long Fan2DisplayCycle = 0;
unsigned long TwentymSecCycle = 0;

float MeasuredAmps = 0;
float MeasuredVoltage = 0;
float PreviousVoltageCh2 = 0;

float RefVoltage = 4.745;
float Ch1AmpsRefVoltage =507.0;

//bool 

CRGB leds[NUM_LEDS];

// u8 PowerButtonState = 1;
// u8 PowerButtonCounter = 0;

void setup() {
  Serial.begin(9600);
  // pinMode(2,OUTPUT); //OnBoard LED
  pinMode(A0,INPUT); // Battery Voltage
  pinMode(A1,INPUT); // Voltage Channel 1
  pinMode(A2,INPUT); // Ampere Channel 1
  pinMode(A6,INPUT); // Ampere Cahnnel 2
  pinMode(A7,INPUT); // Voltalge Channel 2
  pinMode(Charge,INPUT_PULLUP); // Charge Button
  pinMode(Relay,OUTPUT); //Relay

  FastLED.addLeds<WS2812, LED1_PIN, GRB>(leds, NUM_LEDS);

  analogReference(DEFAULT);
  watchdogSetup();
  //detachInterrupt(digitalPinToInterrupt(PowerPin));

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

  DisplayCh2();
}

void DisplayCh2()
{
  if ((PreviousVoltageCh2 * 1.01 < float(V2In * RefVoltage / 1023.0)) || (PreviousVoltageCh2 * 0.99 > float(V2In * RefVoltage / 1023.0)))
  {
    PreviousVoltageCh2 = float(V2In * RefVoltage / 1023.0);
    MeasuredVoltage = PreviousVoltageCh2;
    ShowLEDColorAccordingToVA(MeasuredVoltage, Amps2Average);
    Serial.print("V2: ");
    Serial.println(float(V2In * RefVoltage / 1023.0));
    MeasuredAmps = -1*float( ((Amps2Average - Ch1AmpsRefVoltage)*RefVoltage/1023.0)/0.1 );
    Serial.print("Amps2: ");
    Serial.println(Amps2Average);
  }

}

void ShowLEDColorAccordingToVA(float Volt,float Amp)
{
  // 10V or lower Red
  // 10V to 12V Red to Orange
  // 12V to 13.0V Orange To Yellow
  // 13.0V to 14V Yellow To Chartreuse
  // 14V to 14.4 Chartreuse To Green

  if (Volt<10)
  {
    Volt = 10;
  }
  if ( (Volt >= 10) && (Volt < 12) ) // Red To Orange
  {
    // TempCalc = ((88.125 - Range)/73.125);
    leds[0] = CRGB(255, (128* (Volt - 10 )/(12.0 - 10.0)) , 0);
    FastLED.show();
    // Serial.print("Color Value: ");
    // Serial.println((128* (Range - 15 )/73.125));
  }
  else if ( (Volt >= 12) && (Volt < 13) ) // Orange To Yellow
  {
    leds[0] = CRGB(255, (127*(Volt - 12.0)/(13.0 - 12.0)) + 128 , 0);
    FastLED.show();
    // Serial.print("Color Value: ");
    // Serial.println((255*(Range - 88.125)/73.125));
  }
  else if ( (Volt >= 13) && (Volt < 14) ) // Yellow To Chartreuse
  {
    leds[0] = CRGB((127*(Volt - 13)/(14.0 -13.0)) + 128, 255 , 0);
    FastLED.show();
  }
  else if ( (Volt >= 14) && (Volt < 14.4) ) // Chartreuse To Green
  {
    leds[0] = CRGB((128*( Volt - 14)/(14.4 - 14)), 255 , 0);
    FastLED.show();
  }
  else if ( (Volt >= 14.4) && (Amp >= 2.0) ) // SpringGreen
  {
    leds[0] = CRGB( 0, 255 , 128);
    FastLED.show();
  }
  else if ( (Volt >= 14.4) && (Amp >= 1.0) && (Amp < 2.0) ) // Cyan
  {
    leds[0] = CRGB( 0, 255 , 255);
    FastLED.show();
  }
  else if ( (Volt >= 14.4) && (Amp >= 0.5) && (Amp < 1.0)) // Azure
  {
    leds[0] = CRGB( 0, 128, 255);
    FastLED.show();
  }
  else if ( (Volt >= 14.4) && (Amp < 0.5) ) // Blue
  {
    leds[0] = CRGB( 0, 0 , 255);
    FastLED.show();
  }
}