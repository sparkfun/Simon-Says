
//***********************************************************
// Simon_Says_Wireless.ino
// Simon Says PTH Kit Wireless
// Written by Chris Taylor for SparkFun Electronics
// 1/29/13
//
// Released under the Beerware License:
// 	If you use this code to make something cool, and you ever
//  meet me, you owe me a beer.
//***********************************************************
// This code allows two Simon Says PTH boards to be used in
// wireless battle mode when connected to two XBee explorers 
// which house XBee modules. 
//
// In order to deliver power to the XBee module on the 
// explorer, you will need to solder a wire from the VCC via
// near the Analog outputs on the board to the VCC input of 
// the FTDI header (the via is the third pin in from the
// label "BLK". 
//***********************************************************
// Gameplay: 
// Power up both of the units, and they will play an intro 
// sequence with some beeps and boops, and then will go into
// "attract" mode where they both flash lights in sequence.
// The player that wants to go first presses a button. Both
// Devices will go dark and wait for the first player to 
// press a button. This is the first button in the sequence.
// Once player one presses a button, it will play the
// sequence back to the next player who must repeat it, and 
// then add another button. Play continues until a player 
// incorrectly repeats the sequence or takes too long to
// repeat it. The devices will play their respective "winner"
// and "loser" sequences, and then return to their initial 
// state. 
//***********************************************************
// Physical Description:
//
// The Simon device consists of four buttons with four LEDs.
// The LEDs and buttons are used in the code as follows:
//
// 	  _____________________________
//	 |              |              |
//	 |              |              |
//	 |              |              |
//	 |  Button/LED  |  Button/LED  |
//	 |       1      |      2       |
//	 |              |              |
//       |______________|______________|
//	 |              |              |
//	 |              |              |
//	 |  Button/LED  |  Button/LED  |
//	 |       3      |      4       |
//	 |              |              |
//	 |              |              |
//       |______________|______________|
//
//********************************************************** 

// Pin definitions for the buttons, LEDs, and buzzer
#define BUTTON1 9
#define BUTTON2 2
#define BUTTON3 12
#define BUTTON4 6

#define LED1 10
#define LED2 3
#define LED3 13
#define LED4 5

#define BUZZER1 4
#define BUZZER2 7

// Bitmasks
// These are 4-bit bitmasks used with bitwise logic to 
// communicate which button has been pressed or which LED 
// to light
#define LED1_MASK 1
#define LED2_MASK 2
#define LED3_MASK 4
#define LED4_MASK 8

#define BUTTON1_MASK 1
#define BUTTON2_MASK 2
#define BUTTON3_MASK 4
#define BUTTON4_MASK 8

// Game constants
#define MOVES_TO_WIN 20 // If the players can remember 20 
												// moves, they both win
#define TIME_LIMIT 3000 // Three second limit on a move

// Game variables
byte moves[MOVES_TO_WIN]; // Array of moves already made
byte nmoves = 0; // Number of moves already made
byte first_turn = 0; // Flag indicating whether or not this
										 // is the first turn of the game

void setup()
{
	// Initialize Serial (Wireless) port
  Serial.begin(9600);
   
	// Initialize pins for buttons, LEDs, and buzzer 
  pinMode(BUTTON1, INPUT);    // BUTTON1
  pinMode(BUTTON2, INPUT);    // BUTTON2
  pinMode(BUTTON3, INPUT);    // BUTTON3
  pinMode(BUTTON4, INPUT);    // BUTTON4
  pinMode(LED1, OUTPUT);  		// LED1
  pinMode(LED3, OUTPUT);  		// LED3
  pinMode(LED2, OUTPUT);   		// LED2
  pinMode(LED4, OUTPUT);   		// LED4
  pinMode(BUZZER1, OUTPUT);   // BUZZER1
  pinMode(BUZZER2, OUTPUT);   // BUZZER2
  
  // Enable pull-ups on buttons
	// Button I/Os will read LOW when pressed
  digitalWrite(BUTTON1, HIGH);
  digitalWrite(BUTTON3, HIGH);
  digitalWrite(BUTTON2, HIGH);
  digitalWrite(BUTTON4, HIGH);
}

void loop()
{
  play_winner(); // Intro sequence for startup
  
  // Wait in attract mode until a button is pressed
	// attract mode will return 1 if this player is the first
	// player to press a button
	first_turn = attract_mode(); 
  
  // Begin game: flash all LEDs, then go into game mode
  set_leds(LED1_MASK | LED2_MASK | LED3_MASK | LED4_MASK);
  delay(1000);
  set_leds(0);
  delay(250);
  
  // Play game and handle result
	// game_mode() function returns 1 for win, 0 for loss
  if(game_mode()) { play_winner(); }
  else { play_loser(); }
}


/**********************************************************************
game_mode

This function chooses the operations to be executed during a game. It
has two sections, one for if the players are on the first turn, and 
one for if the players have pressed a first move. It returns 1 for a
win and 0 for a loss.
**********************************************************************/
byte game_mode(void)
{
  nmoves = 0;
  
	// Loop while players have not exceeded the moves to win 
	// variable
  while(nmoves < MOVES_TO_WIN) 
  {
		// If this is the first turn, we don't need to wait for the
		// other player. 
    if(first_turn)
    {
      add_move();
      first_turn = 0;
    }
    else
    {
			// Wait for move from other player. If this receive function
			// returns 0, this is a win. 
      if(!receive_move()) { return 1; }
      // Wait a half second before playing back the moves
			delay(500); 
      play_moves();
			// Test the player, if test is failed, this is a loss.
      if(!test_moves()) { return 0; } 
			// Test is successful, add a new move and send it to the 
			// other player. 
      add_move();
    }
  }
  
  return 1;
}

/**********************************************************************
 Test that the player can remember the sequence.
 Return 1 for success and 0 for failure
**********************************************************************/
byte test_moves(void)
{
  byte move;
    
  for(move = 0; move < nmoves; move++)
  {
		// Wait for a button press
    byte choice = wait_for_button();
		// Light LED and play sound for chosen button
    toner(choice, 150);
    
		// Translate the button from its bitmask
    if(choice == BUTTON1_MASK) { choice = 0; }
    if(choice == BUTTON2_MASK) { choice = 1; }
    if(choice == BUTTON3_MASK) { choice = 2; }
    if(choice == BUTTON4_MASK) { choice = 3; }  
    
    // If the choice is incorrect, player loses
    if(choice != moves[move])
    {
      delay(100); // Wait for the button release
      Serial.print('l'); // Send 'l' to other player to indicate loss
      return 0;
    }
  }
	// Player was correct, return 1
  return 1;
}

/**********************************************************************
add_move

Wait for the user to press a button and add the move to the move array.
Then send the new move to the other user. 
**********************************************************************/
void add_move(void)
{
  byte choice = 0;
 
 	// Wait for the user to press a button
  choice = wait_for_button();

	// Light the LED and beep to indicate received press
  toner(choice, 150);

	// Translate the button from its bitmask
  if(choice == BUTTON1_MASK) { choice = 0; }
  if(choice == BUTTON2_MASK) { choice = 1; }
  if(choice == BUTTON3_MASK) { choice = 2; }
  if(choice == BUTTON4_MASK) { choice = 3; }
  
	moves[nmoves++] = choice; // Add the move to the list
  
	// Send the new move to the other player
	Serial.print(choice);
}

/**********************************************************************
receive_move
Wait for the other player to send the move over XBee. The player will
either send the char 0, 1, 2, or 3 for the corresponding button press,
or send the character 'l' for a 'l'oss, meaning that they were unable
to successfully repeat the sequence. This function returns 1 for a
move or 0 for a loss by the other player. 
**********************************************************************/
byte receive_move(void)
{
  byte choice = 0;
	// Wait for the other player to send a character
  while(1)
  {
    if(Serial.available())
    {
      choice = Serial.read(); // Read in the sent character
      if(choice == 'l') { return 0; } // Other player has lost, return 0
      else { // Sent char was a button press
        choice = choice - 48; // Convert char from ASCII to button value
        moves[nmoves++] = choice;  // Add to list of moves
        return 1;
      }
    }
  }
}

/*********************************************************************
toner()
Accepts a button press and a tone length in ms. It lights the LED of
the chosen button press and plays the respective buzzer tone for a 
time of buzz_length_ms.
**********************************************************************/
void toner(byte which, int buzz_length_ms)
{
	// Light the LED of the chosen button press
  set_leds(which);
	// Play the appropriate tone on the buzzer for the chosen button
  switch(which) 
  {
    case LED1_MASK:
      buzz_sound(buzz_length_ms, 1136);
      break;
    case LED2_MASK:
      buzz_sound(buzz_length_ms, 568);
      break;
    case LED3_MASK:
      buzz_sound(buzz_length_ms, 851);
      break;
    case LED4_MASK:
      buzz_sound(buzz_length_ms, 638);
      break;
  }
  
  // Turn off all LEDs
  set_leds(0);
}

/*********************************************************************
play_moves()
"Playback" the moves in the moves[] array. This function shows the 
player what moves to remember and press to continue the game. 
**********************************************************************/
void play_moves(void)
{
  byte move;
  
	// Play the appropriate button fo the next move in the array. 
  for(move = 0; move < nmoves; move++)
  {
    toner((1 << moves[move]), 150);
    delay(150);
  }
}

/*********************************************************************
attract_mode()
Play a sequence of flashing LEDs to attract a player. Keep playing 
until a button is pressed.
*********************************************************************/
byte attract_mode(void) 
{
  byte current_led = LED1_MASK; // Start blinking with LED1
  char received = 0;
  
  while(1) // Loop until a button is pressed (return a value to exit loop)
  {
    set_leds(current_led);
    delay(100);
		
		// Check if a button is pressed. Once a button is pressed, send
		// a 'c' character to the other player to indicate that a game has
		// started, then return 1 to start game
    if(check_button()) { Serial.print('c'); return 1; } // Return 1 if button is pressed. This means player is player 1
    
		// Check if the other player has sent a 'c'. This means a game has started 
		if(Serial.available()) 
    { 
      received = Serial.read();
      if(received == 'c') { return 0; } // Return 2 if we receive a 'c'hallenge from another player
                                        // This means player is player 2
    } 
    current_led = (current_led << 1) % 0x0F; // bit shift 1 > 2 > 4 > 8 > 1 > 2...
  } 
}

/*********************************************************************
play_winner()
Play a sequence of LEDs and a buzzer sound to indicate a win. 
*********************************************************************/
void play_winner(void)
{
  for(byte x = 0; x < 5; x++) // Repeat 5 times
  {
    set_leds(LED2_MASK | LED3_MASK); // Light LED2 and LED3
    winner_sound(); // Play a winner sound
    set_leds(LED1_MASK | LED4_MASK); // Light LED1 and LED4
    winner_sound(); 
  }
}

/*********************************************************************
play_loser()
Play a sequence of LEDs and a buzzer sound to indicate a loss. 
*********************************************************************/
void play_loser(void) 
{
  for(byte x = 0; x < 5; x++) // Repeat 5 times
  {
    set_leds(LED1_MASK | LED2_MASK); // Light LED1 and LED2
    buzz_sound(255, 1500); // Play a buzzer sound
    set_leds(LED3_MASK | LED4_MASK); // Light LED3 and LED4
    buzz_sound(255, 1500);
  }
}

/*********************************************************************
buzz_sound()
Play a buzz sound of length buzz_length_ms with delay of 
buzz_delay_us.
*********************************************************************/
void buzz_sound(int buzz_length_ms, int buzz_delay_us)
{
  long buzz_length_us;
  
	// Convert to microseconds for loop calculation
  buzz_length_us = buzz_length_ms * (long)1000;
  while(buzz_length_us > (buzz_delay_us * 2))
  {
    buzz_length_us -= (buzz_delay_us * 2);
    
		// Modulate buzzer lines
    digitalWrite(BUZZER1, LOW);
    digitalWrite(BUZZER2, HIGH);
    delayMicroseconds(buzz_delay_us);
    
    digitalWrite(BUZZER1, HIGH);
    digitalWrite(BUZZER2, LOW);
    delayMicroseconds(buzz_delay_us);
  }
}

/*********************************************************************
set_leds()
Light the chosen LED (0 to turn off all).
*********************************************************************/
void set_leds(uint8_t leds) 
{
  if(leds & LED1_MASK) { digitalWrite(LED1, HIGH); }
  else { digitalWrite(LED1, LOW); }
  
  if(leds & LED2_MASK) { digitalWrite(LED2, HIGH); }
  else { digitalWrite(LED2, LOW); }
  
  if(leds & LED3_MASK) { digitalWrite(LED3, HIGH); }
  else { digitalWrite(LED3, LOW); }
  
  if(leds & LED4_MASK) { digitalWrite(LED4, HIGH); }
  else { digitalWrite(LED4, LOW); }
}

/*********************************************************************
winner_sound()
Play a sequence of beeps to indicate a win
*********************************************************************/
void winner_sound(void)
{
  byte x, y;
  
  // Toggle the buzzer at various speeds
  for(x = 250; x > 70; x--) {
    for(y = 0; y < 3; y++) {
      digitalWrite(BUZZER2, HIGH);
      digitalWrite(BUZZER1, LOW);
      
      delayMicroseconds(x);
      
      digitalWrite(BUZZER2, LOW);
      digitalWrite(BUZZER1, HIGH);
      
      delayMicroseconds(x);
    }
  }
}


/*********************************************************************
check_button()
Read the button pin to detect if buttons are pressed. Return a bitmask
of the pressed buttons.
*********************************************************************/
byte check_button(void)
{
  byte button_pressed = 0;
  
  if(!digitalRead(BUTTON1)) { button_pressed |= BUTTON1_MASK; }
  if(!digitalRead(BUTTON2)) { button_pressed |= BUTTON2_MASK; }
  if(!digitalRead(BUTTON3)) { button_pressed |= BUTTON3_MASK; }
  if(!digitalRead(BUTTON4)) { button_pressed |= BUTTON4_MASK; }
  
  return button_pressed;
}


/*********************************************************************
wait_for_button()
Wait until the user presses a button. The amount of time to wait is 
set by the definition TIME_LIMIT. 
*********************************************************************/
byte wait_for_button(void)
{
  int timeout = 0;
  byte button = 0;
  
  while(timeout < TIME_LIMIT) // Loop until button is pressed or 
															// time limit is violated.
  {
    button = check_button();
    
    if(button)
    {
      delay(10); // Debounce      
      while(check_button()); // wait for release
      delay(10);
      // Make sure just one button is pressed
      if( button == BUTTON1_MASK ||
          button == BUTTON2_MASK ||
          button == BUTTON3_MASK ||
          button == BUTTON4_MASK)
      {
        return button;  
      }
    }
    delay(1);
    timeout++;
  }
  return 0; // Timed out
}

