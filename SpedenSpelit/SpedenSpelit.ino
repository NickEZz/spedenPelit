#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"

// Use these 2 volatile variables for communicating between
// loop() function and interrupt handlers
volatile int buttonNumber = -1;           // for buttons interrupt handler
volatile bool newTimerInterrupt = false;  // for timer interrupt handler

unsigned long previousMillis = 0; // Aikaseuranta
float kerroin = 1; 


int randomNumbers[100];  // Satunnainen taulukko pelin arvotuille numeroille
int userNumbers[100];    // Käyttäjän syöttämille numeroille

int counter = 0;  // Laskuri, kuinka monta numeroa on arvottu
int numberCount = 0;  // Laskuri arvottujen numeroiden määrälle

void setup(){
 Serial.begin(9600);  // Aloitetaan sarjayhteys 9600 baudilla
  initializeTimer();
 
}

void loop()
{
   // Jos timeri keskeyttää ja uusi numero on arvottu
  if (newTimerInterrupt) {
   
    newTimerInterrupt = false;  // Nollataan keskeytyslippu
    
    // Arvotaan satunnainen numero ja tallennetaan se taulukkoon
   randomNumbers[counter] = random(0, 4);  // Arvotaan numero 0-3
    Serial.print("Arvottu numero: ");
    Serial.println(randomNumbers[counter]);  // Tulostetaan numero sarjamonitoriin
  

    
  // Päivitetään laskuri ja tarkistetaan, onko 10 numeroa arvottu
    counter++;
    numberCount++;

    if (counter == 10) {
      // Kun 10 numeroa on arvottu, nollataan laskuri
      counter = 0;
    }

     if (numberCount % 10 == 0) {
      // Nopeutetaan timeriä 10 % (mutta ei liian nopeaksi)
      kerroin = kerroin * 0.9;  // Vähennetään aikaväliä 10 %

      // Päivitetään timerin väli (keskeytyksen aika)
       updateOCR1A();
      Serial.print("Nopeus kasvoi! Uusi aikaväli: ");
      Serial.println(kerroin);
    }
  }


//  if(buttonNumber>=0)
 // {
     // start the game if buttonNumber == 4
     // check the game if 0<=buttonNumber<4
 // }

//  if(newTimerInterrupt == true)
 // {
     // new random number must be generated
     // and corresponding let must be activated
  // }

}

void initializeTimer(void){
  TCCR1A = 0;
  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);  // CTC-tila, prescaler 1024
  updateOCR1A();
  TIMSK1 = (1 << OCIE1A);  // Oikea keskeytys asetettu
  sei();  // Ota keskeytykset käyttöön
 
}
ISR(TIMER1_COMPA_vect)
{
   newTimerInterrupt = true; 
}

void updateOCR1A() {
  
  OCR1A = (16000000 / 1024)*kerroin - 1;

}
void checkGame(byte nbrOfButtonPush)
{
	// see requirements for the function from SpedenSpelit.h
 
}


void initializeGame()
{
	// see requirements for the function from SpedenSpelit.h
}

void startTheGame()
{
   // see requirements for the function from SpedenSpelit.h
}

