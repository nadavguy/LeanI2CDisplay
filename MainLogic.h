#include <avr/wdt.h>
#include <stdio.h>
#include <stdlib.h>

float RefVoltage = 5.0;
volatile unsigned long GeneralCounter = 0;
volatile unsigned long FiveSecCycle = 0;

bool IsFan1On = false;
bool IsFan2On = false;
bool IsPLTR1On = false;
bool IsPLTR2On = false;
bool IsFan1ManualMode = false;
bool IsFan2ManualMode = false;

float MaxTempChannel1 = 62.0;
float MinTempChannel1 = 58.0;

float V1In = 0;
uint16_t V1 = 0;
uint16_t V1Array[25] = {0};
uint16_t V1Sum = 0;
uint8_t V1Counter = 0;
uint16_t V1Average = 0;

// uint16_t Amps1 = 0;
// uint16_t Amps1Array[25] = {0};
// uint16_t Amps1Sum = 0;
// uint8_t Amps1Counter = 0;
// float Amps1Average = 0;

float V2In = 0;
uint16_t V2 = 0;
uint16_t V2Array[25] = {0};
uint16_t V2Sum = 0;
uint8_t V2Counter = 0;
float V2Average = 0;

// uint16_t Amps2 = 0;
// uint16_t Amps2Array[25] = {0};
// uint16_t Amps2Sum = 0;
// uint8_t Amps2Counter = 0;
// float Amps2Average = 0;

dht DHT1;
int DHT1chk = 0;
float RelativeHumidity1 = 0.0;

#define TempSns1Pin A0
#define TempSns2Pin A1
#define DHT11_1Pin A2
#define Pltr1Pin 2
#define Fan1Pin 3


float CalcArrayAverage(uint16_t Array[], uint16_t * Sum,uint8_t * Counter, uint16_t Measuremnt, float ArraySize);
void watchdogSetup(void)
{
  cli(); // disable all interrupts
  wdt_reset(); // reset the WDT timer
  /*
    WDTCSR configuration:
    WDIE = 1: Interrupt Enable
    WDE = 1 :Reset Enable
    WDP3 = 0 :For 2000ms Time-out
    WDP2 = 1 :For 2000ms Time-out
    WDP1 = 1 :For 2000ms Time-out
    WDP0 = 1 :For 2000ms Time-out
  */
  // Enter Watchdog Configuration mode:
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  // Set Watchdog settings:
  WDTCSR = (1 << WDIE) | (0 << WDE) | (0 << WDP3) | (1 << WDP2) | (0 << WDP1) | (0 << WDP0);
  sei();
}

ISR(WDT_vect) 
{
  //Serial.println(GeneralCounter);
  //Check Channel1 Voltage 
  V1 = analogRead(TempSns1Pin); // V1 = Vin * 1.5 / 11.5
  V1Average = CalcArrayAverage(V1Array, &V1Sum, &V1Counter, V1, 25.0);
  V1In = ( float(V1Average * RefVoltage / 1023.0) - 0.5 ) * 100;
  Serial.print("T1: ");
  Serial.println(V1In);
  
  //Check Channel2 Voltage
  // V2 = analogRead(A7); // V1 = Vin * 1.5 / 11.5
  // V2Average = CalcArrayAverage(V2Array, &V2Sum, &V2Counter, V2, 25.0);
  // V2In = V2Average * 1;
  // Serial.print("V2In: ");
  // Serial.println(V2In);


  //Check Battery Voltage 
  // VBat = analogRead(A0); // V1 = Vin * 1.5 / 11.5
  // VBatAverage = CalcArrayAverage(VBatArray, &VBatSum, &VBatCounter, VBat, 10.0);
  
  // Serial.print("VBat: ");
  // Serial.println(VBatAverage);
}


float CalcArrayAverage(uint16_t Array[], uint16_t * Sum,uint8_t * Counter, uint16_t Measuremnt, float ArraySize)
{
  // Serial.println("Calc Average: ");
  // Serial.print("Sum1: ");
  *Sum = *Sum - Array[*Counter];
  // Serial.println(*Sum);
  Array[*Counter] = Measuremnt;
  *Sum = *Sum + Measuremnt;
  // Serial.print("Sum2: ");
  // Serial.println(*Sum);
  *Counter=*Counter+1;
  // Serial.print("Counter: ");
  // Serial.println(*Counter);

  // Serial.print("Array0: ");
  // Serial.println(Array[0]);
  // Serial.print("Array1: ");
  // Serial.println(Array[1]);
  // Serial.print("Array2: ");
  // Serial.println(Array[2]);
  // Serial.print("Array3: ");
  // Serial.println(Array[3]);
  // Serial.print("Array4: ");
  // Serial.println(Array[4]);
  // Serial.print("Array5: ");
  // Serial.println(Array[5]);
  // Serial.print("Array6: ");
  // Serial.println(Array[6]);
  // Serial.print("Array7: ");
  // Serial.println(Array[7]);
  // Serial.print("Array8: ");
  // Serial.println(Array[8]);
  // Serial.print("Array9: ");
  // Serial.println(Array[9]);


  if (*Counter >= ArraySize)
  {
    *Counter = 0;
  }
  return *Sum / ArraySize;
}
