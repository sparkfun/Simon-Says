/*

  Simon Experiments #2
  Button
  Pete Lewis
  SparkFun Electronics
  10/13/2010

  Modified by
  Prof Mike Soltys
  University of Colorado
  01/15/14

  Ho Yun "Bobby" Chan
  SparkFun Electronics
  10/8/2017

  This example code is in the public domain.

  //////////////////////////////////////////////////
  SETUP & UPLOAD INSTRUCTIONS
  1. Select "Tools" from the upper window menu.
  2. Select "Serial Port" and then select the COM PORT that your FTDI BASIC is connected on. (It most likely will be the COM 2 or higher).
  3. Select "Tools", then "Board", then "LilyPad Arduino w/ ATmega328"
  4. Click on the "upload button" - it looks like a circle with an arrow to the right.
  //////////////////////////////////////////////////

  //////////////////////////////////////////////////
  DESCRIPTION
  Pressing the button will change the state of the LED from OFF to ON.
  The LED will turn OFF after one second until the button is pressed again.
  //////////////////////////////////////////////////
*/

int ledPin =  3;    // The Simon board has 4 LEDs on it.
                    // For this example, we're just going to use one.
                    // The other LEDs are on pins 3,5,10 and 13.
                    // For fun, try switching "ledPin" to another pin number and see what happens!

int buttonPin =  2; // The Simon board has 4 BUTTONS on it.
                    // For this example, we're just going to use one.
                    // The other BUTTONS are on pins 2,6,9 and 12.
                    // For fun, try switching "buttonPin" to another pin number and see what happens!

int button_state;   // This variable will be used to "store" the state of the button.
                    // It will allow us to know whether the button is pressed or not.

// The setup() funtion runs once, when the sketch starts

void setup()   {
  // initialize the led pin as an output:
  pinMode(ledPin, OUTPUT);

  // initialize the button pin as an input and use internal pullup resistors:
  pinMode(buttonPin, INPUT_PULLUP);
}

// the loop() function runs over and over again,
// as long as the Arduino has power

void loop() {

  // Using the digitalRead() function, we can read the state of a pin, and know whether or not it is Logic HIGH or Logic LOW.
  // When you press the button, you are actually causing an electrical connection between the pin on the micro (buttonPin) and Logic LOW (aka GND).
  // Every time the loop starts over it will first set the variable "button_state" to the state of the pin.
  // It is refreshing every time the loop starts over.
  button_state = digitalRead(buttonPin);

  // The second step in the loop is to actually do something with this variable.
  // In this next "if statement" we are going to decide to do something. Here we are going to turn on the ledPin for a second.
  if (button_state == LOW) { //if button is pressed, do something
    digitalWrite(ledPin, HIGH);  // turn the LED on
    delay(1000);                 // wait for a second
    digitalWrite(ledPin, LOW);   // set the LED off
  }

}
