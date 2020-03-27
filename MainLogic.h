#include <avr/wdt.h>
#include <stdio.h>
#include <stdlib.h>

volatile unsigned long GeneralCounter = 0;

uint16_t V2 = 0;
uint16_t V2Array[25] = {0};
uint16_t V2Sum = 0;
uint8_t V2Counter = 0;
float V2Average = 0;

uint16_t Amps2 = 0;
uint16_t Amps2Array[25] = {0};
uint16_t Amps2Sum = 0;
uint8_t Amps2Counter = 0;
float Amps2Average = 0;

float V2In = 0;

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
  //Check Channel2 Voltage & Amper
  V2 = analogRead(A7); // V1 = Vin * 1.5 / 11.5
  V2Average = CalcArrayAverage(V2Array, &V2Sum, &V2Counter, V2, 25.0);
  V2In = V2Average * (21.93+2.977) / 2.977;
  // Serial.print("V2In: ");
  // Serial.println(V2In);
  Amps2 = analogRead(A6); 
  Serial.print("Amps2 Raw: ");
  Serial.println(Amps2);
  Amps2Average = CalcArrayAverage(Amps2Array, &Amps2Sum, &Amps2Counter, Amps2, 25.0);
  // Serial.print("Amps2Average: ");
  // Serial.println(Amps2Average);
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
