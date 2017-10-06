/*
MrRoboto.ino

Mr Roboto 
Prof Mike Soltys
University of Colorado
01/15/2014

This example code is in the public domain.

//////////////////////////////////////////////////
DESCRIPTION
Pressing the buttion will play the song Mr Roboto

Note: these buttions are SUPER sensitive (freekishly so). The simon says code usese
a delay and check process called debouncing to fix this.
//////////////////////////////////////////////////

*/



int ledPin =  3;     // LEDs are on pins 3,5,10 and 13.
int buttonPin =  2;  // BUTTONS are on pins 2,6,9 and 12.  
int button_state;    // This variable will be used to "store" the state of the button.

/// These next two definitions are setting up the buzzer pins. 
/// By sending these HIGH/LOW we can create a sound from the buzzer.
int buzzer_1 = 4;
int buzzer_2 = 7;

/* Here are some variables we'll use to make a little jingle. 
   First we'll define frequencies of a few notes to use in the jingle */
const int NOTE_F4 = 349;  // F4 (F above middle c)
const int NOTE_DS4 = 311;  // D-sharp 4
const int NOTE_REST = 0;  // Rest, no tone
const int jingleLength = 12;  // This is the length of the jingle - 12 notes
/* This array contains the note played, in order */
const int jingleNote[jingleLength] = {
  NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_DS4, NOTE_REST,
  NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_DS4};
/* jingleDuration contains the length of each note played 
   8 = 1/8 note, 4 = 1/4 note, 32 = 1/32 note, etc.*/
const int jingleDuration[jingleLength] = {
  8, 8, 8, 8, 8, 4, 32,// do-mo-ar-i-ga-to-(rest)
  4, 4, 8, 8, 4 };  // mis-ter-ro-bot-o
const int jingleBPM = 60;  // Jingle beats-per-minute = 60 bpm

void setup()   {           
  pinMode(ledPin, OUTPUT);     
  
  digitalWrite(buttonPin, HIGH);
  pinMode(buttonPin, INPUT);     

  pinMode(buzzer_1, OUTPUT);
  pinMode(buzzer_2, OUTPUT);
  digitalWrite(buzzer_1, LOW); // buzzer_1 will toggle HIGH/LOW to create the sound - see buzz() function below.
  digitalWrite(buzzer_2, LOW); // buzzer_2 will toggle as well (to create more volume).
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
for (int i=0; i<jingleLength; i++)
  { // Run through this for loop once for each note
    /* First lets calculate how long to play the note 
       this value will be in miliseconds, so we divide 60000 miliseconds
       by our beats per minute, and divide that by the note's duration */
    int noteDuration = (60000/jingleBPM)/jingleDuration[i];
    /* Now we'll play the tone using the tone(pin, frequency, duration)
       function. The pin will always be nosePin, the note is defined above,
       and we just calculated the duration in milliseconds */
    tone(buzzer_1, jingleNote[i], noteDuration);
    /* Now just a short delay, so you can distinguish between the notes.
       This calculation is mostly arbitrary, but it sounds good. */
    delay(noteDuration * 1.3);
  }
}

