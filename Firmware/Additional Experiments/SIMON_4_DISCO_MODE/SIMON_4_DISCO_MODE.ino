/*
Simon Experiments #4
Disco Mode
Pete Lewis
Sparkfun Electronics 

This example code is in the public domain.
//////////////////////////////////////////////////
DESCRIPTION
Assuming that there is a photoresistor (i.e. light sensor) attached analog pin 0, Simon
will play a  Bee Gees via the buzzer if the photoresistor is below a certain threshold.
The LEDs will flash in disco mode.

//////////////////////////////////////////////////
 */

int counter = 0;
int leds[] = {3, 5, 10, 13};
int pinCount = 4;           // the number of pins (i.e. the length of the array)
int buzzer_1 = 4;
int buzzer_2 = 7;
int count = 20;
int delaytime = 19;

void setup()  { 
    pinMode(buzzer_1, OUTPUT);
    pinMode(buzzer_2, OUTPUT);
    digitalWrite(buzzer_1, LOW);
    digitalWrite(buzzer_2, LOW);
    for (int thisPin = 0; thisPin < pinCount; thisPin++)  {
    pinMode(leds[thisPin], OUTPUT);    
    digitalWrite(leds[thisPin], LOW);    
    }
} 

void loop()  { 
  int light;
  light = (analogRead(0)/4);
  if(light < 150)
  {
  beegees_loop();  
  }
  else
  {
    for (int thisPin = 0; thisPin < pinCount; thisPin++)  {   
    digitalWrite(leds[thisPin], LOW);    
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////
void beegees_loop(){
buzz(3);
delay(delaytime);
buzz(4);
rest(1);
delay(delaytime);
buzz(5);
rest(1);
rest(1);
buzz(3);
rest(1);
rest(1);
rest(1);
buzz(2);
rest(1);
buzz(1);
buzz(2);
buzz(3);
rest(1);
buzz(1);
buzz(2);
rest(1);
buzz(3);
rest(1);
rest(1);   
buzz(1);
rest(1);
buzz(2);
rest(1);
buzz(3);
rest(1);
buzz(4);
rest(1);
buzz(5);
rest(1);           
delay(23);
}


//////////////////////////////////////////////////////////////////////////////////////
void buzz(int tone){
  
  ///declare an integer, "freq", for frequency of the note to be played.
  int freq;
  
  ///5 different tones to select. Each tone is a different frequency.
  if(tone == 1){
  freq = 2000;
  }
  if(tone == 2){
  freq = 1800;
  }
  if(tone == 3){
  freq = 1500;
  }
  if(tone == 4){
  freq = 1350;
  }
  if(tone == 5){
  freq = 1110;
  }
  
  /// Because frequency is determined by the wavelength (the time HIGH and the time LOW),
  /// you need to have "count" in order to keep a note the same length in time.
  /// "count" is the number of times this function will repeat the HIGH/LOW pattern - to create the sound of the note.
  
  count = 19;
  
  /// In order to keep all 5 notes the same length in time, you must compare them to the longest note (tonic)  - aka the "1" note.
  count = count*(2000/freq);
  
  /// this next function simply changes the next LED to turn on.
  change_led();
  
  /// this next for loop actually makes the buzzer pin move.
  /// it uses the "count" variable to know how many times to play the frequency.
  /// -this keeps the timing correct.
  for(int i = 0; i < count; i++){
  digitalWrite(buzzer_1, HIGH);
  digitalWrite(buzzer_2, LOW);
  delayMicroseconds(freq);
  digitalWrite(buzzer_1, LOW);
  digitalWrite(buzzer_2, HIGH);
  delayMicroseconds(freq);
  }
  delay(60);
}

//////////////////////////////////////////////////////////////////////////////////////
 void rest(int tone){
  int freq;
    if(tone == 1){
    freq = 2000;
    }
    if(tone == 2){
    freq = 1800;
    }
    if(tone == 3){
    freq = 1500;
    }
    if(tone == 4){
    freq = 1350;
    }
    if(tone == 5){
    freq = 1110;
    }
    count = 20;
 count = count*(2000/freq);
change_led(); 
 for(int i = 0; i < count; i++){
 digitalWrite(buzzer_1, LOW);
 delayMicroseconds(freq);
  digitalWrite(buzzer_1, LOW);
 delayMicroseconds(freq);
 }
  delay(75);
}

//////////////////////////////////////////////////////////////////////////////////////
void change_led(){
   digitalWrite(leds[counter], LOW);
   counter = counter+1;
   if(counter >= 4){
    counter = 0; 
   }
 digitalWrite(leds[counter], HIGH);
}
