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
int randomNumbers[100];  // Satunnainen taulukko pelin arvotuille numeroille
int userNumbers[100];    // Käyttäjän syöttämille numeroille
int counter = 0;  // Laskuri, kuinka monta numeroa on arvottu
int numberCount = 0;  // Laskuri arvottujen numeroiden määrälle
int userCounter = 0; // Laskuri, käyttäjän painamille numeroille
byte score = 0; //pisteiden muuttuja
byte highScore = 0; //highscore muuttuja
byte currentLed = -1;


void setup(){
  delay(500); //0.5 sekunnin viive 
  Serial.begin(9600);  // Aloitetaan sarjayhteys 9600 baudilla
  initializeTimer(); 
  initializeDisplay(); //näyttön koodit päälle
  isGameRunning = false;
  initializeLeds();  //ledienkoodit päälle
  initButtonsAndButtonInterrupts(); // suorittaa nappien inilizaation
  interrupts(); //interruptit päälle
  // EEPROM
  highScore = loadHighScore(); //ladataan highscore näytölel
  showResult(highScore); //ladataan highscore näytölel
  
}

void loop()
{

 if (buttonNumber >= 0) {// jos buttonNumber on yli 0 suoritetaan if lause
    if (buttonNumber == 4) {//jos ButtonNumber on 4 eli painetaan nappia 6 niin aloitetaan peli
        startTheGame();
    } else if (isGameRunning && userCounter < 100) {
        userNumbers[userCounter] = buttonNumber;//tallentaa napin painalluksen taulukkoon
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
      numberCount ++;
       }
     // Päivitetään laskuri ja tarkistetaan, onko 10 numeroa arvottu
      counter++;   
     if (numberCount == 10) { //jos numero laskut on 10
      numberCount = 0;        //numero laskuri takaisin 0
      kerroin = kerroin * 0.9;  // Vähennetään aikaväliä 10 %
      // Päivitetään timerin väli (keskeytyksen aika)
       updateOCR1A();//päivitetään ocr1a
     }
     if (score == 99){ //jos score on 99
      highScore = score;
      stopTheGame(); //pysäytetään peli
      }
    }
}
void initializeTimer(void){
  TCCR1A = 0; //nollataan timer reksiteri
  TCNT1 = 0;  //nollaa timerin
  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);  // CTC-tila, prescaler 1024
  updateOCR1A(); //päivitetään ocr1a
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
        showResult(score); //näytä score näytöllä 
    } else {
        //jos numerot ei täsmää
        stopTheGame(); // lopeta peli
    }
}
void startTheGame()
{ 
  showResult(highScore);
  ledVilkutus();
  showResult(0); //näytölle näkymään 0 
  initializeTimer();
   isGameRunning = true;  //asetetaan pelipäälle
   score = 0;   //Nollataan pisteet
   kerroin = 1; // Nollataan kerroin
   counter = 0; // Nollataan laskuri
   userCounter = 0; // Nollataan käyttäjän laskuri
    for (int i = 0; i < 100; i++) {
        userNumbers[i] = -1; // Nollataan käyttäjän numerot
    }
}
void stopTheGame()
{
 if (score > highScore) {
        highScore = score;
        saveHighScore(highScore);
    }   
    counter = 0; //nollataan lasku
    numberCount = 0; //nollataan numeroiden lasku
    kerroin = 1; // Palautetaan alkuperäinen nopeus
    userCounter = 0; // Nollataan käyttäjän laskuri
    updateOCR1A(); // Päivitetään keskeytys
    clearAllLeds(); //ledit pois päältä
    setAllLeds();   //kaikki ledit päälle
    isGameRunning = false; //peli pois päältä
}
void saveHighScore(byte highScore){
  EEPROM.update(0, highScore);// Voi valita kumpaa käyttää, update vois olla parempi
 // Päivitä muuttuja jos arvo muuttunut
  //EEPROM.put(0, highScore); // Tallenetaan highScore 0 osotteeseen
}
byte loadHighScore() {
  byte storedHighScore = EEPROM.read(0);  //luetaan high score eepromista
  return storedHighScore; //palautetaan
}
void ledVilkutus(){
  showResult(highScore);
       delay(500);   //0.5s tauko
  setAllLeds(); //ledit päälle
       delay(500); //0.5s tauko
  clearAllLeds(); //ledit pois päältä
       delay(500); //0.5s tauko
  setAllLeds(); //ledit päälle
       delay(500); //0.5s tauko
  clearAllLeds(); //ledit pois päältä
       delay(500); //0.5s tauko
  setAllLeds(); //ledit päälle
       delay(500); //0.5s tauko
  clearAllLeds();  //ledit pois päältä
       delay(500); //0.5s tauko
  showResult(0); //näytölle näkymään 0 
  }