#include "leds.h"

// LEDien liitäntäpinnit
const int ledPins[] = {A2, A3, A4, A5}; // LEDien pinniarvot taulukossa

// Alustaa kaikki LEDit
void initializeLeds()
{
  for (int i = 0; i < 4; i++) { // Käy läpi kaikki LEDien pinnit
    pinMode(ledPins[i], OUTPUT); // Määritä pinni ulostuloksi
    digitalWrite(ledPins[i], HIGH); // Aseta LED tilaan HIGH (syttyy hetkeksi)
    delay(250); // Viive näkyvyyden parantamiseksi
    clearAllLeds();
  }
}

// Sytyttää tietyn LEDin
void setLed(byte ledNumber)
{
  if (ledNumber < 4) { // Varmista, että LED-numero on validi
    clearAllLeds();
    digitalWrite(ledPins[ledNumber], HIGH); // Sytytä haluttu LED
    currentLed = ledNumber; // Päivitä aktiivinen LED
  }
}

// Sammuttaa kaikki LEDit
void clearAllLeds()
{
  for (int i = 0; i < 4; i++) { // Käy läpi kaikki LEDien pinnit
    digitalWrite(ledPins[i], LOW); // Sammuta jokainen LED
  }
  currentLed = -1; // Ei aktiivista LEDiä
}

// Sytyttää kaikki LEDit samanaikaisesti
void setAllLeds()
{
  for (int i = 0; i < 4; i++) { // Käy läpi kaikki LEDien pinnit
    digitalWrite(ledPins[i], HIGH); // Sytytä jokainen LED
  }
}

// Näyttää LEDeillä binäärilukuja (0–15)
void show1()
{
  for (int num = 0; num < 16; num++) { // Binäärinumerot 0–15
      for (int i = 0; i < 4; i++) { // Jokainen LED edustaa yhtä bittiä
        digitalWrite(ledPins[i], (num & (1 << i)) ? HIGH : LOW); // Aseta bittiä vastaava tila
      }
      delay(250); // Viive, jotta numero näkyy selvästi
  }
}

// Näyttää "pyörivän" efektin LEDeillä usean kierroksen ajan
void show2(int rounds)
{
  int delayTime = 500; // Alkuviive
  for (int r = 0; r < rounds; r++) { // Kierrokset
      for (int i = 0; i < 4; i++) { // Sytytä LEDit yksi kerrallaan
        digitalWrite(ledPins[i], HIGH); // Sytytä yksi LED
        delay(delayTime); // Viive ennen seuraavaa LEDiä
        delayTime = max(50, delayTime - 50); // Pienennä viivettä, mutta älä alle 50 ms
      }
      clearAllLeds();
      delay(200); // Lyhyt tauko ennen seuraavaa kierrosta
  }
}