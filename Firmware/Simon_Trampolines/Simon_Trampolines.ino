/*
 Started: 6-19-2007
 Spark Fun Electronics
 Nathan Seidle
 
 Simon Says is a memory game. Start the game by pressing one of the four buttons. When a button lights up, 
 press the button, repeating the sequence. The sequence will get longer and longer. The game is won after 
 13 rounds.
 
 This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 
 Simon Says game originally written in C for the PIC16F88.
 Ported for the ATmega168, then ATmega328, then Arduino 1.0.
 Fixes and cleanup by Joshua Neal <joshua[at]trochotron.com>
 
 Generates random sequence, plays music, and displays button lights.
 
 Simon tones from Wikipedia
 - A (red, upper left) - 440Hz - 2.272ms - 1.136ms pulse
 - a (green, upper right, an octave higher than A) - 880Hz - 1.136ms,
 0.568ms pulse
 - D (blue, lower left, a perfect fourth higher than the upper left) 
 587.33Hz - 1.702ms - 0.851ms pulse
 - G (yellow, lower right, a perfect fourth higher than the lower left) - 
 784Hz - 1.276ms - 0.638ms pulse
 
 The tones are close, but probably off a bit, but they sound all right.
 
 The old version of SparkFun simon used an ATmega8. An ATmega8 ships
 with a default internal 1MHz oscillator.  You will need to set the
 internal fuses to operate at the correct external 16MHz oscillator.
 
 Original Fuses:
 avrdude -p atmega8 -P lpt1 -c stk200 -U lfuse:w:0xE1:m -U hfuse:w:0xD9:m
 
 Command to set to fuses to use external 16MHz: 
 avrdude -p atmega8 -P lpt1 -c stk200 -U lfuse:w:0xEE:m -U hfuse:w:0xC9:m
 
 The current version of Simon uses the ATmega328. The external osciallator
 was removed to reduce component count.  This version of simon relies on the
 internal default 1MHz osciallator. Do not set the external fuses.

 3/2/2016
 Pete Lewis
 Modifying code to work better for trampolines

1) 
 Fix bug on startup when all four players are standing on trampolines
 -I think this is something to do with the variable keeping track of if trampolines have been "up" meaning someone jumped up or steped off completely.
  - This is necessary so that you can stand on the trampolines or "walk" from one to another. It makes the triggering occur only when a trampoline crosses the threshold
   - of high to low. FIXED 3/2/2016

2)
 Increase playback speed as sequence grows. This will allow for faster playing. We are planning on having groups of 4 play for 2 minutes, so they need to be able to try a few times,
 and potentially win a round within that time frame. ADDED 3/2/2016

3)
  One of the tablah sounds is too low for the computer speakers to actually make, so I need to change this to a sound that will be heard.
  Also, it could be good to choose some more groups to add variety. FIXED 3/2/2016

4) 
Add some code to avoid repeating the same button more than 2 times in a sequence DONE 3/2/2016 - mostly happens in add_to_moves()

5)
Ensure that each button gets chosen an equal amount. This will ensure that when you are playing four player trampolines, everyone gets to jump. 


 
 */

#include "hardware_versions.h"
#include "pitches.h" // Used for the MODE_BEEGEES, for playing the melody on the buzzer!

// Define game parameters
int ROUNDS_TO_WIN = 8; //Number of rounds to succesfully remember before you win. 13 is do-able. 8 is more do-able on trampolines :)
int MAX_seq_repeat = 2; // Max sequence repeat. Used to ensure variety in pattern. NOTE, this must be changed if you change rounds to win
#define ENTRY_TIME_LIMIT   12000 //Amount of time to press a button before game times out. 3000ms = 3 sec

#define MODE_MEMORY  0
#define MODE_BATTLE  1
#define MODE_BEEGEES 2
#define MODE_COMBO_BUTTON 3
#define MODE_MUSICAL_INST 4

// Game state variables
byte gameMode = MODE_MEMORY; //By default, let's play the memory game
byte gameBoard[32]; //Contains the combination of buttons as we advance
byte gameRound = 0; //Counts the number of succesful rounds the player has made it through
boolean debug = true; // turn on/off debug for development

// TRAMPOLINE EXTENTION STUFF
boolean trampoline = true; // this changes the delay before start game to 1 sec
                           // in order for the flag servors to settle.
                           // Also effects the button sensing to not require a release, 
                           // this helps when transitioning or "walking" from tramp to tramp.

boolean real_time_mode_input = false; // if true, it will look for a specific position on a potentiometer on A5
                            // At every exit of attract(), it will check the potentiometer position and
                            // go to that mode of the game.
                            
// FOUR variables to keep track of each trampoline status.
// To register a true jump on a trampoline, 
// we are looking for when the trampoline changes from HIGH (not standing on it) to LOW (standing on it)
// This is different than bressing a button,
// Because when pressing a button, we wait for the release moment to actually trigger the button
// When jumping on trampolines, we want to the trigger point to be exactly when a tampoline transitions
// from HIGH to LOW. This means that the player could still hold the previous trampoline LOW 
// (i.e. that could have one foot on the previous step)
// and still use their other foot to trigger the next step.
// These four variables keep track of how long each trampoline has been in the HIGH state
// Doing this, allows us to require that a trampoline be in the HIGH state for a certain
// amount of time. This helps debounce and prevent false triggering.

// 3/2/2016, initiating all of these to 0, so that you can start a round standing on one trampoline, or any combo.

int HIGH_COUNTER_RED = 0;
int HIGH_COUNTER_GREEN = 0;
int HIGH_COUNTER_BLUE = 0;
int HIGH_COUNTER_YELLOW = 0;

// Adding in counters to keep track of how many times each button has been added to the sequence,
// these will be used to avoid "favoring" a certain button, and ensure that each player gets to jump :)
// these will be adjusted in "add_to_moves()" and reset before each new round.

int RED_seq_count = 0;
int GREEN_seq_count = 0;
int BLUE_seq_count = 0;
int YELLOW_seq_count = 0;

// EXT MIDI TRIG STUFF
// This is used to trigger the Music Instrument Shield
// This makes for more fun sounds than the buzzer can do
// check out the tab "EXT_MIDI_TRIG" for more info
boolean midi = true; // turn on/off midi in toner fundtions
#include <SoftwareSerial.h>
SoftwareSerial mySerial(15, 14); // RX (not used), TX
byte note = 72; //whistle
byte bank = 0x78; //drums
byte resetMIDI = 16; //Tied to VS1053 Reset line
int notes[] = { // fun notes found in the drum bank. grouped in 4.
  65,66,67,68,
  //60,61,62,63, // 62 and 63 are sub sonic for most speakers, so changing them - PGL 3/2/2016
  60,61,76,77,
  78,79,80,81,
  86,57,38,51,
  56,58,71,74}; // Note, if you add more 4-note-groups, you must update variable at game start below
int start_note = -4; // start note, so we can quickly go to different groups in the array
                    // add 4 to this and you can skip to next 4-note-group
                    // 3/2/2016 PGL, changing this to "-4", so that we start at the beginning of array when we add 4 the first time.
int instrument = 0;         

#define PLAYBACK_SPEED 600
int playback_speed = PLAYBACK_SPEED; // 150 is standard, but slowing it down makes it easier.
                                      // Note, this is reset each tiem a new game starts, so it has to be an int set by a define
int attract_mode_speed = 150;

void setup()
{
  //Setup hardware inputs/outputs. These pins are defined in the hardware_versions header file

  // DUNK TANK TRIGGER
  pinMode(A3, OUTPUT);
  digitalWrite(A3, HIGH);

  //Enable pull ups on inputs
  pinMode(BUTTON_RED, INPUT_PULLUP);
  pinMode(BUTTON_GREEN, INPUT_PULLUP);
  pinMode(BUTTON_BLUE, INPUT_PULLUP);
  pinMode(BUTTON_YELLOW, INPUT_PULLUP);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);

  pinMode(BUZZER1, OUTPUT);
  pinMode(BUZZER2, OUTPUT);

  if(debug){
    Serial.begin(115200);
    Serial.println("Simon Says Debug ON"); 
  }
  
//      while(1){ // FOR DEBUG ONLY - CALIBRATING POTENTIOMETER INPUT
//    int input = analogRead(A5);
//    Serial.print("input=");
//    Serial.println(input);
//    delay(250);
//    }
    
    
  
  //Mode checking

  // Check to see if the lower right button is pressed
  if (checkButton() == CHOICE_YELLOW) play_beegees();

  // Check to see if upper right button is pressed
  else if (checkButton() == CHOICE_GREEN)
  {
    gameMode = MODE_BATTLE; //Put game into battle mode

    //Turn on the upper right (green) LED
    setLEDs(CHOICE_GREEN);
    toner(CHOICE_GREEN, 150);

    setLEDs(CHOICE_RED | CHOICE_BLUE | CHOICE_YELLOW); // Turn on the other LEDs until you release button

    while(checkButton() != CHOICE_NONE) ; // Wait for user to stop pressing button
    
    if(debug) Serial.println("Battle Mode entered"); 

    //Now do nothing. Battle mode will be serviced in the main routine
  }

  // Check to see if upper left button is pressed
  else if (checkButton() == CHOICE_RED)
  {
    gameMode = MODE_COMBO_BUTTON; //Put game into combo button mode

    //Turn on the RED LED
    setLEDs(CHOICE_RED);
    toner(CHOICE_RED, 150);

    setLEDs(CHOICE_GREEN | CHOICE_BLUE | CHOICE_YELLOW); // Turn on the other LEDs until you release button

    while(checkButton() != CHOICE_NONE) ; // Wait for user to stop pressing button
    
    if(debug) Serial.println("Combo Button Mode entered"); 
    
    //Now do nothing. Combo button mode will be serviced in the main routine
  }
  
  
  // midi setup
  if(midi){
      //Setup soft serial for MIDI control
    mySerial.begin(31250);
  
    //Reset the VS1053
    pinMode(resetMIDI, OUTPUT);
    digitalWrite(resetMIDI, LOW);
    delay(100);
    digitalWrite(resetMIDI, HIGH);
    delay(100);
  
    testmelody();
    bank = 0x78;// drums 
  }
  
  play_winner(); // After setup is complete, say hello to the world
}

void loop()
{
  attractMode(); // Blink lights while waiting for user to press a button
  playback_speed = PLAYBACK_SPEED;
  RED_seq_count = 0;
  GREEN_seq_count = 0;
  BLUE_seq_count = 0;
  YELLOW_seq_count = 0;

  // Indicate the start of game play
  setLEDs(CHOICE_RED | CHOICE_GREEN | CHOICE_BLUE | CHOICE_YELLOW); // Turn all LEDs on
  delay(1000);
  setLEDs(CHOICE_OFF); // Turn off LEDs
  if(trampoline) delay(1000);
  else delay(250);
  
  // cycling through each 4-note-group
  if(start_note < 16) start_note += 4;   // Note, you must adjust the "greater than number" when adding more groups.
  else start_note = 0; // reset

  // Real_time_mode_input is a boolean variable above, only use this if you have input potentiometer on A5
  if(real_time_mode_input){ // note, Combo mode not yet supported as of 6/6/2013
                            // I didn't include this cause it would be really hard on trampolines.
    
    int input = analogRead(A5);
    if(debug) Serial.print("input=");
    if(debug) Serial.println(input);

    if(input <= 1023) {
      ROUNDS_TO_WIN = 8;
      gameMode = MODE_MEMORY;
    }
    if(input < 800) {
      ROUNDS_TO_WIN = 10;
      gameMode = MODE_MEMORY;
    }
    if(input < 530) {
      gameMode = MODE_MUSICAL_INST;
    }
    if(input < 250) {
      gameMode = MODE_BATTLE;
    }
  }
  
    if(debug) Serial.print("rounds to win=");
    if(debug) Serial.println(ROUNDS_TO_WIN);
    if(debug) Serial.print("gameMode=");
    if(debug) Serial.println(gameMode);

  if (gameMode == MODE_MEMORY)
  {
    // Play memory game and handle result
    if (play_memory() == true){
      win_melody(); // midi sounds
      play_winner(); // Player won, play winner tones
    }
    else 
      play_loser(); // Player lost, play loser tones
  }

  if (gameMode == MODE_BATTLE)
  {
    play_battle(); // Play game until someone loses

    play_loser(); // Player lost, play loser tones
  }
  
  if (gameMode == MODE_COMBO_BUTTON)
  {
    // Play memory game in combo mode and handle result
    if (play_memory_combo() == true){
      play_winner(); // Player won, play winner tones
      win_melody(); // midi sounds
    }
    else 
      play_loser(); // Player lost, play loser tones
  }
  
  if (gameMode == MODE_MUSICAL_INST)
  {
    play_musical_inst(); // Play musical inst until a timeout occurs (3 seconds)
  }
  
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//The following functions are related to game play only

// Wait for a button to be pressed. 
// Returns one of LED colors (LED_RED, etc.) if successful, 0 if timed out
byte wait_for_button(byte currentMove = 0)
{
  long startTime = millis(); // Remember the time we started the this loop

  // reset trampoline variables, to avoid false jumps, (in case someone jumped during playback)
  // This will require a "fresh" jump after playback.
  HIGH_COUNTER_RED = 0;
  HIGH_COUNTER_GREEN = 0;
  HIGH_COUNTER_BLUE = 0;
  HIGH_COUNTER_YELLOW = 0;

  while ( (millis() - startTime) < ENTRY_TIME_LIMIT) // Loop until too much time has passed
  {
    byte button = checkButton();
    
    if(trampoline) button = checkButton_trampoline();

    if (button != CHOICE_NONE)
    { 
      toner(button, 150); // Play the button the user just pressed
      
      if(trampoline == false) while(checkButton() != CHOICE_NONE) ;  // Now let's wait for user to release button, works fine in standard playing with buttons.
      
      delay(250); // This helps with debouncing and accidental double taps

      return button;
    }

  }

  return CHOICE_NONE; // If we get here, we've timed out!
}


// Play the regular memory game
// Returns 0 if player loses, or 1 if player wins
boolean play_memory(void)
{
  randomSeed(millis()); // Seed the random generator with random amount of millis()

  gameRound = 0; // Reset the game to the beginning

  while (gameRound < ROUNDS_TO_WIN) 
  {
    add_to_moves(); // Add a button to the current moves, then play them back

    playMoves(); // Play back the current game board

    // Then require the player to repeat the sequence.
    for (byte currentMove = 0 ; currentMove < gameRound ; currentMove++)
    {
      
      byte choice = wait_for_button(); // See what button the user presses
      
      if (choice == 0) return false; // If wait timed out, player loses

      if (choice != gameBoard[currentMove]) return false; // If the choice is incorect, player loses
    }

    delay(2000); // Player was correct, delay before playing moves
  }

  return true; // Player made it through all the rounds to win!
}

// Play the special 2 player battle mode
// A player begins by pressing a button then handing it to the other player
// That player repeats the button and adds one, then passes back.
// This function returns when someone loses
boolean play_battle(void)
{
  gameRound = 0; // Reset the game frame back to one frame
  
  while (1) // Loop until someone fails 
  {
    byte newButton = wait_for_button(); // Wait for user to input next move
    gameBoard[gameRound++] = newButton; // Add this new button to the game array

    // Then require the player to repeat the sequence.
    for (byte currentMove = 0 ; currentMove < gameRound ; currentMove++)
    {
      byte choice = wait_for_button();

      if (choice == 0) return false; // If wait timed out, player loses.

      if (choice != gameBoard[currentMove]) return false; // If the choice is incorect, player loses.
    }

    delay(100); // Give the user an extra 100ms to hand the game to the other player
  }

  return true; // We should never get here
}

// Plays the current contents of the game moves
void playMoves(void)
{
  for (byte currentMove = 0 ; currentMove < gameRound ; currentMove++) 
  {
    // Pete Lewis 3/2/2016, decrease playback_speed as the sequence gets longer
    // To win you get to 13, so let's shoot for 1/2 of playback speed by the time you get to 13 steps,
    // this means we need to cut 1/2 of playback speed over 13 rounds
    int subtraction = playback_speed * 0.15;
    subtraction /= ROUNDS_TO_WIN; // 
    subtraction *= currentMove; // depending on where we are in the sequence, the larger this "subtraction" will be, making it faster.
    playback_speed -= subtraction;
    toner(gameBoard[currentMove], playback_speed);

    // Wait some amount of time between button playback
    // Shorten this to make game harder
    if(currentMove != (gameRound-1)) delay(playback_speed); // 150 works well. 75 gets fast.
                                                        // also note, if on the last one, don't delay,
                                                        // this allows the player to jump right into user input... no punn intended haha.
  }
}

// Adds a new random button to the game sequence
void add_to_moves(void)
{
  byte newButton = random(0, 4); //min (included), max (exluded)

  // ensure we don't repeat a button more than two times in a row - add to the while loop below, if you want more repeats possible.
  if(gameRound > 1) // only after you make it to step 3.
  {
      // attempt 1, works.
      // while((newButton == gameBoard[gameRound-1]) && (newButton == gameBoard[gameRound-2])) newButton = random(0, 4); // keep pulling in more variables until this isn't true.
      // basically, if it's the same as the previous 2 buttons, then it will keep finding a random number until it's "fresh".

      // attempt 2, attempting to add in limit per button to avoid leaving one jumper out.
    while(1)
    {
      newButton = random(0, 4);
      if(!((newButton == gameBoard[gameRound-1]))) // This ensures it's not going to repeat same button 3 times
      {
        // check to see if that button is already "maxed out" (aka it has been used twice already)
        if((newButton == 0) && (RED_seq_count == 2)); // do nothing
        else if((newButton == 1) && (GREEN_seq_count == 2)); // do nothing
        else if((newButton == 2) && (BLUE_seq_count == 2)); // do nothing
        else if((newButton == 3) && (YELLOW_seq_count == 2)); // do nothing
        else break; // get out of this while loop and continue playing. This means that the new button is good to go. 
      }    
    }
  }
  
  // We have to convert this number, 0 to 3, to CHOICEs
  if(newButton == 0) 
  {
    newButton = CHOICE_RED;
    RED_seq_count++;
  }
  else if(newButton == 1) 
  {
    newButton = CHOICE_GREEN;
    GREEN_seq_count++;
  }
  else if(newButton == 2) 
  {
    newButton = CHOICE_BLUE;
    BLUE_seq_count++;
  }
  else if(newButton == 3) 
  {
    newButton = CHOICE_YELLOW;
    YELLOW_seq_count++;
  }

  gameBoard[gameRound++] = newButton; // Add this new button to the game array
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//The following functions control the hardware

// Lights a given LEDs
// Pass in a byte that is made up from CHOICE_RED, CHOICE_YELLOW, etc
void setLEDs(byte leds)
{
  if ((leds & CHOICE_RED) != 0)
    digitalWrite(LED_RED, HIGH);
  else
    digitalWrite(LED_RED, LOW);

  if ((leds & CHOICE_GREEN) != 0)
    digitalWrite(LED_GREEN, HIGH);
  else
    digitalWrite(LED_GREEN, LOW);

  if ((leds & CHOICE_BLUE) != 0)
    digitalWrite(LED_BLUE, HIGH);
  else
    digitalWrite(LED_BLUE, LOW);

  if ((leds & CHOICE_YELLOW) != 0)
    digitalWrite(LED_YELLOW, HIGH);
  else
    digitalWrite(LED_YELLOW, LOW);
}



// play_musical_inst()
// This mode allows the user to simply press buttons and see LED light up and hear the sound
// if it times out, then it returns false and this will
// take you back to the very beginning of the main loop.

void play_musical_inst(void)
{
  long startTime = millis(); // Remember the time we started the this loop

  while ( (millis() - startTime) < ENTRY_TIME_LIMIT) // Loop until too much time has passed
  {
    byte button = checkButton_trampoline(); // alwasy do trampoline, even if playing with buttons
                                            // This allows us to press the next sound without releasing
                                            // the previous button.
                                            // This makes it more like a traditional keyboard.

    if (button != CHOICE_NONE)
    { 
      toner(button, 1); // Play the button the user just pressed
      
      //delay(10); // This helps with debouncing and accidental double taps

      startTime = millis();
    }
  }
// If we get here, we've timed out!
}

// Returns a '1' bit in the position corresponding to CHOICE_RED, CHOICE_GREEN, etc.
byte checkButton(void)
{
  if (digitalRead(BUTTON_RED) == 0) return(CHOICE_RED); 
  else if (digitalRead(BUTTON_GREEN) == 0) return(CHOICE_GREEN); 
  else if (digitalRead(BUTTON_BLUE) == 0) return(CHOICE_BLUE); 
  else if (digitalRead(BUTTON_YELLOW) == 0) return(CHOICE_YELLOW);
  
  return(CHOICE_NONE); // If no button is pressed, return none
}

// Returns a '1' bit in the position corresponding to CHOICE_RED, CHOICE_GREEN, etc.
// but the FLAG of that button must have been set to
byte checkButton_trampoline(void)
{
  /////////////////RED
  if ((digitalRead(BUTTON_RED) == 0) && (HIGH_COUNTER_RED > 100)) // if FLAG_RED is set to true, that means that it has been release at some point previously.
  {
    HIGH_COUNTER_RED = 0; // reset counter
    //delay(500);
    return(CHOICE_RED);
  }

  /////////////////GREEN
  if ((digitalRead(BUTTON_GREEN) == 0) && (HIGH_COUNTER_GREEN > 100)) // if FLAG_RED is set to true, that means that it has been release at some point previously.
  {
    HIGH_COUNTER_GREEN = 0; // reset counter
    //delay(500);
    return(CHOICE_GREEN);
  }

  /////////////////BLUE
  if ((digitalRead(BUTTON_BLUE) == 0) && (HIGH_COUNTER_BLUE > 100)) // if FLAG_RED is set to true, that means that it has been release at some point previously.
  {
    HIGH_COUNTER_BLUE = 0; // reset counter
    //delay(100);
    return(CHOICE_BLUE);
  }

  /////////////////YELLOW
  if ((digitalRead(BUTTON_YELLOW) == 0) && (HIGH_COUNTER_YELLOW > 100)) // if FLAG_RED is set to true, that means that it has been release at some point previously.
  {
    HIGH_COUNTER_YELLOW = 0; // reset counter
    //delay(100);
    return(CHOICE_YELLOW);
  }
  
  if((digitalRead(BUTTON_RED) == 1) && (HIGH_COUNTER_RED < 101)) HIGH_COUNTER_RED++;
  if((digitalRead(BUTTON_GREEN) == 1) && (HIGH_COUNTER_GREEN < 101)) HIGH_COUNTER_GREEN++;
  if((digitalRead(BUTTON_YELLOW) == 1) && (HIGH_COUNTER_YELLOW < 101)) HIGH_COUNTER_YELLOW++;
  if((digitalRead(BUTTON_BLUE) == 1) && (HIGH_COUNTER_BLUE < 101)) HIGH_COUNTER_BLUE++;
  
  return(CHOICE_NONE); // If no button is pressed, return none
}

// Light an LED and play tone
// Red, upper left:     440Hz - 2.272ms - 1.136ms pulse
// Green, upper right:  880Hz - 1.136ms - 0.568ms pulse
// Blue, lower left:    587.33Hz - 1.702ms - 0.851ms pulse
// Yellow, lower right: 784Hz - 1.276ms - 0.638ms pulse
void toner(byte which, int buzz_length_ms)
{
  setLEDs(which); //Turn on a given LED

  //Play the sound associated with the given LED
  switch(which) 
  {
  case CHOICE_RED:
    if(midi){
      talkMIDI(0xB0, 0, bank); //Bank select drums
      talkMIDI(0xC0, instrument, 0x20); //Set instrument number. 0xC0 is a 1 data byte command
      noteOn(0, notes[start_note], 127); // start note, so we can quickly go to different groups in the array
    }
    buzz_sound(buzz_length_ms, 1136); 
    break;
  case CHOICE_GREEN:
    if(midi){
      talkMIDI(0xB0, 0, bank); //Bank select drums
      talkMIDI(0xC0, instrument, 0x20); //Set instrument number. 0xC0 is a 1 data byte command
      noteOn(0, notes[start_note+1], 127); // start_note+1 to get the next spot in 4-note-group
    }
    buzz_sound(buzz_length_ms, 568); 
    break;
  case CHOICE_BLUE:
    if(midi){
      talkMIDI(0xB0, 0, bank); //Bank select drums
      talkMIDI(0xC0, instrument, 0x20); //Set instrument number. 0xC0 is a 1 data byte command
      noteOn(0, notes[start_note+2], 127); // start_note+2 to get the next spot in 4-note-group
    }
    buzz_sound(buzz_length_ms, 851); 
    break;
  case CHOICE_YELLOW:
    if(midi){
      talkMIDI(0xB0, 0, bank); //Bank select drums
      talkMIDI(0xC0, instrument, 0x20); //Set instrument number. 0xC0 is a 1 data byte command
      noteOn(0, notes[start_note+3], 127); // start_note+3 to get the next spot in 4-note-group
    }
    buzz_sound(buzz_length_ms, 638); 
    break;
  }

  setLEDs(CHOICE_OFF); // Turn off all LEDs
}

// Toggle buzzer every buzz_delay_us, for a duration of buzz_length_ms.
void buzz_sound(int buzz_length_ms, int buzz_delay_us)
{
  // Convert total play time from milliseconds to microseconds
  long buzz_length_us = buzz_length_ms * (long)1000;

  // Loop until the remaining play time is less than a single buzz_delay_us
  while (buzz_length_us > (buzz_delay_us * 2))
  {
    buzz_length_us -= buzz_delay_us * 2; //Decrease the remaining play time

    // Toggle the buzzer at various speeds
    digitalWrite(BUZZER1, LOW);
    digitalWrite(BUZZER2, HIGH);
    delayMicroseconds(buzz_delay_us);

    digitalWrite(BUZZER1, HIGH);
    digitalWrite(BUZZER2, LOW);
    delayMicroseconds(buzz_delay_us);
  }
}

// Play the winner sound and lights
void play_winner(void)
{
  setLEDs(CHOICE_GREEN | CHOICE_BLUE);
  winner_sound();
  setLEDs(CHOICE_RED | CHOICE_YELLOW);
  winner_sound();
  setLEDs(CHOICE_GREEN | CHOICE_BLUE);
  winner_sound();
  setLEDs(CHOICE_RED | CHOICE_YELLOW);
  winner_sound();
}

// Play the winner sound
// This is just a unique (annoying) sound we came up with, there is no magic to it
void winner_sound(void)
{
  // Toggle the buzzer at various speeds
  for (byte x = 250 ; x > 70 ; x--)
  {
    for (byte y = 0 ; y < 3 ; y++)
    {
      digitalWrite(BUZZER2, HIGH);
      digitalWrite(BUZZER1, LOW);
      delayMicroseconds(x);

      digitalWrite(BUZZER2, LOW);
      digitalWrite(BUZZER1, HIGH);
      delayMicroseconds(x);
    }
  }
}

// Play the loser sound/lights
void play_loser(void)
{
  setLEDs(CHOICE_RED | CHOICE_GREEN);
  talkMIDI(0xB0, 0, 0x78); //Bank select drums
  talkMIDI(0xC0, instrument, 0x20); //Set instrument number. 0xC0 is a 1 data byte command
  noteOn(0, 38, 127); // snaire
  delay(100);
  noteOn(0, 50, 127); // High Tom
  noteOn(0, 72, 127); // whistle  
  buzz_sound(255, 1500);
  
  setLEDs(CHOICE_BLUE | CHOICE_YELLOW);
  noteOn(0, 38, 127); // snaire
  delay(100);  
  noteOn(0, 45, 127); // Low Tom
  buzz_sound(255, 1500);

  setLEDs(CHOICE_RED | CHOICE_GREEN);
  noteOn(0, 38, 127); // snaire
  delay(100);  
  noteOn(0, 50, 127); // High Tom  
  buzz_sound(255, 1500);

  setLEDs(CHOICE_BLUE | CHOICE_YELLOW);
  noteOn(0, 38, 127); // snaire
  delay(100);  
  noteOn(0, 45, 127); // Low Tom  
  noteOn(0, 49, 127); // crash
  noteOn(0, 86, 127); // kick
  noteOn(0, 38, 127); // snaire
  noteOn(0, 58, 127); // clacker 
  buzz_sound(255, 1500);
  
}

// Show an "attract mode" display while waiting for user to press button.
void attractMode(void)
{
  int counter = 0;
  //if(trampoline) attract_mode_speed *= 10;
  while(1) 
  {
    if(Serial.available()) exe_serial_command();
    else{
      if(trampoline)
      {
        if (checkButton_trampoline() != CHOICE_NONE) return;
        if(counter == 0) setLEDs(CHOICE_RED);
        else if(counter == 2000) setLEDs(CHOICE_BLUE);
        else if(counter == 4000) setLEDs(CHOICE_GREEN);
        else if(counter == 6000) setLEDs(CHOICE_YELLOW);
        counter++;
        if(counter == 8000) counter = 0; // reset
      }
      else{
        setLEDs(CHOICE_RED);
        delay(attract_mode_speed);
        if (checkButton() != CHOICE_NONE) return;
    
        setLEDs(CHOICE_BLUE);
        delay(attract_mode_speed);
        if (checkButton() != CHOICE_NONE) return;
    
        setLEDs(CHOICE_GREEN);
        delay(attract_mode_speed);
        if (checkButton() != CHOICE_NONE) return;
    
        setLEDs(CHOICE_YELLOW);
        delay(attract_mode_speed);
        if (checkButton() != CHOICE_NONE) return;
      }
    }
  }
}


