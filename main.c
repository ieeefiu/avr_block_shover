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
#include <util/delay.h>

#include "USART.h"
#include "i2c_master.h"
#include "blockshover.h"

// constants for servo controls
#define PULSE_MIN 1000
#define PULSE_MAX 2000
#define PULSE_MID 1500

void menu(void);

volatile uint8_t shovecolor = NONE;
volatile uint8_t received;

static inline void initTimer1Servo(void);

int main(void)
{
	uint8_t i;
	uint16_t values[4];
	uint8_t sensors[SENSOR_NUMBER];
	initUSART();
	initTimer1Servo();
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
		printString("Channel: ");
		printByte(mux_get());
		printString("\n");
		sensor_init();
	}
	mux_select(0);
	printString("~All sensors initialized~\n\n");
	
	while(1) {
		menu();
		shovecolor = NONE;
		sleep_mode();
		for(i = 0; i < SENSOR_NUMBER; i++) {
			sensor_get(i, values);
			printString("Channel: ");
			printByte(mux_get());
			printString("\n");
			sensor_printvalues(values);
			check_color(i, values, sensors);
		}
		shove(shovecolor, sensors);
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

static inline void initTimer1Servo(void)
{
	// graciously borrowed from Make: AVR Programming
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1 << WGM13);
	TCCR1B |= (1 << CS10);
	TCCR1A |= (1 << COM1A1);
	ICR1 = 20000;
}
