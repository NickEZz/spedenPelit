#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"

// Use these 2 volatile variables for communicating between
// loop() function and interrupt handlers
volatile int buttonNumber = -1;           // for buttons interrupt handler
volatile bool newTimerInterrupt = false;  // for timer interrupt handler


volatile bool isGameRunning = false;
unsigned long previousMillis = 0; // Aikaseuranta
float kerroin = 1; 
unsigned long x = 0;
const unsigned long y = 700;

int randomNumbers[100];  // Satunnainen taulukko pelin arvotuille numeroille
int userNumbers[100];    // Käyttäjän syöttämille numeroille
int counter = 0;  // Laskuri, kuinka monta numeroa on arvottu
int numberCount = 0;  // Laskuri arvottujen numeroiden määrälle
int userCounter = 0; // Laskuri, käyttäjän painamille numeroille



unsigned long lastLedChange = 0;
const unsigned ledChangeTimer = 3000; // 3 sec
int score = 0;
byte currentLed = -1;
bool ledChanged = false;


void setup(){
 Serial.begin(9600);  // Aloitetaan sarjayhteys 9600 baudilla
  initializeTimer();
  initializeDisplay();
  initializeLeds();
  initButtonsAndButtonInterrupts();
  interrupts();
}

void loop()
{
  if (buttonNumber == 4){
      startTheGame();
     Serial.print("nappi 4");
      buttonNumber = -1;
    }
 if (buttonNumber >= 0) {
    if (buttonNumber == 4) {
        startTheGame();
    } else if (isGameRunning && userCounter < 100) {
        userNumbers[userCounter] = buttonNumber;
        Serial.print("Käyttäjän numero: ");
        Serial.println(userNumbers[userCounter]);
        userCounter++;
    }
    buttonNumber = -1; // Nollaa painikemuuttuja välittömästi
}

   // Jos timeri keskeyttää ja uusi numero on arvottu
  if (newTimerInterrupt) {
   
    newTimerInterrupt = false;  // Nollataan keskeytyslippu
    
    if (isGameRunning == true)
    {
         // Arvotaan satunnainen numero ja tallennetaan se taulukkoon
           do {
           randomNumbers[counter] = random(0, 4);  // Arvotaan numero 0-3
         } while (counter > 0 && randomNumbers[counter] == randomNumbers[counter - 1]);
        //   Serial.print("Arvottu numero: ");
         showResult(randomNumbers[counter]);
         setLed(randomNumbers[counter]);
         //  Serial.println(randomNumbers[counter]);
                                // Tulostetaan numero sarjamonitoriin
         
       }
     // Päivitetään laskuri ja tarkistetaan, onko 10 numeroa arvottu
      counter++;   
      numberCount ++;



    if (counter == 100){
      stopTheGame();
    }
     if (numberCount == 10) {
      // Kun 10 numeroa on arvottu, nollataan laskuri
      numberCount = 0;
     }

     if (numberCount % 10 == 0) {
   
      kerroin = kerroin * 0.9;  // Vähennetään aikaväliä 10 %

      // Päivitetään timerin väli (keskeytyksen aika)
       updateOCR1A();
      Serial.print("Nopeus kasvoi! Uusi aikaväli: ");
      Serial.println(kerroin);
     }
      if ( userNumbers !=randomNumbers  )
      {
      //  stopTheGame();
      }
    }

    if(newTimerInterrupt == true)
   {
    //  new random number must be generated
     // and corresponding let must be activated
   }

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
    // if (isGameRunning) {
   //     newTimerInterrupt = true;
      
    //}
   
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
   isGameRunning = true;  
   kerroin = 1; // Nollataan kerroin
   counter = 0; // Nollataan laskuri
   userCounter = 0; // Nollataan käyttäjän laskuri
    for (int i = 0; i < 100; i++) {
        userNumbers[i] = -1; // Nollataan käyttäjän numerot
    }
  Serial.println("Peli aloitettu");
   // see requirements for the function from SpedenSpelit.h
}
void stopTheGame()
{
   
    Serial.println("Peli pysäytetty");
    counter = 0;
    numberCount = 0;
    kerroin = 1; // Palautetaan alkuperäinen nopeus
    userCounter = 0; // Nollataan käyttäjän laskuri
    updateOCR1A(); // Päivitetään keskeytys
    clearAllLeds();
    isGameRunning = false;
   // see requirements for the function from SpedenSpelit.h
}
