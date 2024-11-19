#include "buttons.h"
//volatile int buttonNumber = -1;
volatile bool btnpress = false;
static unsigned long lastbouncetime = 0;
const unsigned long bouncedelay = 100;
void initButtonsAndButtonInterrupts(void)
{
  PCICR |= (1 << PCIE2);        
  PCMSK2 = 0b01111100;
  // See requirements for this function from buttons.h
    for(int napit = 2;napit<6;napit++)
  {
    pinMode(napit, INPUT_PULLUP); 
    
  }
 
  
}

ISR(PCINT2_vect) {
 for(int Button = 2; Button<6; Button++){
     
    
     
    if(digitalRead(Button)==LOW){
    
     
    buttonNumber = Button;
     // Serial.print(Button);
    
     break;
    
    
      
    }
    
 } 
  
}



