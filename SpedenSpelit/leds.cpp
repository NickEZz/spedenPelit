#include "leds.h"

// Pinit jokaiselle LED
const int ledPins[] = {A2, A3, A4, A5};

void initializeLeds()
{
// see requirements for this function from leds.h
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW); // Aloitus kaikilla pineillä LOW
  }
}

void setLed(byte ledNumber)
{
// see requirements for this function from leds.h
  clearAllLeds();
  if (ledNumber < 4) {
    // Turn on only the specific LED
    digitalWrite(ledPins[ledNumber], HIGH);
    Serial.print("LED ");
    Serial.print(ledNumber);
    Serial.print(" on päällä, pinni: ");
    Serial.println(ledPins[ledNumber]);
  } else {
    Serial.println("Virheellinen LED-numero. Käytä arvoa 0-3.");
  }
}


void clearAllLeds()
{
// see requirements for this function from leds.h
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], LOW);
  }
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