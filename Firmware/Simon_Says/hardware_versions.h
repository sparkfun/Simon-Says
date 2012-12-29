/*
 Started: 12-26-2012
 Spark Fun Electronics

 The SparkFun Simon Says game has been through dozens of revisions over the years. This header
 file attempts to map and support all the different hardware versions.
*/

#define CHOICE_OFF      0 //Used to control LEDs
#define CHOICE_NONE     0 //Used to check buttons
#define CHOICE_RED	(1 << 0)
#define CHOICE_GREEN	(1 << 1)
#define CHOICE_BLUE	(1 << 2)
#define CHOICE_YELLOW	(1 << 3)


// Uncomment one of the following, corresponding to the board you have.
//#define BOARD_REV_6_25_08
//#define BOARD_REV_4_9_2009
#define BOARD_REV_PTH



#ifdef BOARD_REV_PTH

// LED pin definitions, these are Arduino pins, not ATmega pins
#define LED_RED	    10
#define LED_GREEN   3
#define LED_BLUE    13
#define LED_YELLOW  5

// Button pin definitions
#define BUTTON_RED    9
#define BUTTON_GREEN  2
#define BUTTON_BLUE   12
#define BUTTON_YELLOW 6

// Buzzer pin definitions
#define BUZZER1  4
#define BUZZER2	 7

#endif  // End definition for BOARD_REV_PTH

#ifdef BOARD_REV_6_25_08

// LED pin definitions
#define LED_RED_PIN		3
#define LED_RED_PORT		PORTC
#define LED_GREEN_PIN		2
#define LED_GREEN_PORT		PORTD
#define LED_BLUE_PIN		0
#define LED_BLUE_PORT		PORTC
#define LED_YELLOW_PIN		5
#define LED_YELLOW_PORT		PORTD

// Button pin definitions
#define BUTTON_RED_PIN		2
#define	BUTTON_RED_PORT		PINC
#define BUTTON_GREEN_PIN	5
#define	BUTTON_GREEN_PORT	PINC
#define BUTTON_BLUE_PIN		1
#define	BUTTON_BLUE_PORT	PINC
#define BUTTON_YELLOW_PIN	6
#define	BUTTON_YELLOW_PORT	PIND

// Buzzer pin definitions
#define BUZZER1		3
#define BUZZER1_PORT	PORTD
#define BUZZER2		4
#define BUZZER2_PORT	PORTD

#endif  // End define for BOARD_REV_6_25_08


#ifdef BOARD_REV_4_9_2009

// LED pin definitions
#define LED_BLUE_PIN		5
#define LED_BLUE_PORT		PORTB
#define LED_YELLOW_PIN		5
#define LED_YELLOW_PORT		PORTD
#define LED_RED_PIN		2
#define LED_RED_PORT		PORTB
#define LED_GREEN_PIN		2
#define LED_GREEN_PORT		PORTD

// Button pin definitions
#define BUTTON_RED_PIN		0
#define	BUTTON_RED_PORT		PINB
#define BUTTON_GREEN_PIN	1
#define	BUTTON_GREEN_PORT	PINB
#define BUTTON_BLUE_PIN		7
#define	BUTTON_BLUE_PORT	PIND
#define BUTTON_YELLOW_PIN	6
#define	BUTTON_YELLOW_PORT	PIND

// Buzzer pin definitions
#define BUZZER1		3
#define BUZZER1_PORT	PORTD
#define BUZZER2		4
#define BUZZER2_PORT	PORTD

#endif  // End define for BOARD_REV_4_9_2009


//The following functions are specific to different versions of the board

#ifdef BOARD_REV_6_25_08
void init_gpio(void)
{
  // 1 = output, 0 = input
  DDRB = 0b11111111; 
  DDRC = 0b00001001; // LEDs and Buttons
  DDRD = 0b00111110; // LEDs, buttons, buzzer, TX/RX

  PORTC = 0b00100110; // Enable pull-ups on buttons 0, 2, 3
  PORTD = 0b01000000; // Enable pull-up on button 1
}
#endif  // End BOARD_REV_6_25_08

#ifdef BOARD_REV_4_9_2009
void init_gpio(void)
{
  // 1 = output, 0 = input
  DDRB = 0b11111100; // Button 2,3 on PB0,1
  DDRD = 0b00111110; // LEDs, buttons, buzzer, TX/RX

  PORTB = 0b00000011; // Enable pull-ups on buttons 2, 3
  PORTD = 0b11000000; // Enable pull-up on button 0, 1
}
#endif // End BOARD_REV_4_9_2009
