#include "leds.h"

// Pinit jokaiselle LED
const int ledPins[] = {A2, A3, A4, A5};
// button = 2 dp2, 3 = dp3, 4 = dp4, 5 = dp5, 6 = dp6 (startGame)

void initializeLeds()
{
// see requirements for this function from leds.h
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], HIGH); // Aloitus kaikilla pineillä HIGH
    delay(250);
    clearAllLeds(); // Set them to LOW
  }
}

void setLed(byte ledNumber)
{
// see requirements for this function from leds.h
  clearAllLeds();
  if (ledNumber < 4) {
    // Turn on only the specific LED
    digitalWrite(ledPins[ledNumber], HIGH);
    currentLed = ledNumber;
  }
}


void clearAllLeds()
{
// see requirements for this function from leds.h
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], LOW);
  }
  currentLed = -1;
}

void setAllLeds()
{
// see requirements for this function from leds.h
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], HIGH);
  }
}


void show1()
{
// see requirements for this function from leds.h
  for (int num = 0; num < 16; num++) {
      for (int i = 0; i < 4; i++) {
        // Set LED according to binary of num
        digitalWrite(ledPins[i], (num & (1 << i)) ? HIGH : LOW);
      }
      delay(250); // Small delay to make binary number recognizable
  }
}

void show2(int rounds)
{
// see requirements for this function from leds.h
  int delayTime = 500;
  for (int r = 0; r < rounds; r++) {
      for (int i = 0; i < 4; i++) {
        digitalWrite(ledPins[i], HIGH);
        delay(delayTime);
        // Reduce delay to speed up, minimum delay 50 ms
        delayTime = max(50, delayTime - 50);
      }
      // Tyhjennä ledit ennen seuraavaa erää
      clearAllLeds();
      delay(200);
  }
}