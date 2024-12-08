#include <EEPROM.h> //Uno, Mega ja Nano, on sisäänrakennettu EEPROM
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
byte score = 0;
byte highScore = 0;
byte currentLed = -1;
bool ledChanged = false;


void setup(){
  delay(500); //vakaatuminen
  Serial.begin(9600);  // Aloitetaan sarjayhteys 9600 baudilla
  initializeTimer();
  initializeDisplay();


  isGameRunning = false;
  initializeLeds();
  initButtonsAndButtonInterrupts(); // suorittaa nappien inilizaation
  interrupts();

  // EEPROM
  highScore = loadHighScore();
  Serial.println(highScore);
}

void loop()
{
  
  if(!isGameRunning){
      
 //   Serial.println("korkein score on:");
//    Serial.println(highScore);
    showResult(highScore);
  
  }

 if (buttonNumber >= 0) {// jos buttonNumber on yli 0 suoritetaan if lause
    if (buttonNumber == 4) {//jos ButtonNumber on 4 eli painetaan nappia 6 niin aloitetaan peli
        startTheGame();
    } else if (isGameRunning && userCounter < 100) {
        userNumbers[userCounter] = buttonNumber;//tallentaa napin painalluksen taulukkoon
        Serial.print("Käyttäjän numero: ");
        Serial.println(userNumbers[userCounter]);
        userCounter++;//menee taulukossa yhden eteenpäin
        checkGame(userNumbers[userCounter - 1]);
    }
    buttonNumber = -1; // Nollaa painikemuuttuja välittömästi
}
   // Jos timeri keskeyttää ja uusi numero on arvottu
  if (newTimerInterrupt) {
   
    newTimerInterrupt = false;  // Nollataan keskeytyslippu
    
    if (isGameRunning == true) //katsotaa onko peli päällä
    {
         // Arvotaan satunnainen numero ja tallennetaan se taulukkoon
           do {
           randomNumbers[counter] = random(0, 4);  // Arvotaan numero 0-3
         } while (counter > 0 && randomNumbers[counter] == randomNumbers[counter - 1]);
         
         setLed(randomNumbers[counter]);
         Serial.println(randomNumbers[counter]);
      numberCount ++;
       }
     // Päivitetään laskuri ja tarkistetaan, onko 10 numeroa arvottu
      counter++;   
      

     if (numberCount == 10) {
      numberCount = 0;
      kerroin = kerroin * 0.9;  // Vähennetään aikaväliä 10 %

      // Päivitetään timerin väli (keskeytyksen aika)
       updateOCR1A();
      Serial.print("Nopeus kasvoi! Uusi aikaväli: ");
      Serial.println(kerroin);
     }
     if (score == 99){
      highScore = score;
      stopTheGame();
    }
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
}
void updateOCR1A() {
  OCR1A = (16000000 / 1024)*kerroin - 1; //säädetään pelin nopeus
}
void checkGame(byte nbrOfButtonPush)
{
	 if (userCounter > 0 && userNumbers[userCounter - 1] == randomNumbers[userCounter - 1]) {
        // Jos käyttäjän syöttämä numero on oikea
        score++;  // Lisää pisteitä
        Serial.print("Oikea numero! Pisteet: ");
        Serial.println(score);
        showResult(score);

        // Voit halutessasi tehdä muita toimenpiteitä, kuten ledin vilkuttaminen
       
    } else {
        // Jos numerot eivät täsmää
      
        stopTheGame();
        
    }
}
void startTheGame()
{ 
  delay(100);
   isGameRunning = true;  
   score = 0;
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
 if (score > highScore) {
        highScore = score;
        saveHighScore(highScore);
    }   
   
    Serial.println("Peli pysäytetty");
    counter = 0;
    numberCount = 0;
    kerroin = 1; // Palautetaan alkuperäinen nopeus
    userCounter = 0; // Nollataan käyttäjän laskuri
    updateOCR1A(); // Päivitetään keskeytys
    clearAllLeds();
    setAllLeds();
    isGameRunning = false;
   // see requirements for the function from SpedenSpelit.h
}
void saveHighScore(byte highScore){
  Serial.print("Tallennetaan EEPROM:iin: ");
  Serial.println(highScore);
  EEPROM.update(0, highScore);// Voi valita kumpaa käyttää, update vois olla parempi
 // Päivitä muuttuja jos arvo muuttunut
  //EEPROM.put(0, highScore); // Tallenetaan highScore 0 osotteeseen
}
byte loadHighScore() {
  byte storedHighScore = EEPROM.read(0); 
  Serial.print("Ladattu EEPROM:sta: ");
  Serial.println(storedHighScore);
  return storedHighScore;
}