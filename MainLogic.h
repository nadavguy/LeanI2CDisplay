#include <avr/wdt.h>

volatile unsigned long GeneralCounter = 0;
bool IsFan1On = false;
bool IsFan2On = false;
bool IsPLTR1On = false;
bool IsPLTR2On = false;

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
  WDTCSR = (1 << WDIE) | (0 << WDE) | (0 << WDP3) | (1 << WDP2) | (1 << WDP1) | (1 << WDP0);
  sei();
}

ISR(WDT_vect) 
{
    /*Manange PLTR, Fans, Read Temperature measurements */ 
    Serial.println(GeneralCounter);
//drawText(6,3,"float(11)");
/* ReEnable the watchdog interrupt, as this gets reset when entering this ISR and automatically enables the WDE signal that resets the MCU the next time the  timer overflows */
//WDTCSR = (1 << WDIE) | (1 << WDP2) | (1 << WDP1);

}

