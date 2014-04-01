/* 
 Code checks for combination in order. Must press upper left button, then upper right button,
 then lower left for success.  Any wrong button will result in an unplesant sound, 
 and the code will reset.
 
 Prof Mike Soltys
 University of Colorado
 1/31/2014
 */


// Define the button, LED, and buzzer pins (this is standard for all Simon Says games
// I've labeled mine UL for Upper Left, LR for Lower Left and so on. 
const int led_UL = 10; //Red
const int led_UR = 3; //Green
const int led_LL = 13; //Blue
const int led_LR = 5; //Yellow

const int but_UL = 9;
const int but_UR = 2;
const int but_LL = 12;
const int but_LR = 6;

const int BUZZER1 = 4;
const int BUZZER2 = 7;

// We'll set up variables for each button, so Upper Left is Button 1,
// Upper right is 2, etc.
const int UL = 1;
const int UR = 2;
const int LL = 3;
const int LR = 4;

void setup() {
  //Setup hardware inputs/outputs. 
  //Enable pull ups on inputs
  pinMode(but_UL, INPUT_PULLUP);
  pinMode(but_UR, INPUT_PULLUP);
  pinMode(but_LL, INPUT_PULLUP);
  pinMode(but_LR, INPUT_PULLUP);

  pinMode(led_UL, OUTPUT);
  pinMode(led_UR, OUTPUT);
  pinMode(led_LL, OUTPUT);
  pinMode(led_LR, OUTPUT);

  pinMode(BUZZER1, OUTPUT);
  pinMode(BUZZER2, OUTPUT);
}

// keep doing this over and over again.
void loop() {
  // this calls the bit of code ButtonWait() which is found below
  // the code waits untill a button is pressed, and then tells you which button was pressed
  // (1, 2, 3, 4)
  int Button = ButtonWait();

  // check to see if the button was the first one in our sequence (Upper Left)
  if (Button == UL){
    // if so, turn on the upper left button
    digitalWrite(led_UL, HIGH);
    // and wait for another button to be pressed
    Button = ButtonWait();
    // now check to see if the second button (UR) in our sequence was pressed
    if (Button == UR){
      // if so, turn that button on and continue
      digitalWrite(led_UR, HIGH);  
      Button = ButtonWait();
      if (Button == LL){
        // if so, turn that button on and continue
        digitalWrite(led_LL, HIGH);  
        // play success tone!
        tone(BUZZER1,399,1000);
        // interesting fact: 399 Hz is the "most plesant" frequency
        // http://www.ncbi.nlm.nih.gov/pubmed/503755

        // if at any point a wrong button is pressed, the code will jump down
        // to delay 1000, turn off the LEDs, and restart at the top of void loop()
      }
      // if the wrong button is pressed, play an unplesant sound
      else {
        tone(BUZZER1,2000,500);
      }
    }
    // if the wrong button is pressed, play an unplesant sound
    else {
      tone(BUZZER1,2000,500);
    }
  }
  // if the wrong button is pressed, play an unplesant sound
  else {
    tone(BUZZER1,2000,500);
  }

  // wait a second and turn off all LEDS
  delay(1000);
  digitalWrite(led_UR, LOW);
  digitalWrite(led_UL, LOW);
  digitalWrite(led_LL, LOW);  
  digitalWrite(led_LR, LOW);     
}

// Waits for a button to be pressed, then returns 1-4 to tell what button it was.
int ButtonWait(void){
  // this is always true, so this loop will keep going until it reaches a "return" command  
  while(1 == 1){
    // Check if the UL button is pressed
    if(digitalRead(but_UL) == 0){
      //wait till the user releases the button
      while (digitalRead(but_UL) == 0){
        delay(10);
      }
      // return what button was pressed
      return(UL);
    }
    // Check if the UR button is pressed
    else if(digitalRead(but_UR) == 0){
      //wait till the user releases the button
      while (digitalRead(but_UR) == 0){
        delay(10);
      }
      // return what button was pressed
      return(UR);
    }
    // and so on.    
    else if(digitalRead(but_LL) == 0){
      while (digitalRead(but_LL) == 0){ 
        delay(10);
      }
      return(LL);
    }  
    else if(digitalRead(but_LR) == 0){
      while (digitalRead(but_LR) == 0){ 
        delay(10);
      }
      return(LR);
    }
    // if we get here, nothing has been pressed, and so we'll restart at the top of
    // the loop while (1==1).  
  }
}





