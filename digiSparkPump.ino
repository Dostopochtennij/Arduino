/*
	Digispark autopump for my plants. Devide by AlexGyver.
*/

#define PERIOD 54000  // TTL (54000 = 60 * 60 * 15 = 15 hours)
#define WORK 15       // time of work
#define MOS 1         // PIN

uint32_t mainTimer, myTimer;
boolean state = false;

#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#define adc_disable() (ADCSRA &= ~(1<<ADEN)) // disable ADC (before power-off)
#define adc_enable()  (ADCSRA |=  (1<<ADEN)) // re-enable ADC

void setup() {
  
  for (byte i = 0; i < 6; i++) {
    pinMode(i, INPUT);
  }
  adc_disable();          

  wdt_reset();            
  wdt_enable(WDTO_1S);    
  WDTCR |= _BV(WDIE);     
  sei();                  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}

void loop() {
  mainTimer++;

  if (!state) {                           
    if ((long)mainTimer - myTimer > PERIOD) {
      myTimer = mainTimer;                
      state = true;                       
      pinMode(MOS, OUTPUT);               
      digitalWrite(MOS, HIGH);            
    }
  } else {                                
    if ((long)mainTimer - myTimer > WORK) {
      myTimer = mainTimer;                
      state = false;                      
      digitalWrite(MOS, LOW);             
      pinMode(MOS, INPUT);                
    }
  }

  sleep_enable();   
  sleep_cpu();      
}

ISR (WDT_vect) {
  WDTCR |= _BV(WDIE); 
}
