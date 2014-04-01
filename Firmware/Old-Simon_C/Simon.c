/**
 * 6-19-2007
 * Copyright 2009, Spark Fun Electronics
 * Nathan Seidle
 * nathan at sparkfun.com
 *
 * Released under the Creative Commons Attribution Share-Alike 3.0 License
 * http://creativecommons.org/licenses/by-sa/3.0
 *
 * Simon Game ported for the ATmega168
 *
 * Fixes and cleanup by Joshua Neal <joshua[at]trochotron.com>
 *
 * Generates random sequence, plays music, and displays button lights.
 *
 * Simon tones from Wikipedia
 * - A (red, upper left) - 440Hz - 2.272ms - 1.136ms pulse
 * - a (green, upper right, an octave higher than A) - 880Hz - 1.136ms,
 *   0.568ms pulse
 * - D (blue, lower left, a perfect fourth higher than the upper left) 
 *   587.33Hz - 1.702ms - 0.851ms pulse
 *   G (yellow, lower right, a perfect fourth higher than the lower left) - 
 *   784Hz - 1.276ms - 0.638ms pulse
 *
 * The tones are close, but probably off a bit, but they sound all right.
 *   
 * The old version of SparkFun simon used an ATmega8. An ATmega8 ships
 * with a default internal 1MHz oscillator.  You will need to set the
 * internal fuses to operate at the correct external 16MHz oscillator.
 *
 * Original Fuses:
 * avrdude -p atmega8 -P lpt1 -c stk200 -U lfuse:w:0xE1:m -U hfuse:w:0xD9:m
 *
 * Command to set to fuses to use external 16MHz: 
 * avrdude -p atmega8 -P lpt1 -c stk200 -U lfuse:w:0xEE:m -U hfuse:w:0xC9:m
 *
 * The current version of Simon uses the ATmega168. The external osciallator
 * was removed to reduce component count.  This version of simon relies on the
 * internal default 1MHz osciallator. Do not set the external fuses.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

/* Uncomment one of the following, corresponding to the board you have. */
//#define BOARD_REV_6_25_08
//#define BOARD_REV_4_9_2009
//#define BOARD_REV_6_3_2009
#define BOARD_REV_PTH

#ifdef BOARD_REV_PTH

#define CHIP_ATMEGA168

#define LED_RED		(1 << 0)
#define LED_GREEN	(1 << 1)
#define LED_BLUE	(1 << 2)
#define LED_YELLOW	(1 << 3)

/* LED pin definitions */
#define LED_RED_PIN			2
#define LED_RED_PORT		PORTB
#define LED_GREEN_PIN		3
#define LED_GREEN_PORT		PORTD
#define LED_BLUE_PIN		5
#define LED_BLUE_PORT		PORTB
#define LED_YELLOW_PIN		5
#define LED_YELLOW_PORT		PORTD

/* Button pin definitions */
#define BUTTON_RED_PIN		1
#define	BUTTON_RED_PORT		PINB
#define BUTTON_GREEN_PIN	2
#define	BUTTON_GREEN_PORT	PIND
#define BUTTON_BLUE_PIN		4
#define	BUTTON_BLUE_PORT	PINB
#define BUTTON_YELLOW_PIN	6
#define	BUTTON_YELLOW_PORT	PIND

/* Buzzer pin definitions */
#define BUZZER1		4
#define BUZZER1_PORT	PORTD
#define BUZZER2		7
#define BUZZER2_PORT	PORTD

#endif  /* BOARD_REV_PTH */


#ifdef BOARD_REV_6_25_08

#define CHIP_ATMEGA168

#define LED_RED		(1 << 0)
#define LED_GREEN	(1 << 1)
#define LED_BLUE	(1 << 2)
#define LED_YELLOW	(1 << 3)

/* LED pin definitions */
#define LED_RED_PIN		3
#define LED_RED_PORT		PORTC
#define LED_GREEN_PIN		2
#define LED_GREEN_PORT		PORTD
#define LED_BLUE_PIN		0
#define LED_BLUE_PORT		PORTC
#define LED_YELLOW_PIN		5
#define LED_YELLOW_PORT		PORTD

/* Button pin definitions */
#define BUTTON_RED_PIN		2
#define	BUTTON_RED_PORT		PINC
#define BUTTON_GREEN_PIN	5
#define	BUTTON_GREEN_PORT	PINC
#define BUTTON_BLUE_PIN		1
#define	BUTTON_BLUE_PORT	PINC
#define BUTTON_YELLOW_PIN	6
#define	BUTTON_YELLOW_PORT	PIND

/* Buzzer pin definitions */
#define BUZZER1		3
#define BUZZER1_PORT	PORTD
#define BUZZER2		4
#define BUZZER2_PORT	PORTD

#endif  /* BOARD_REV_6_25_08 */


#ifdef BOARD_REV_4_9_2009

#define CHIP_ATMEGA168

/* LED pin definitions */
#define LED_BLUE_PIN		5
#define LED_BLUE_PORT		PORTB
#define LED_YELLOW_PIN		5
#define LED_YELLOW_PORT		PORTD
#define LED_RED_PIN		2
#define LED_RED_PORT		PORTB
#define LED_GREEN_PIN		2
#define LED_GREEN_PORT		PORTD

/* Button pin definitions */
#define BUTTON_RED_PIN		0
#define	BUTTON_RED_PORT		PINB
#define BUTTON_GREEN_PIN	1
#define	BUTTON_GREEN_PORT	PINB
#define BUTTON_BLUE_PIN		7
#define	BUTTON_BLUE_PORT	PIND
#define BUTTON_YELLOW_PIN	6
#define	BUTTON_YELLOW_PORT	PIND

/* Buzzer pin definitions */
#define BUZZER1		3
#define BUZZER1_PORT	PORTD
#define BUZZER2		4
#define BUZZER2_PORT	PORTD

#endif  /* BOARD_REV_4_9_2009 */

#ifdef BOARD_REV_6_3_2009

#define CHIP_ATMEGA168

#define LED_RED		(1 << 0)
#define LED_GREEN	(1 << 1)
#define LED_BLUE	(1 << 2)
#define LED_YELLOW	(1 << 3)

/* LED pin definitions */
#define LED_RED_PIN			2
#define LED_RED_PORT		PORTB
#define LED_GREEN_PIN		2
#define LED_GREEN_PORT		PORTD
#define LED_BLUE_PIN		5
#define LED_BLUE_PORT		PORTB
#define LED_YELLOW_PIN		5
#define LED_YELLOW_PORT		PORTD

/* Button pin definitions */
#define BUTTON_RED_PIN		0
#define	BUTTON_RED_PORT		PINB
#define BUTTON_GREEN_PIN	1
#define	BUTTON_GREEN_PORT	PINB
#define BUTTON_BLUE_PIN		7
#define	BUTTON_BLUE_PORT	PIND
#define BUTTON_YELLOW_PIN	6
#define	BUTTON_YELLOW_PORT	PIND

/* Buzzer pin definitions */
#define BUZZER1		3
#define BUZZER1_PORT	PORTD
#define BUZZER2		4
#define BUZZER2_PORT	PORTD

#endif  /* BOARD_REV_6_3_2009 */

/* Define game parameters */
#define MOVES_TO_WIN	14
#define TIME_LIMIT	3000		/* 3000ms = 3 sec */

#define sbi(port_name, pin_number)   (port_name |= 1<<pin_number)
#define cbi(port_name, pin_number)   \
		((port_name) &= (uint8_t)~(1 << pin_number))

/* Declarations for static functions */
static void delay_us(uint16_t delay); 
static void delay_ms(uint16_t delay);
static uint8_t check_button(void);
static void set_leds(uint8_t leds);
static void buzz_sound(uint16_t buzz_length_ms, uint16_t buzz_delay);
static void toner(uint8_t tone, uint16_t buzz_length_ms);
static void add_to_moves(void);
static void play_moves(void);
static void play_loser(void);
static void play_winner(void);
static void ioinit(void);      

/* Game state */
uint8_t moves[32];
uint8_t nmoves = 0;

ISR (SIG_OVERFLOW2) 
{
	/* 
	 * Prescalar of 1024
	 * Clock = 16MHz
	 * 15,625 clicks per second
	 * 64us per click	
	 */

	/* Preload timer 2 for 125 clicks. Should be 8ms per ISR call */
	TCNT2 = 131; 		/* 256 - 125 = 131 */
}

/* General short delays, using internal timer do a fairly accurate 1us */
#ifdef CHIP_ATMEGA168
static void delay_us(uint16_t delay)
{
	while (delay > 256)
	{
		TIFR0 = (1<<TOV0); /* Clear any interrupt flags on Timer0 */
		TCNT0 = 0; 
		while ( (TIFR0 & (1<<TOV0)) == 0);
		
		delay -= 256;
	}

	TIFR0 = (1<<TOV0); 	/* Clear any interrupt flags on Timer0 */
	/* 
	 * 256 - 125 = 131 : Preload timer 0 for x clicks.
	 * Should be 1us per click
	 */
	TCNT0 = 256 - delay; 
	while ((TIFR0 & (1<<TOV0)) == 0) {
		/* do nothing */
	}
}
#endif

/* General short delays */
void delay_ms(uint16_t x)
{
	while (x-- > 0) {
		delay_us(1000);
	}
}

/* Light the given set of LEDs */
static void set_leds(uint8_t leds)
{
	if ((leds & LED_RED) != 0) {
		sbi(LED_RED_PORT, LED_RED_PIN);
	} else {
		cbi(LED_RED_PORT, LED_RED_PIN);
	}
	if ((leds & LED_GREEN) != 0) {
		sbi(LED_GREEN_PORT, LED_GREEN_PIN);
	} else {
		cbi(LED_GREEN_PORT, LED_GREEN_PIN);
	}
	if ((leds & LED_BLUE) != 0) {
		sbi(LED_BLUE_PORT, LED_BLUE_PIN);
	} else {
		cbi(LED_BLUE_PORT, LED_BLUE_PIN);
	}
	if ((leds & LED_YELLOW) != 0) {
		sbi(LED_YELLOW_PORT, LED_YELLOW_PIN);
	} else {
		cbi(LED_YELLOW_PORT, LED_YELLOW_PIN);
	}
}


#ifdef BOARD_REV_6_25_08
static void init_gpio(void)
{
	/* 1 = output, 0 = input */
	DDRB = 0b11111111; 
	DDRC = 0b00001001; /* LEDs and Buttons */
	DDRD = 0b00111110; /* LEDs, buttons, buzzer, TX/RX */

	PORTC = 0b00100110; /* Enable pull-ups on buttons 0,2,3 */
	PORTD = 0b01000000; /* Enable pull-up on button 1 */
}
#endif  /* BOARD_REV_6_25_08 */

#ifdef BOARD_REV_4_9_2009
static void init_gpio(void)
{
	/* 1 = output, 0 = input */
	DDRB = 0b11111100; /* button 2,3 on PB0,1 */
	DDRD = 0b00111110; /* LEDs, buttons, buzzer, TX/RX */

	PORTB = 0b00000011; /* Enable pull-ups on buttons 2,3 */
	PORTD = 0b11000000; /* Enable pull-up on button 0,1 */
}
#endif  /* BOARD_REV_4_9_2009 */

#ifdef BOARD_REV_PTH
static void init_gpio(void)
{
	/* 1 = output, 0 = input */
	DDRB = 0xFF & ~(1<<BUTTON_BLUE_PIN) & ~(1<<BUTTON_RED_PIN); // Buttons are inputs, LEDs outputs
	DDRC = 0x00; //All pins are open GPIOs
	DDRD = 0xFF & ~(1<<BUTTON_GREEN_PIN) & ~(1<<BUTTON_YELLOW_PIN); // 

	//DDRB = 0b11111100; /* button 2,3 on PB0,1 */
	//DDRD = 0b00111110; /* LEDs, buttons, buzzer, TX/RX */

	PORTB = (1<<BUTTON_BLUE_PIN) | (1<<BUTTON_RED_PIN); // Enable pull-ups on button
	PORTC = 0xFF;
	PORTD = (1<<BUTTON_GREEN_PIN) | (1<<BUTTON_YELLOW_PIN); // Enable pull-up on button
}
#endif  /* BOARD_REV_PTH */

#ifdef BOARD_REV_6_3_2009
static void init_gpio(void)
{
	/* 1 = output, 0 = input */
	DDRB = 0xFF & ~(1<<BUTTON_GREEN_PIN) & ~(1<<BUTTON_RED_PIN); // Buttons are inputs, LEDs outputs
	DDRC = 0x00; //All pins are open GPIOs
	DDRD = 0xFF & ~(1<<BUTTON_BLUE_PIN) & ~(1<<BUTTON_YELLOW_PIN); // 

	PORTB = (1<<BUTTON_GREEN_PIN) | (1<<BUTTON_RED_PIN); // Enable pull-ups on button
	PORTC = 0xFF;
	PORTD = (1<<BUTTON_BLUE_PIN) | (1<<BUTTON_YELLOW_PIN); // Enable pull-up on button
}
#endif  /* BOARD_REV_6_3_2009 */


void ioinit(void)
{
	init_gpio();	

	/* Init timer 0 for delay_us timing (1,000,000 / 1 = 1,000,000) */
    //TCCR0B = (1<<CS00); //Set Prescaler to No Prescaling. 1clk = 1us (assume we are running at internal 1MHz). CS00=1 
    TCCR0B = (1<<CS01); //Set Prescaler to clk/8 : 1click = 1us(assume we are running at internal 8MHz). CS01=1 
	//TCCR0B = (1<<CS00); /* Set Prescaler to 1. CS00=1 */

	/* Init timer 2 */
	ASSR = 0;
	/* Set Prescaler to 1024. CS22=1, CS21=1,CS20=1 */
	TCCR2B = (1<<CS22)|(1<<CS21)|(1<<CS20); 
	TIMSK2 = (1<<TOIE2); /* Enable Timer 2 Interrupt */
    
	sei();
}

/* Returns a '1' bit in the position corresponding to LED_RED, etc. */
uint8_t check_button(void)
{
	uint8_t button_pressed = 0;
	
	if ((BUTTON_RED_PORT & (1 << BUTTON_RED_PIN)) == 0)
		button_pressed |= LED_RED; 
	if ((BUTTON_GREEN_PORT & (1 << BUTTON_GREEN_PIN)) == 0)
		button_pressed |= LED_GREEN; 
	if ((BUTTON_BLUE_PORT & (1 << BUTTON_BLUE_PIN)) == 0)
		button_pressed |= LED_BLUE; 
	if ((BUTTON_YELLOW_PORT & (1 << BUTTON_YELLOW_PIN)) == 0)
		button_pressed |= LED_YELLOW; 
	
	return button_pressed;
}

/* Play the loser sound/lights */
void play_loser(void)
{
	set_leds(LED_RED|LED_GREEN);
	buzz_sound(255, 1500);

	set_leds(LED_BLUE|LED_YELLOW);
	buzz_sound(255, 1500);

	set_leds(LED_RED|LED_GREEN);
	buzz_sound(255, 1500);

	set_leds(LED_BLUE|LED_YELLOW);
	buzz_sound(255, 1500);
}

/* Play the winner sound */
static void winner_sound(void)
{
	uint8_t x, y;

	/* Toggle the buzzer at various speeds */
	for (x = 250; x > 70; x--) {
		for (y = 0; y < 3; y++) {
			sbi(BUZZER2_PORT, BUZZER2);
			cbi(BUZZER1_PORT, BUZZER1);

			delay_us(x);

			cbi(BUZZER2_PORT, BUZZER2);
			sbi(BUZZER1_PORT, BUZZER1);

			delay_us(x);
		}
	}
}

/* Play the winner sound and lights */
void play_winner(void)
{
	set_leds(LED_GREEN|LED_BLUE);
	winner_sound();
	set_leds(LED_RED|LED_YELLOW);
	winner_sound();
	set_leds(LED_GREEN|LED_BLUE);
	winner_sound();
	set_leds(LED_RED|LED_YELLOW);
	winner_sound();
}

/* Plays the current contents of the game moves */
static void play_moves(void)
{
	uint8_t move;

	for (move = 0; move < nmoves; move++) {
		toner(moves[move], 150);
		delay_ms(150);
	}
}

/* Adds a new random button to the game sequence, by sampling the timer */
static void add_to_moves(void)
{
	uint8_t new_button;

	/* Use the lower 2 bits of the timer for the random value */
	new_button = 1 << (TCNT2 & 0x3);

	moves[nmoves++] = new_button;
}

/* Toggle buzzer every buzz_delay_us, for a duration of buzz_length_ms. */
static void buzz_sound(uint16_t buzz_length_ms, uint16_t buzz_delay_us)
{
	uint32_t buzz_length_us;

	buzz_length_us = buzz_length_ms * (uint32_t)1000;
	while (buzz_length_us > buzz_delay_us*2) {
		buzz_length_us -= buzz_delay_us*2;

		/* toggle the buzzer at various speeds */
		cbi(BUZZER1_PORT, BUZZER1);
		sbi(BUZZER2_PORT, BUZZER2);
		delay_us(buzz_delay_us);

		sbi(BUZZER1_PORT, BUZZER1);
		cbi(BUZZER2_PORT, BUZZER2);
		delay_us(buzz_delay_us);
	}
}


/*
 * Light an LED and play tone
 *
 * red, upper left:     440Hz - 2.272ms - 1.136ms pulse
 * green, upper right:  880Hz - 1.136ms - 0.568ms pulse
 * blue, lower left:    587.33Hz - 1.702ms - 0.851ms pulse
 * yellow, lower right: 784Hz - 1.276ms - 0.638ms pulse
 */
static void toner(uint8_t which, uint16_t buzz_length_ms)
{
	set_leds(which);
	switch (which) {
		case LED_RED:
			buzz_sound(buzz_length_ms, 1136); 
			break;

		case LED_GREEN:
			buzz_sound(buzz_length_ms, 568); 
			break;

		case LED_BLUE:
			buzz_sound(buzz_length_ms, 851); 
			break;

		case LED_YELLOW:
			buzz_sound(buzz_length_ms, 638); 
			break;
	}

	/* Turn off all LEDs */
	set_leds(0);
}

/* Show an "attract mode" display while waiting for user to press button. */
static void attract_mode(void)
{
	while (1) {
		set_leds(LED_RED);
		delay_ms(100);
		if (check_button() != 0x00)
			return;

		set_leds(LED_BLUE);
		delay_ms(100);
		if (check_button() != 0x00) 
			return;

		set_leds(LED_GREEN);
		delay_ms(100);
		if (check_button() != 0x00) 
			return;

		set_leds(LED_YELLOW);
		delay_ms(100);
		if (check_button() != 0x00) 
			return;
	}
}


/* Wait for a button to be pressed.  Returns one of led colors (LED_RED, etc.)
 * if successful, 0 if timed out */
static uint8_t wait_for_button(void)
{
	uint16_t time_limit = TIME_LIMIT;
	uint8_t released = 0;
	uint8_t old_button;

	while (time_limit > 0) {
		uint8_t button;

		/* Implement a small bit of debouncing */
		old_button = button;
		button = check_button();

		/* 
		 * Make sure we've seen the previous button
		 * released before accepting new buttons
		 */
		if (button == 0) 
			released = 1;
		if (button == old_button && released == 1) {
			/* Make sure just one button is pressed */
			if (button == LED_RED || 
					button == LED_BLUE ||
					button == LED_GREEN || 
					button == LED_YELLOW) {
				return button;
			}
		}

		delay_ms(1);

		time_limit--; 
	}
	return 0;	/* Timed out */
}



/* Play the game.   Returns 0 if player loses, or 1 if player wins. */
static int game_mode(void)
{
	nmoves = 0;
	while (nmoves < MOVES_TO_WIN) {
		uint8_t move;

		/* Add a button to the current moves, then play them back */
		add_to_moves(); 
		play_moves(); 

		/* Then require the player to repeat the sequence. */
		for (move = 0; move < nmoves; move++) {
			uint8_t choice = wait_for_button();

			/* If wait timed out, player loses. */
			if (choice == 0)
				return 0;

			toner(choice, 150); 

			/* If the choice is incorect, player loses. */
			if (choice != moves[move]) {
				return 0;
			}
		}

		/* Player was correct, delay before playing moves */
		delay_ms(1000);
	}

	/* player wins */
	return 1;
}

int main(void)
{

	/* Setup IO pins and defaults */
	ioinit(); 
	
	/* Main loop */
	while (1) {
		/* Wait for user to start game */
		attract_mode();

		/* Indicate the start of game play */
		set_leds(LED_RED|LED_GREEN|LED_BLUE|LED_YELLOW);
		delay_ms(1000);
		set_leds(0);
		delay_ms(250);

		if (game_mode() != 0) {
			/* Player won, play winner tones */
			play_winner(); 
		} else {
			/* Player lost, play loser tones */
			play_loser(); 
		}
	}

	return(0);
}

