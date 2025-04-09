//LEDS represent motor
//Green is forward, red is backwards

int main(void){ 
  // setup code that only runs once
  DDRB= 0b00001111;  //initialize LEDs as outputs
  
  driveForward(1371);
    stop(10); 
   turnRight(420); 
       stop(10); 
   driveForward(1371);
       stop(10); 
   turnLeft(420);
       stop(10); 
    driveForward(685);
        stop(10); 
    driveBackwards(2100);
        stop(10); 
    turnLeft(380);
        stop(10); 
    driveForward(1371);
    stop(10); 

  while(1){ 
    // code that loops forever
    //write code to "drive forward" 
    // green LEDS on, red LEDs off
    
    

   //write code to make robot "drive backwards"
    // greeen off red on
    
    
  
  }
  
  
  
}


int driveForward (int t){
    PORTB |= 0b00001010; //Pins 9 and 11 high
    PORTB &= 0b11111010; // pins 8 and 10 low
    delay(t);
}

int  driveBackwards(int t){
    PORTB |= 0b00000101; //pins 8 and 10 high
    PORTB &= 0b11110101;  //pins 9 and 11 low
   delay(t);
}

int turnRight (int t){
  PORTB |= 0b00001001;  //pins 8 and 11 high
  PORTB &= 0b11111001;  //pins 9 and 10 low
   delay(t);
}

int turnLeft(int t){
  PORTB |= 0b00000110;  //pins 9 and 10 high
  PORTB &= 0b11110110;  //pins 8 and 11 low
   delay(t);
}


int stop(int t){
  PORTB &= 0b11110000;
  delay(t);
  
}

int delay(int n) {
  for(int i=0; i<n; i++){
     _delay_ms(1);
  }
}

