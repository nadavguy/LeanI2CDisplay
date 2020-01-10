
#include <SPI.h>
#include <Wire.h>
#include "DisplaySupport.h"
#include "PowerImages.h"


// #define SCREEN_WIDTH 128 // OLED display width, in pixels
// #define SCREEN_HEIGHT 32 // OLED display height, in pixels

// // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// #define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
unsigned long CurrentTimeuSec = 0;
unsigned long FanDisplayCycle = 0;
uint8_t FanPhase = 0; 
void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forevert
  }
  ClearScreen();
  drawbitmap(BatteryIconXOffset ,BatteryIconYOffset , BatteryIconWidth, BatteryIconHeight, Battery0Percent_bmp);
  drawbitmap(MosfetIconXOffset ,MosfetIconYOffset , MosfetIconWidth, MosfetIconHeight, MOSFETOn_bmp);
  drawbitmap(MosfetIconXOffset-20 ,MosfetIconYOffset , MosfetIconWidth, MosfetIconHeight, MOSFETOff_bmp);
  drawText(0,1,"Empty Cell");
  drawText(0,2,"Full Cell");
  drawText(0,3,"Third Line");
  drawText(0,4,"Fourth Line");
  drawNumber(80,3,float(10));
  ClearDigits(1,1,4);
  // display.setTextColor(SSD1306_BLACK);
  //  display.setCursor(0, 0);
	// 	//display.write(' ');
	// 	display.println("E");
  //   display.display();
  CurrentTimeuSec = micros();
}

void loop() {
  // put your main code here, to run repeatedly:
  if ( (micros() - FanDisplayCycle > 500000) && (FanPhase == 0) )
  {
    ClearPixels(MosfetIconXOffset ,MosfetIconYOffset , MosfetIconWidth, MosfetIconHeight);
    drawbitmap(MosfetIconXOffset ,MosfetIconYOffset , MosfetIconWidth, MosfetIconHeight, FANOn1_bmp);
    FanPhase = 1;
    FanDisplayCycle = micros();
  }

  if ( (micros() - FanDisplayCycle > 500000) && (FanPhase == 1) )
  {
    ClearPixels(MosfetIconXOffset ,MosfetIconYOffset , MosfetIconWidth, MosfetIconHeight);
    drawbitmap(MosfetIconXOffset ,MosfetIconYOffset , MosfetIconWidth, MosfetIconHeight, FANOn2_bmp);
    FanPhase = 0;
    FanDisplayCycle = micros();
  }
  // delay(500);
  // ClearPixels(MosfetIconXOffset ,MosfetIconYOffset , MosfetIconWidth, MosfetIconHeight);
  // drawbitmap(MosfetIconXOffset ,MosfetIconYOffset , MosfetIconWidth, MosfetIconHeight, FANOn2_bmp);
  // delay(500);
}
