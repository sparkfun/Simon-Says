/* 
Make the Simon Says Game a 4-hole Ocarina (Like in the legend of zelda)

*/
// Define the button, LED, and buzzer pins
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
  // Start with all buttons pressed - C4 - 262
  if (digitalRead(but_UL) == 0 && digitalRead(but_UR) == 0 && digitalRead(but_LL) == 0 && digitalRead(but_LR) == 0) {
  tone(BUZZER2,262,50);
  }
  // uncover upper right - D4 - 294
  else  if (digitalRead(but_UL) == 0  && digitalRead(but_LL) == 0 && digitalRead(but_LR) == 0) {
  digitalWrite(led_UR, HIGH);
  tone(BUZZER2,294,50);
  }
  // uncover lower right - E4 -330
  else  if (digitalRead(but_UL) == 0 && digitalRead(but_UR) == 0  && digitalRead(but_LL) == 0) {
  tone(BUZZER2,330,50);
  digitalWrite(led_LR, HIGH);
  }
  // uncover upper left - F4# -370
  else  if (digitalRead(but_UR) == 0  && digitalRead(but_LL) == 0 && digitalRead(but_LR) == 0) {
  digitalWrite(led_UL, HIGH);
  tone(BUZZER2,370,50);
  }
  // uncover both right - F4 -349
  else  if (digitalRead(but_UL) == 0 && digitalRead(but_LL) == 0) {
  digitalWrite(led_UR, HIGH);
  digitalWrite(led_LR, HIGH);
  tone(BUZZER2,349,50);  
  }  
  // uncover both top - G4 -392
  else  if (digitalRead(but_LL) == 0 && digitalRead(but_LR) == 0) {
  digitalWrite(led_UR, HIGH);
  digitalWrite(led_UL, HIGH);  
  tone(BUZZER2,392,50);
  }    
  // uncover upper left and lower right - G4# -415
  else  if (digitalRead(but_LL) == 0 && digitalRead(but_UR) == 0) {
  digitalWrite(led_LR, HIGH);
  digitalWrite(led_UL, HIGH);   
  tone(BUZZER2,415,50); 
  }     
  // cover LL only - A4 -440
  else  if (digitalRead(but_LL) == 0) {
  digitalWrite(led_UR, HIGH);
  digitalWrite(led_UL, HIGH);  
  digitalWrite(led_LR, HIGH);  
  tone(BUZZER2,440,50);   
  }
  // cover LR only - A4# -466
  else  if (digitalRead(but_LR) == 0) {
  digitalWrite(led_UR, HIGH);
  digitalWrite(led_UL, HIGH);  
  digitalWrite(led_LL, HIGH);   
  tone(BUZZER2,466,50);  
  }        
  // cover UR only - B4 -494
  else  if (digitalRead(but_UR) == 0) {
  digitalWrite(led_UL, HIGH);
  digitalWrite(led_LL, HIGH);  
  digitalWrite(led_LR, HIGH);   
  tone(BUZZER2,494,50);    
  }
  // all open - C5 -523
  else {
  digitalWrite(led_UL, HIGH);
  digitalWrite(led_UR, HIGH);  
  digitalWrite(led_LL, HIGH);  
  digitalWrite(led_LR, HIGH);   
  tone(BUZZER2,523,50);    
  }      
  // Turn off all LEDs
  digitalWrite(led_UR, LOW);
  digitalWrite(led_UL, LOW);
  digitalWrite(led_LL, LOW);  
  digitalWrite(led_LR, LOW);     
}

