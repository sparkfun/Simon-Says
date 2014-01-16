/*

Simon Experiments #2
Button
Pete Lewis
Sparkfun Electronics 
10/13/2010

Modified by
Prof Mike Soltys
University of Colorado
01/15/14
This example code is in the public domain.

//////////////////////////////////////////////////
SETUP & UPLOAD INSTRUCTIONS
1. Select "Tools" from the upper window menu.
2. Select "Serial Port" and then select the COM PORT that your FTDI BASIC is connected on. (It most likely will be the COM 2 or higher).
3. Select "Tools", then "Board", then "LilyPad Arduino w/ ATmega328"
4. Click on the "upload button" - it looks like a box with an arrow to the right.
//////////////////////////////////////////////////

//////////////////////////////////////////////////
DESCRIPTION
Pressing the buttion will change the state of the LED from on to off or off to on.

Note: these buttions are SUPER sensitive (freakishly so). The simon says code usese
a delay and check process called debouncing to fix this.
//////////////////////////////////////////////////
*/



int ledPin =  3;    // The simon board has 4 LEDs on it. 
                    // For this example, we're just going to use one. 
                    // The other LEDs are on pins 3,5,10 and 13.
                    // For fun, try switching "ledPin" to another pin number and see what happens!
                    
int buttonPin =  2; // The simon board has 4 BUTTONS on it. 
                    // For this example, we're just going to use one. 
                    // The other BUTTONS are on pins 2,6,9 and 12.
                    // For fun, try switching "buttonPin" to another pin number and see what happens!
                    
int button_state;   // This variable will be used to "store" the state of the button.
                    // It will allow us to know whether the button is pressed or not.

int led_state = 0;      // This variable will be used to "store" the state of the LED.
                    // It will allow us to know whether the LED is on or off.
// The setup() funtion runs once, when the sketch starts

void setup()   {                
  // initialize the led pin as an output:
  pinMode(ledPin, OUTPUT);     
  // initialize the internal pull-up on the button pin:
  digitalWrite(buttonPin, HIGH);
  // initialize the button pin as an input:
  pinMode(buttonPin, INPUT);     
  
}

// the loop() function runs over and over again,
// as long as the Arduino has power

void loop()                     
{
  
  // Using the digitalRead() function, we can read the state of a pin, and know whether or not it is Logic HIGH or Logic LOW.
  // When you press the button, you are actually causing an electrical connection between the pin on the micro (buttonPin) and Logic LOW (aka GND).
  // Every time the loop starts over it will first set the variable "button_state" to the state of the pin.
  // It is refreshing every time the loop starts over.
  int button_state = digitalRead(buttonPin);
  
  // The second step in the loop is to actually do something with this variable.
  // In this next "if statement" we are going to decide to do something. Here we are going to turn on the ledPin for a second.
  if(button_state == 1){
    // If the LED is off, turn it on
    if (led_state == 0 ){
      digitalWrite(ledPin, HIGH);   // set the LED on
      led_state = 1; 
      }
     // If the LED is on, turn it off
     else if (led_state == 1 ) {
      digitalWrite(ledPin, LOW);    // set the LED off
      led_state = 0;
     }
     delay(1000);              // wait for a second
  }
  
}
