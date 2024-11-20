#include "buttons.h"

//volatile int buttonNumber = -1;
volatile bool btnpress = false;
static unsigned long lastbouncetime = 0;
const unsigned long bouncedelay = 100;

extern byte currentLed;

void initButtonsAndButtonInterrupts(void)
{
  PCICR |= (1 << PCIE2); 
  PCMSK2 = 0b01111100;
  // See requirements for this function from buttons.h
  for(int napit = 2;napit<7;napit++)
  {
    pinMode(napit, INPUT_PULLUP);
  }
}

ISR(PCINT2_vect) {
  static unsigned long lastInterruptTime = 0;
  unsigned long currentTime = millis();
  if (currentTime - lastInterruptTime > 100) {  // Debounce
    for (int button = 2; button < 7; button++) {
      if (digitalRead(button) == LOW) {
        buttonNumber = button - 2;
        break;
      }
    }
  }
  lastInterruptTime = currentTime;
}