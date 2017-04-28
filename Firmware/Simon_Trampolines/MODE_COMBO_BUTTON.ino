////////// COMBO CODE


// This file contains functions used in the combo button mode of the game.
// They are all very similar to the regular memory mode,
// But include slightly tweaked functions to accomodate the new pattern options
// of combination button presses.

// The unique functions are:
// play_memory_combo()
// add_to_moves_combo()
// playMoves_combo()
// wait_for_button_combo()
// checkButton_combo()
// toner_combo()
// setLEDs_combo()

byte current_button_choice = 0; // needed in both checkButton() and read_buttons(), so must be global variable

// Notes for when playing in combo mode. We need 15, so how about some C pentatonic scale notes...
int combo_buzz_notes[] = {
  NOTE_C4,
  NOTE_DS4,
  NOTE_F4,
  NOTE_G4,
  NOTE_AS4,
  NOTE_C5,
  NOTE_DS5,
  NOTE_F5,
  NOTE_G5,
  NOTE_AS5,
  NOTE_C6,
  NOTE_DS6,
  NOTE_F6,
  NOTE_G6,
  NOTE_C6};
  
 
// Play the memory game in combo button
// Returns 0 if player loses, or 1 if player wins
boolean play_memory_combo(void)
{
  randomSeed(millis()); // Seed the random generator with random amount of millis()
  gameRound = 0; // Reset the game to the beginning
  while (gameRound < ROUNDS_TO_WIN) 
  {
    add_to_moves_combo(); // Add a button to the current moves, then play them back
    playMoves_combo(); // Play back the current game board
    // Then require the player to repeat the sequence.
    for (byte currentMove = 0 ; currentMove < gameRound ; currentMove++)
    {
      byte choice = wait_for_button_combo(); // See what button the user presses
      if (choice == 0) return false; // If wait timed out, player loses
      if (choice != gameBoard[currentMove]) return false; // If the choice is incorect, player loses
    }
    delay(1000); // Player was correct, delay before playing moves
  }
  return true; // Player made it through all the rounds to win!
}

// Adds a new random button to the game sequence
void add_to_moves_combo(void)
{
  byte newButton = random(1, 16); //min (included), max (exluded)
  // new button will be a byte that represents the combo of buttons pressed. 
  // "0" is not included, because you need at least one button to represent part of the sequence
  gameBoard[gameRound++] = newButton; // Add this new button to the game array
  if(debug){
    Serial.print("newButton:");
    Serial.println(newButton); 
  }
}


// Plays the current contents of the game moves
void playMoves_combo(void)
{
  for (byte currentMove = 0 ; currentMove < gameRound ; currentMove++) 
  {
      if(debug){
        Serial.print(gameBoard[currentMove]); 
        Serial.print(", ");
      }
    toner_combo(gameBoard[currentMove], 150);

    // Wait some amount of time between button playback
    // Shorten this to make game harder
    delay(150); // 150 works well. 75 gets fast.
  }
}

// Wait for a button to be pressed. 
// Returns one of LED colors (LED_RED, etc.) if successful, 0 if timed out
byte wait_for_button_combo(void)
{
  long startTime = millis(); // Remember the time we started the this loop

  while ( (millis() - startTime) < ENTRY_TIME_LIMIT) // Loop until too much time has passed
  {
    byte button = checkButton_combo();

    if (button != CHOICE_NONE)
    { 
      toner_combo(button, 150); // Play the button the user just pressed
      
      while(checkButton_combo() != CHOICE_NONE) ;  // Now let's wait for user to release button
      
      delay(10); // This helps with debouncing and accidental double taps

      return button;
    }

  }

  return CHOICE_NONE; // If we get here, we've timed out!
}

// Returns a '1' bit in the position corresponding to CHOICE_RED, CHOICE_GREEN, etc.
byte checkButton_combo(void)
{
  current_button_choice = 0; // reset to get a new reading
  if (read_buttons()){
    delay(100); // debounce and give the user a little time to actually press more than one button at the EXACT same time
    read_buttons(); // read them again to see what combo they are pressing 
  }
  
  if(debug){
    Serial.print("current_button_choice:"); 
    Serial.println(current_button_choice);
  }
      
  return(current_button_choice);
}

byte read_buttons(void)
{
    // set the bits for each button pressed
  if (digitalRead(BUTTON_RED) == 0) current_button_choice |= (CHOICE_RED); 
  if (digitalRead(BUTTON_GREEN) == 0) current_button_choice |= (CHOICE_GREEN); 
  if (digitalRead(BUTTON_BLUE) == 0) current_button_choice |= (CHOICE_BLUE); 
  if (digitalRead(BUTTON_YELLOW) == 0) current_button_choice |= (CHOICE_YELLOW);
}

// Light an LED and play tone
// Red, upper left:     440Hz - 2.272ms - 1.136ms pulse
// Green, upper right:  880Hz - 1.136ms - 0.568ms pulse
// Blue, lower left:    587.33Hz - 1.702ms - 0.851ms pulse
// Yellow, lower right: 784Hz - 1.276ms - 0.638ms pulse
void toner_combo(byte which, int buzz_length_ms)
{
  setLEDs_combo(which); //Turn on a given LED
  
  buzz_sound(buzz_length_ms, combo_buzz_notes[which]); 
  
  setLEDs_combo(CHOICE_OFF); // Turn off all LEDs
}

// Lights a given LEDs
// Pass in a byte that is made up from CHOICE_RED, CHOICE_YELLOW, etc
void setLEDs_combo(byte leds)
{
  if ((leds & CHOICE_RED) > 0)
    digitalWrite(LED_RED, HIGH);
  else
    digitalWrite(LED_RED, LOW);

  if ((leds & CHOICE_GREEN) > 0)
    digitalWrite(LED_GREEN, HIGH);
  else
    digitalWrite(LED_GREEN, LOW);

  if ((leds & CHOICE_BLUE) > 0)
    digitalWrite(LED_BLUE, HIGH);
  else
    digitalWrite(LED_BLUE, LOW);

  if ((leds & CHOICE_YELLOW) > 0)
    digitalWrite(LED_YELLOW, HIGH);
  else
    digitalWrite(LED_YELLOW, LOW);
}
