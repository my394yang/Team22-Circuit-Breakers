int main(void){
 
  DDRD = 0b00000000;
  DDRB = 0b00000100; //（when it's DDRD instead of DDRB, taken as voltimeter）
  //DDRD = 1 << 7;
  // 1 is pushed to position 7
  //pinMode(7,OUTPUT);
  
  Serial.begin(9600); // initialize serial communication (freqeuncy of audrino com)
  
  while(1){
    // code that loops forever
    if(PIND & 0b00100000){ //if botton is pressed 
      PORTB |= 0b00000100;
      Serial.println("LED on");
    }
    else{
      PORTB &= 0b11111011;
      Serial.println("LED off");
    }
    _delay_ms(5); // short debounce 
  }  
}