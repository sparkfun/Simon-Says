/* 
 Make the Simon Says Game a 4-hole Ocarina (Like in the legend of zelda)
 
 Prof Mike Soltys
 University of Colorado
 1/28/2014
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

void loop() {
  // keep doing this over and over again.
  // I will start with the most buttons pressed, and move on to the least buttons pressed
  // for any combination, i'll play a matching note and light up the LEDs that aren't being pressed.

  // Start with all buttons pressed - C4 - 262
  int Button=ButtonWait();
  delay(100); 
  if (Button == 1){
    digitalWrite(led_UL, HIGH);
    Button=ButtonWait();
    if (Button==2){
    digitalWrite(led_UR, HIGH);  
    }
  }
  delay(1000);
  digitalWrite(led_UR, LOW);
  digitalWrite(led_UL, LOW);
  digitalWrite(led_LL, LOW);  
  digitalWrite(led_LR, LOW);     
}

int ButtonWait(void){
while(1 == 1){
    if(digitalRead(but_UL) == 0){
      while (digitalRead(but_UL) != 0) ;
      delay(10);
      return 1;
    }
    else if(digitalRead(but_UR) == 0){
      while (digitalRead(but_UR) != 0) ; 
      delay(10);
      return 2;
    }       
    else if(digitalRead(but_LL) == 0){
      while (digitalRead(but_LL) != 0) ;
      delay(10);
      return 3;
    }  
    else if(digitalRead(but_LR) == 0){
      while (digitalRead(but_LR) != 0) ;
      delay(10);
      return 4;
    }
  }
}
