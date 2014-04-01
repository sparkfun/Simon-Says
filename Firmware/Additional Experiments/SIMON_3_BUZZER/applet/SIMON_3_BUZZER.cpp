/*
Simon Experiments #2
Buzzer
Pete Lewis
Sparkfun Electronics 
10/13/2010
*/



#include "WProgram.h"
void setup();
void loop();
void buzz();
int ledPin =  3;     // LEDs are on pins 3,5,10 and 13.
int buttonPin =  2;  // BUTTONS are on pins 2,6,9 and 12.  
int button_state;    // This variable will be used to "store" the state of the button.

/// These next two definitions are setting up the buzzer pins. 
/// By sending these HIGH/LOW we can create a sound from the buzzer.
int buzzer_1 = 4;
int buzzer_2 = 7;
                    

void setup()   {           
  pinMode(ledPin, OUTPUT);     
  pinMode(buttonPin, INPUT);     
  
  pinMode(buzzer_1, OUTPUT);
  pinMode(buzzer_2, OUTPUT);
  digitalWrite(buzzer_1, LOW); // buzzer_1 will toggle HIGH/LOW to create the sound - see buzz() function below.
  digitalWrite(buzzer_2, LOW); // buzzer_2 will always stay low.
}


void loop()                     
{
  int button_state = digitalRead(buttonPin);
  
  if(button_state == 1){
    
  digitalWrite(ledPin, HIGH);   // set the LED on
  delay(1000);                  // wait for a second
  digitalWrite(ledPin, LOW);    // set the LED off
  
  // Call the "buzz()" funtion. See below to know what this does.
  buzz();
  
  }
  
}


//////////////////////////////////////////////////////////////////////////////////////
void buzz(){
  /// this function makes the buzzer pin move and crease a sound.
  /// By writing the pin HIGH/LOW in a pattern we can create a frequency.
  /// this FOR LOOP is used to repeat the pattern and let us hear the note for second.
  for(int i = 0; i < 100; i++){
  digitalWrite(buzzer_1, HIGH);
  delay(1);
  digitalWrite(buzzer_1, LOW);
  delay(1);
  }
}


int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

