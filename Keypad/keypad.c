/*
Author: Raphael Nagel
UWE Bristol - BSc Hons Robotics - 10032284
Date created: 19/Nov/2012 - based on the Keypad.c source file by same author - 02/Mar/2012

This code reads and debounces the input from a Rapid 4 x 4 keypad from Accord.
It lights up a LED everytime a debounced button is pressed and sends the  ASCII code equivalent to the printing on the keypad via serial to the pc.

It is a prototype for the code used in the VHDL Co-Design Group Coursework 2012/13
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;
//the columns and pins on the used keypad are mapped here...
int c1 = 4;
int c2 = 6;
int c3 = 7;
int c4 = 8;
int r1 = 9;
int r2 = 2;
int r3 = 3;
int r4 = 5;

int key_pressed = 0;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
    pinMode(led, OUTPUT);    
 
 //initialises the rows as output and the columns as inputs
    pinMode(r1, OUTPUT); //KEYPAD_TRIS_R1 = 0;
    pinMode(r2, OUTPUT);   // KEYPAD_TRIS_R2 = 0;
    pinMode(r3, OUTPUT);    //KEYPAD_TRIS_R3 = 0;
    pinMode(r4, OUTPUT);    //KEYPAD_TRIS_R4 = 0;

    pinMode(c1, INPUT); //KEYPAD_TRIS_C1 = 1;
    pinMode(c2, INPUT); //KEYPAD_TRIS_C2 = 1;
    pinMode(c3, INPUT); //KEYPAD_TRIS_C3 = 1;
    pinMode(c4, INPUT); //KEYPAD_TRIS_C4 = 1; 
       
    //set up the serial port
    Serial.begin(9600); 
 
   // initialize Timer1
    cli();             // disable global interrupts - good practice while setting up
    TCCR1A = 0;        // set entire TCCR1A register to 0
    TCCR1B = 0;        //this is to start from a clean setup when setting up the timer 
 // set compare match register to desired timer count(# timer counts + 1) = (target time(1ms)) / (timer resolution(16Mhz^1*prescaler)):
 //the interrupt/timer will trigger as soon as the timer value is equal to the value set in OCR1A
    OCR1A = 156;
    // turn on CTC mode (Clear Timer on Compare Match):
    TCCR1B |= (1 << WGM12);
    // enable Timer1 compare match interrupt:
    TIMSK1 = (1 << OCIE1A);
    // Set CS10 bit so timer runs at clock speed/1024 - (clk = 16Mhz) --> setting the hardware prescaler:
    //relates to datasheet ...
    TCCR1B |= (1 << CS10);
    TCCR1B |= (1 << CS12);
    // enable global interrupts:
    sei();
}

ISR(TIMER1_COMPA_vect) //this timer1 interrupt should trigger every 1 ms as defined by the initialise Timer1 setup above
{
  int static threshold = 0;
 
  //read the keypad and store its output
  //the keypad returns the ASCII code for all its symbols(look at the buttons)
  //if nothing has been pressed it returns the ASCII 'X'
  //the following code samples the keypress "threshold"-times and only triggers an output when the button press hasn't changed inbetween.
  key_pressed = read_keypad(); 

  if(key_pressed != 'X'){ //if a button has been pressed
    threshold++;
    if(threshold >=10){ //sample the input multiple times
        digitalWrite(led, HIGH); //turn on the LED
        Serial.println(key_pressed); //output the ASCII code for the button pressed
        threshold = 0;
     }
  }else{
    threshold = 0;
 digitalWrite(led, LOW); //turn off the LED

  }
 }

// the loop routine runs over and over again forever - nothing happens in here, all in the interupt service routine
void loop() {
  
}


/*************************************

 * Keypad source file

 * Author: Raphael Nagel

 * Date: 02/March/2012

 *

 *************************************/


unsigned int check_columns(unsigned int row) {

    switch (row) { //set one or many of the rows low(this is low because we have permanent pullups on the columns. In order to be able to sense that a row and column are connected we will monitor the columns.
                   // normally they should be at logic high voltage because they are pulled high with pull-up resistors. When the button is pressed on that row the current flows across the pullup resistor to ground. 
                   //Therefore the voltage at the pin will lower and it will be interpreted at logic low. We then know on which column the button has been pressed If we only set one row low(beforehand) and we know which one,
                  // then we know which button has been pressed

        case 0:

           digitalWrite(r1,0);
           digitalWrite(r2,1);
           digitalWrite(r3,1);
           digitalWrite(r4,1);


            break;

        case 1:

           digitalWrite(r1,1);
           digitalWrite(r2,0);
           digitalWrite(r3,1);
           digitalWrite(r4,1);

            break;

        case 2:
           digitalWrite(r1,1);
           digitalWrite(r2,1);
           digitalWrite(r3,0);
           digitalWrite(r4,1);
            break;

        case 3:

           digitalWrite(r1,1);
           digitalWrite(r2,1);
           digitalWrite(r3,1);
           digitalWrite(r4,0);

            break;

        default:
           digitalWrite(r1,0);
           digitalWrite(r2,0);
           digitalWrite(r3,0);
           digitalWrite(r4,0);

    }

    //check the rows to see where the button is pressed

    if (digitalRead(c1) == 0)

        return (0);

    if (digitalRead(c2) == 0)

        return (1);

    if (digitalRead(c3) == 0)

        return (2);

    if (digitalRead(c4) == 0)

        return (3);

return(6);

}



char read_keypad(){

	unsigned int r;

	unsigned int c;

	char keys[4][4] = {

	{'1','2','3','A'},

	{'4','5','6','B'},

	{'7','8','9','C'},

	{'*','0','#','D'}};
	

	for(r = 0; r<4;r++){

		c = check_columns(r);

		if(c<6){
			return(keys[r][c]); //returns the ASCII value matching the print on the key pressed.

		}

	}		

	return ('X'); //returns 'X' when no button is pressed.

}
