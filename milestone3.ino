/*
 * Milestone 4: Color Sensing w/ TCS3200 Color Sensor + Boarder Detection w/ QTI Line Sensor
 *
 * Created (Date): 4/16/2025
 * Author (Name, netID): Michelle Yang, my394
 *
 *
 * Milestone description:
 * The robot starts on a random color (blue or yellow), in a random orientation, and can be place any distance away from the other color.
 * The robot must drive on a narrow a blue and yellow track.
 * The robot must immediately stop when it hits the other color. 
 * The robot then turn 180 degrees and drive to the back of the starting color, and stop again.
 * Boarder detection is necessary to prevent the robot from leaving the track.
 *
 * Color detection plan: 
 * S2 and S3 will be set to Low and High respectively to select the blue filter.
 * When the output frequency of the color sensor (w/ blue filter) is high -> on blue
 * Red and green at equal intensity results in the color yellow, so when the output frequency of the color sensor (w/ blue filter) is low -> on yellow
 *
 */ 

// --- Global Variables ---

// "period" : stores the value of the output wave period (in microseconds) 
volatile int period = 0; 

// "timer" : stores the value of TIMER1 (16 bits, higher resolution for color sensing)
volatile int timer = 0; 

// "color_current" : stores current/past color
volatile char color_current = NULL; // Blue (B), Yellow (Y)

// "color_next" : stores next color for comparison with color_current
volatile char color_next = NULL;

// --- ISR: an interrupt function that resets and reads the timer value (measures in clock ticks, 1/2 period of output signal of color sensor signal)

ISR(PCINT2_vect) // Arduino port D pin 4 -> PCINT20 -> Pin Change Interrupt Request 2 
{	

  
  
  // Resets the timer to zero on a rising edge (start measuring 1/2 period)
  if (PIND & 0b00010000) {
    timer = TCNT1; // Store TIMER1 value TCNT1 in variable timer -> output frequency of color sensor
  } else { // Stores the timer value in a variable ("timer") on a falling edge (end measuring 1/2 period)
    TCNT1 = 0; // Reset TIMER1 clock
  }  
}


// --- initColor: initializes the interrupt and timer 

void initColor()
{

  // Initialize I/O pins -> control motors
  DDRB = 0b00001111;  // Initialize H-bridge controls as outputs
 
  // Initialize pin change interrupt -> Set Pin Change Interrupt Enable 2 (PCIE2) for  PCINT20 (Arduino port D, pin 4)
  PCICR = 0b00000100;

  // Initialize timer -> set TIMER1 (16 bits) to normal mode (for now) and set prescaler N
  // ATmega328P uses a 16MHz clock. The desired period to be measured: 50μs - 250μs (visible color spectrum).
  // 2^(16) * (62.5 ns) * N = overflow period
  // CS12, CS11, CS10  -> 001 (N = 1)
  // Minimum prescaler of TIMER1 = N = 1
  // WGM11, WGM10, WGM13, WGM12 = 0 (mode 0, Normal)
  TCCR1A = 0b00000000; 
  TCCR1B = 0b00000001; 

}


// --- getColor: calculates the period of the sensor output wave and returns it as a variable (in microsecond) 

int getColor()
{

  // Enable specific bit for Pin Change Interrupt
  PCMSK2 = 0b00010000; // Set bit 4, PCINT20
    
  // Add a short delay (5 ~ 10 milliseconds) to give interrupt time to trigger
  _delay_ms(7); // 7 ms

  // Disable specific bit for pin change interrupt
  // (to prevent further interrupts until call getColor again)
  PCMSK2 = 0b00000000;
  
  // Store to global variable period in units of time (ms)
  // Convert value for ISR from clock ticks to microseconds, then multiply by 2 to get 1 full period
  Serial.print("Timer in clock ticks: ");
  Serial.println(timer);
  period = (timer*(0.0625))*2; // 6.25ns = 0.0625μs
  return period; // Return value as a variable (period)

}


int main(void)
{
	
  // Call initColor function
  initColor();

  Serial.begin(9600); // For debugging, initialize serial communication

  sei();

  // Call getColor function
  getColor();
   // Control robot motion (steering w/ DC motors)
    // variable for initial
    // If period is above certain ____ (detects on blue) -> Need to measure in lab
    if ((period > 290) && (period < 315)) { 
      // Set a variable to Blue
      color_current = B;
    } else if ((period < 290) && (period > 150)) {
      // Set a variable yellow
      color_current = Y;
    } else {
      color_current = O;
    }

    int counter = 0;
    

  while(1) 
  {

    // Call getColor function
    getColor();

    

    // Print the "period" -> serial communication
    Serial.print("Period in μs: ");
    Serial.println(period);

    // Add short delay (less than a second) for readability in monitor
    _delay_ms(500); // 1/2 a second

    if ((period > 290) && (period < 315)) { 
      // Set a variable to Blue
      color_next = B;
    } else if ((period < 290) && (period > 150)) {
      // Set a variable yellow
      color_next = Y;
    } else {
      color_next = O;
    }


    if (color_current == color_next) {
      driveForward();
    } else {
      counter += 1;
      if (counter == 2) {
        break;
      }
      stop(10);
      // Turn 180 degrees
      turnRight(900);
      

    }

  }

    /*
     * Blue ~ 310 period
     * Black ~ 330 period
     * Yellow ~ 200 period
     * Floor ~ na

    */

}

int driveForward (){
    PORTB |= 0b00001010; //Pins 9 and 11 high
    PORTB &= 0b11111010; // pins 8 and 10 low
}

int  driveBackwards(){
    PORTB |= 0b00000101; //pins 8 and 10 high
    PORTB &= 0b11110101;  //pins 9 and 11 low
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
