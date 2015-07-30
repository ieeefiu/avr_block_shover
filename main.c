#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "USART.h"

void menu(void);

enum colors {NONE, RED, GREEN, YELLOW, BLUE} color;

void shove(uint8_t col);

volatile uint8_t received;

int main(void)
{
	initUSART();
	UCSR0B |= (1 << RXCIE0);
	sei(); // enable interrupts
	set_sleep_mode(SLEEP_MODE_IDLE);
	
	printString("|------------------------|\n"
				"| Block Shover 5000      |\n"
				"| By William Oberndorfer |\n"
				"|------------------------|\n\n");

	
	while(1) {
		shove(color);
		menu();
		sleep_mode();
	}
}

ISR(USART_RX_vect)
{
	received = UDR0;
	switch(received) {
	case 0x31:
		color = RED;
		break;
	case 0x32:
		color = GREEN;
		break;
	case 0x33:
		color = YELLOW;
		break;
	case 0x34:
		color = BLUE;
		break;
	case 0x30:
		color = NONE;
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

void shove(uint8_t col)
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
		color = NONE;
		break;
	}

	switch(col) {
	default:
		color = RED;
		break;
	}
}
