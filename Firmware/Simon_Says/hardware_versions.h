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
#define BOARD_REV_2_3_2011 //Works with PTH board label '2-3-2011'
//#define BOARD_REV_4_9_2009   //Works with SMD simoon board label '4/9/2009' and '9/7/11'
//#define BOARD_REV_6_25_2008

#ifdef BOARD_REV_2_3_2011
  // LED pin definitions, these are Arduino pins, not ATmega pins
  #define LED_RED     10
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
  #define BUZZER2  7
#endif  // End definition for BOARD_REV_2_3_2011

#ifdef BOARD_REV_4_9_2009
  // LED pin definitions, these are Arduino pins, not ATmega pins
  #define LED_BLUE     13 //PORTB.5
  #define LED_YELLOW   5 //PORTD.5
  #define LED_RED      10 //PORTB.2
  #define LED_GREEN    2 //PORTD.2
  
  // Button pin definitions
  #define BUTTON_RED     8 //PINB.0
  #define BUTTON_GREEN   9 //PINB.1
  #define BUTTON_BLUE    7 //PIND.7
  #define BUTTON_YELLOW  6 //PIND.6
  
  // Buzzer pin definitions
  #define BUZZER1  3 //PORTD.3
  #define BUZZER2  4 //PORTD.4
#endif  // End define for BOARD_REV_4_9_2009

#ifdef BOARD_REV_6_25_2008
  // LED pin definitions, these are Arduino pins, not ATmega pins
  #define LED_RED     A3 //PORTC.3
  #define LED_GREEN   2 //PORTD.2
  #define LED_BLUE    A0 //PORTC.0
  #define LED_YELLOW  5 //PORTD.5
  
  // Button pin definitions
  #define BUTTON_RED     A2 //PINC.2
  #define BUTTON_GREEN   A5 //PINC.5
  #define BUTTON_BLUE    A1 //PINC.1
  #define BUTTON_YELLOW  6 //PIND.6
  
  // Buzzer pin definitions
  #define BUZZER1 3 //PORTD.3
  #define BUZZER2 4 //PORTD.4
#endif  // End define for BOARD_REV_6_25_2008

