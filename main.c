/* Block Shover 5000
 * 
 * A silly little program for the ATmega328p for switching channels on an
 * i2c multiplexer for the purpose of reading from multiple i2c color
 * sensors with the same address.
 *
 * Near-future goal: identify colors on blocks
 * Eventual goal: use color data to operate servos to shove blocks
 *
 * The Players:
 * PCA9547 - An eight-channel i2c multiplexer
 * Two (2) TCS34725 - i2c color sensor
 * ATmega328p - A microcontroller
 *
 * Uses the i2c master library from github user devthrash
 * https://github.com/devthrash/I2C-master-lib
 *
 * Uses the USART library for serial communication by Elliot Williams
 * https://github.com/hexagon5un/AVR-Programming/
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "USART.h"
#include "blockshover.h"

void menu(void);

enum menuitems {DISPLAY, RED, GREEN, YELLOW, BLUE} selection;

void shove(uint8_t col, uint16_t* val);

volatile uint8_t received;

int main(void)
{
	uint8_t i;
	uint16_t values[4];
	initUSART();
	UCSR0B |= (1 << RXCIE0);
	sei(); // enable interrupts
	set_sleep_mode(SLEEP_MODE_IDLE);
	
	printString("|------------------------|\n"
				"| Block Shover 5000      |\n"
				"| By William Oberndorfer |\n"
				"|------------------------|\n\n");
	i2c_init();
	mux_init();

	printString("Initializing sensors\n"
				"--------------------\n");
	for (i = 0; i < SENSOR_NUMBER; i++) {
		mux_select(i);
		sensor_init();
	}
	printString("~All sensors initialized~\n\n");

	mux_select(0);
	
	while(1) {
		menu();
		sleep_mode();
		for(i = 0; i < SENSOR_NUMBER; i++) {
			sensor_get(i, values);
			sensor_printvalues(values);
		}
	}
}

ISR(USART_RX_vect)
{
	received = UDR0;
	switch(received) {
	case 0x31:
		selection = RED;
		break;
	case 0x32:
		selection = GREEN;
		break;
	case 0x33:
		selection = YELLOW;
		break;
	case 0x34:
		selection = BLUE;
		break;
	case 0x30:
		selection = DISPLAY;
		break;
	default:
		break;
	}
}

void menu(void)
{
		printString("Awaiting input. Valid inputs are numbers 1-4, which\n"
					"correspond to the following commands:\n\n"
					"1. Shove Red\n"
					"2. Shove Green\n"
					"3. Shove Yellow\n"
					"4. Shove Blue\n\n"
					"0. Display colors\n");
}

void shove(uint8_t col, uint16_t* val)
{
	switch(col) {
	case RED:
		printString("shove red\n");
		break;
	case GREEN:
		printString("shove green\n");
		break;
	case YELLOW:
		printString("shove yellow\n");
		break;
	case BLUE:
		printString("shove blue\n");
		break;
	case DISPLAY:
		break;
	}
}
