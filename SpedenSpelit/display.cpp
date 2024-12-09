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
  DDRB |= 0b00011111;
  PORTB &= ~(1 << PB4);  //Tyhjennetään sarjaväylä datasta varalta.
  PORTB |= (1 << PB4);
  PORTB &= ~(1<<PB1); //Output enable on aktiivinen LOW
}


void WriteByte(uint8_t number, bool last) {

  byte numeronKuvio = numero_segmentit[number];  //Haetaan oikean numeron kuvio numero_segments taulukosta.

  for (int i = 0; i < 8; i++) {         //Käydään bitti kerrallaan läpi.

    PORTB = (PORTB & ~(1 << PB0)) | ((numeronKuvio & 1) << PB0); // Tyhjennetään PB0 ja lisätään numeronKuvio LSB.
    numeronKuvio >>= 1; //Siirretään bittejä kerran oikealle.
    PORTB |= (1 << PB3); //Kellopulssi sarjaväylän etenemiseksi.
    PORTB &= ~(1 << PB3);
  }
  if (last) {
    PORTB |= (1 << PB2); //Latchataan data outputteihin.
    PORTB &= ~(1 << PB2);
    PORTB &= ~(1 << PB4); //Resetoidaan sarjaväylästä numerot pois, että ne eivät siirry seuraavaan näyttöön.
    PORTB |= (1 << PB4);

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