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
#include <util/delay.h>
#include <avr/interrupt.h>

#include "i2c_master.h"
#include "USART.h"

// Addresses for reading and writing to the multiplexer and sensor
#define MUX_WRITE 0b11100000
#define MUX_READ 0b11100001

#define SENSOR_WRITE 0b01010010
#define SENSOR_READ 0b01010011

// Number of sensors attached to the multiplexer. Supports up to eight
#define SENSOR_NUMBER 2

// Initializes multiplexer and sets the inital channel to 0
void mux_init(void);

// Selects between channels on the multiplexer
void mux_select(uint8_t channel);

// Returns the current channel
uint8_t mux_get(void);

// Initializes color sensor
void sensor_init(void);

// prints the values given to it
void sensor_printvalues(uint16_t* val);

// populates the values array with crgb data
void sensor_get(uint16_t* values);

int main(void)
{
	uint8_t i;
	uint16_t values[4];
	initUSART();
	printString("|------------------------|\n"
				"| Block Shover 5000      |\n"
				"| By William Oberndorfer |\n"
				"| -----------------------|\n");
	i2c_init();
	mux_init();
	sei();
	printString("Initializing sensors\n"
				"--------------------\n");
	for (i = 0; i < SENSOR_NUMBER; i++) {
		mux_select(i);
		sensor_init();
	}
	printString("~All sensors initialized~\n\n");
	
	mux_select(0);
	
	while(1) {
		for (i = 0; i < SENSOR_NUMBER; i++) {
			printString("\n");
			mux_select(i);
			sensor_get(values);
			sensor_printvalues(values);
			_delay_ms(1000);
		}
	}

	return 0;
}

ISR(USART_RXC_vect)
{
	char received;
	received = receiveByte();
	printString("bloof");
}

void mux_init(void)
{
	i2c_start(MUX_WRITE);
	i2c_write(0x08);
	i2c_stop();
	printString("PCA9547 Initialized\n\n");
}

void mux_select(uint8_t channel)
{
	i2c_start(MUX_WRITE);
	i2c_write(0x08 | channel);
	i2c_stop();
	printString("Channel selected: ");
	printByte(channel);
	printString("\n");
}

uint8_t mux_get(void)
{
	uint8_t retval;
	i2c_start(MUX_READ);
	retval = i2c_read_nack();
	i2c_stop();
	return retval - 0x08;
}

void sensor_init(void)
{
	i2c_start(SENSOR_WRITE);
	i2c_write(0x80 | 0x00);
	i2c_stop();
	i2c_start(SENSOR_WRITE);
	i2c_write(0x01 | 0x02);
	i2c_stop();
	i2c_start(SENSOR_WRITE);
	i2c_write(0x80 | 0x14);
	i2c_stop();
	printString("Sensor initialized\n");
}

void sensor_printvalues(uint16_t* val)
{
	printString("C: ");
	printWord(val[0]);
	printString("    ");
	printString("R: ");
	printWord(val[1]);
	printString("    ");
	printString("G: ");
	printWord(val[2]);
	printString("    ");
	printString("B: ");
	printWord(val[3]);
	printString("\n");
}

void sensor_get(uint16_t* values)
{
	uint8_t i;
	i2c_start(SENSOR_READ);
	for(i = 0; i < 4; i++) {
		if (i < 3) {
			values[i] = i2c_read_ack();
			values[i] |= (i2c_read_ack() << 8);
		}
		else if (i == 3) {
			values[i] = i2c_read_ack();
			values[i] = (i2c_read_nack() << 8);
		}
	}
	i2c_stop();
}
