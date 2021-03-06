/*
Simon Experiments #3
Buzzer
Pete Lewis
SparkFun Electronics 
10/13/2010

Updated by 
Prof Mike Soltys
University of Colorado
01/15/2014

Ho Yun "Bobby" Chan
SparkFun Electronics
10/8/2017

This example code is in the public domain.

//////////////////////////////////////////////////
DESCRIPTION
Pressing the buttion will flash the LED for 1 second and play a tone.
//////////////////////////////////////////////////

*/

int ledPin =  3;     // LEDs are on pins 3,5,10 and 13.
int buttonPin =  2;  // BUTTONS are on pins 2,6,9 and 12.  
int button_state;    // This variable will be used to "store" the state of the button.

/// These next two definitions are setting up the buzzer pins. 
/// By sending these HIGH/LOW we can create a sound from the buzzer.
int buzzer_1 = 4;
int buzzer_2 = 7;

void setup()   {           
  pinMode(ledPin, OUTPUT);     
  
  // Note: For the way the circuit is setup, INPUT_PULLUP will result much more
  // stability than INPUT
  pinMode(buttonPin, INPUT_PULLUP);     

  pinMode(buzzer_1, OUTPUT);
  pinMode(buzzer_2, OUTPUT);
  digitalWrite(buzzer_1, LOW); // buzzer_1 will toggle HIGH/LOW to create the sound - see buzz() function below.
  digitalWrite(buzzer_2, LOW); // buzzer_2 will toggle as well (to create more volume).
}


void loop()                     
{
  button_state = digitalRead(buttonPin);
  
  if(button_state == 0){
    
  digitalWrite(ledPin, HIGH);   // set the LED on
  delay(1000);                  // wait for a second
  digitalWrite(ledPin, LOW);    // set the LED off
  
  // Call the "buzz()" funtion. See below to know what this does.
  tone(buzzer_1,400,500);
  delay(1000);
  tone(buzzer_2,600,500);
  delay(1000);
  tone(buzzer_1,400,500);
  delay(250);
  tone(buzzer_2,600,500);
  //tone(buzzer_1,500,100);
  
  }
  
}

//////////////////////////////////////////////////////////////////////////////////////
void buzz(){
  /// this function makes the buzzer pin move and crease a sound.
  /// By writing the pin HIGH/LOW in a pattern we can create a frequency.
  /// this FOR LOOP is used to repeat the pattern and let us hear the note for second.
  for(int i = 0; i < 100; i++){
  digitalWrite(buzzer_1, HIGH);
  digitalWrite(buzzer_2, LOW);
  delay(1);
  digitalWrite(buzzer_1, LOW);
  digitalWrite(buzzer_2, HIGH);
  delay(1);
  }
}

