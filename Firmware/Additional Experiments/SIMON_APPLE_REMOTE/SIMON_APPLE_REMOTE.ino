#include "AppleRemoteSender.h"

int irpin = 15;
byte remote_id = 0x01;

AppleRemoteSender ars = AppleRemoteSender(irpin, remote_id);

//Control Buttons
int play = 9;
int menu = 9; // upper left
int next = 2; // upper right
int previous = 12; //lower left
int volUp = 2; // upper Right?
int volDown = 6; // lower right

// The setup() method runs once, when the sketch starts
void setup()   {                
  // initialize the digital pin as an output:
 // pinMode(play, INPUT);
  pinMode(menu, INPUT);
  pinMode(next, INPUT);
  pinMode(previous, INPUT);
  pinMode(volUp, INPUT);
  pinMode(volDown, INPUT);
  digitalWrite(next, HIGH);
  digitalWrite(previous, HIGH);
  digitalWrite(volUp, HIGH);
  digitalWrite(volDown, HIGH);
  // digitalWrite(play, HIGH);
  digitalWrite(menu, HIGH);
  
}

// the loop() method runs over and over again,
// as long as the Arduino has power

void loop()                     
{
  if(digitalRead(menu) == LOW){
    ars.menu(); //Transmit the codes for Play/Pause
    ars.right();
    ars.menu();
    ars.right();
    ars.menu();
    ars.right();
  }
//  if(digitalRead(play) == LOW){
//    ars.play(); //Transmit the codes for Play/Pause
//    ars.play();
//    ars.play();
//  }
  else if(digitalRead(next) == LOW){
    ars.right(); //Transmit the codes for Next
    ars.right();
    ars.right();
  }
  else if(digitalRead(previous) == LOW){
    ars.left(); //Transmit the codes for Previous
    ars.left();
    ars.left();
  }
  else if(digitalRead(volUp) == LOW){
    ars.up(); //Transmit the codes for Volume Up
    ars.up();
    ars.up();
  }
  else if(digitalRead(volDown) == LOW){
    ars.down(); //Transmit the codes for Volume Down
    ars.down();
    ars.down();
  }
}
