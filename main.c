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
#include "i2c_master.h"
#include "blockshover.h"

void menu(void);

volatile uint8_t shovecolor = 0;

void shove(uint8_t col, uint8_t* values);

volatile uint8_t received;

int main(void)
{
	DDRB |= 0xFF;
	
	uint8_t i;
	uint16_t values[4];
	uint8_t sensors[SENSOR_NUMBER];
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
	mux_select(0);
	printString("~All sensors initialized~\n\n");
	
	while(1) {
		menu();
		sleep_mode();
		for(i = 0; i < SENSOR_NUMBER; i++) {
			sensor_get(i, values);
			sensor_printvalues(values);
			check_color(i, values, sensors);
		}

		// part of that same dumb test
		if(sensors[0] == GREEN) {
			PORTB |= (1 << GREEN_0);
			PORTB &= ~(1 << RED_0);
		}
		else if(sensors[0] == RED) {
			PORTB |= (1 << RED_0);
			PORTB &= ~(1 << GREEN_0);
		}
		else {
			PORTB &= ~(1 << GREEN_0);
			PORTB &= ~(1 << RED_0);
		}
		
		if(sensors[1] == GREEN) {
			PORTB |= (1 << GREEN_1);
			PORTB &= ~(1 << RED_1);
		}
		else if(sensors[1] == RED) {
			PORTB |= (1 << RED_1);
			PORTB &= ~(1 << GREEN_1);
		}
		else {
			PORTB &= ~(1 << GREEN_1);
			PORTB &= ~(1 << RED_1);
		}
		// boy that sure was some dumb bullshit
		
		shove(color, sensors);
	}
}

ISR(USART_RX_vect)
{
	cli();
	received = UDR0;
	switch(received) {
	case 0x31:
		shovecolor = RED;
		break;
	case 0x32:
		shovecolor = GREEN;
		break;
	case 0x33:
		shovecolor = YELLOW;
		break;
	case 0x34:
		shovecolor = BLUE;
		break;
	default:
		shovecolor = NONE;
		break;
	}
	sei();
}

void menu(void)
{
		printString("\nAwaiting input. Valid inputs are numbers 1-4, which\n"
					"correspond to the following commands:\n\n"
					"1. Shove Red\n"
					"2. Shove Green\n"
					"3. Shove Yellow\n"
					"4. Shove Blue\n\n");
}

void shove(uint8_t col, uint8_t* sensors)
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
	default:
		break;
	}
}
