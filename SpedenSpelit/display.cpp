#include "display.h"

const byte numero_segmentit[] = { // Järjestys abcdefg ja viimeisenä dp, joka on aina 0.
    0b11111100,  // 0
    0b01100000,  // 1
    0b11011010,  // 2
    0b11110010,  // 3
    0b01100110,  // 4
    0b10110110,  // 5
    0b10111110,  // 6
    0b11100000,  // 7
    0b11111110,  // 8
    0b11110110   // 9
  }; 


void initializeDisplay(void)
{
  for(int i = 8; i<=12; i++){ //Initialisoidaan tarvittavat pinit outputeiksi
    pinMode(i, OUTPUT);
  }
  digitalWrite(12, LOW); //Tyhjennetään sarjaväylä datasta
  digitalWrite(12, HIGH);
  digitalWrite(9, LOW); //Output enable on aktiivinen LOW
}


void WriteByte(uint8_t number, bool last) {
  
  byte numeronKuvio = numero_segmentit[number]; //Haetaan oikean numeron kuvio numero_segmentit taulukosta.

  
  for (int i = 0; i < 8; i++) { //Käydään bitti kerrallaan läpi, digitalWrite 1 = HIGH ja 0 = LOW
    digitalWrite(8, numeronKuvio & 1); //Irroitetaan LSB ja välitetään se sajraväylään. numeronKuvio & 0b00000001 = LSB.
    numeronKuvio >>= 1;  //Siirretään bittejä kerran oikealle, että saadaan seuraava bitti LSB kohdalle.
    digitalWrite(11, HIGH); //Kellopulssi sarjaväylän etenemiseksi.
    digitalWrite(11, LOW);
  }


  if (last) {
    digitalWrite(10, HIGH); //Latchataan data outputteihin.
    digitalWrite(10, LOW);
    //digitalWrite(9, LOW);  
    digitalWrite(12, LOW); //Resetoidaan sarjaväylästä numerot pois, että ne eivät siirry seuraavaan näyttöön.
    digitalWrite(12, HIGH);

  }
}

void writeHighAndLowNumber(uint8_t tens,uint8_t ones)
{
  if(tens==0){ 
    WriteByte(ones, true); //Niin kauan kuin kymmeniä on 0, WriteByte saa tulostaa luvun ja tyhjentää sarjaväylän.
  }
  else{ 
    WriteByte(tens,false); //Odotetaan, että kymmenetkin ehtivät rekisteriin.
    WriteByte(ones,true); //Tulostetaan molemmat.
  }
}


void showResult(byte number)
{
  writeHighAndLowNumber((uint8_t(number)/10), uint8_t(number)%10); //Jaetaan kymmenellä toiseen muuttujaan ja otetaan jakojäännös toiseen muuttujaan. 
}
