#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "USART.h"

void menu(void);

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

	menu();
	
	while(1) {
		sleep_mode();
	}
}

ISR(USART_RX_vect)
{
	received = UDR0;
	switch(received) {
	case 0x31:
		printString("Shove Red\n");
		break;
	case 0x32:
		printString("Shove Green\n");
		break;
	case 0x33:
		printString("Shove Yellow\n");
		break;
	case 0x34:
		printString("Shove Blue\n");
		break;
	case 0x30:
		printString("Display colors\n");
		break;
	default:
		printString("Invalid input\n");
		break;
	}
	menu();
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
