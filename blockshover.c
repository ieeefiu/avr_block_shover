#include <avr/io.h>

#include "i2c_master.h"
#include "USART.h"
#include "blockshover.h"

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

void sensor_printvalues(uint16_t* values)
{
	printString("C: ");
	printWord(values[0]);
	printString("    ");
	printString("R: ");
	printWord(values[1]);
	printString("    ");
	printString("G: ");
	printWord(values[2]);
	printString("    ");
	printString("B: ");
	printWord(values[3]);
	printString("\n");
}

void sensor_get(uint8_t channel, uint16_t* values)
{
	uint8_t i;
	mux_select(channel);
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

void check_color(uint8_t channel, uint16_t* values, uint8_t* sensors)
{
	if(values[0] > 5000) {
		if(values[1] > values[3]) {
			sensors[channel] = RED;
		}
		else if(values[1] < values[3]) {
			sensors[channel] = GREEN;
		}
	}
	else sensors[channel] = NONE;
}

void shove(uint8_t color, uint8_t* sensors)
{
	switch(color) {
	case RED:
		printString("shoving red\n");
		break;
	case GREEN:
		printString("shoving green\n");
		break;
	case YELLOW:
		printString("shoving yellow\n");
		break;
	case BLUE:
		printString("shoving blue\n");
		break;
	case NONE:
		printString("shoving none\n");
		break;
	default:
		break;
	}
}
