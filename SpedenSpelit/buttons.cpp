#include "buttons.h"

//volatile int buttonNumber = -1;
volatile bool btnpress = false;
static unsigned long lastbouncetime = 0;
const unsigned long bouncedelay = 250;

unsigned long lastPressTime[5];  // Aikaa viimeisimmän napin painalluksen rekisteröimiseksi (5 nappia, pin 2-6)


extern byte currentLed;

void initButtonsAndButtonInterrupts(void)
{   // Aktivoi keskeytykset PCINT2 (painikkeet 2-6)
  PCICR |= (1 << PCIE2);      // PCIE2 mahdollistaa PCINT-sarjan keskeytykset
  PCMSK2 = 0b01111100;      // Vain pin 2-6 aktivoidaan (pin-maski)


  // See requirements for this function from buttons.h
  for(int napit = 2;napit<7;napit++)
  {
    pinMode(napit, INPUT_PULLUP);
  }
}
// Keskeytyksellinen napin rekisteröinti
ISR(PCINT2_vect) {
    byte pinState = PIND & 0b01111100; // Lue painikeportti (vain 2-6)

    for (int button = 2; button < 7; button++) {
        if (!(pinState & (1 << button))) { // Tarkista, onko nappi painettu alas
            unsigned long currentTime = millis();  // Hae nykyinen aika
            int buttonIndex = button - 2;  // Lasketaan nappi-indeksi (0-4)

            // Varmistetaan, että viivettä on kulunut tarpeeksi ennen uutta rekisteröintiä
            if (currentTime - lastPressTime[buttonIndex] >= bouncedelay) {
                buttonNumber = buttonIndex;  // Aseta rekisteröity nappi
                lastPressTime[buttonIndex] = currentTime;  // Tallenna tämän napin rekisteröintiaika
                break; 
            }
        }
    }
}