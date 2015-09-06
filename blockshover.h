#ifndef blockshover_h
#define blockshover_h

// Addresses for reading and writing to the multiplexer and sensor
#define MUX_WRITE 0b11100000
#define MUX_READ 0b11100001

#define SENSOR_WRITE 0b01010010
#define SENSOR_READ 0b01010011

// Pin values for shovers
#define RAIL_1 PB1
#define RAIL_2 PB2

// Colors
enum colors {NONE, RED, GREEN, YELLOW, BLUE};

// Number of sensors attached to the multiplexer. Supports up to eight
#define SENSOR_NUMBER 3

// Initializes multiplexer and sets the initial channel to 0
void mux_init(void);

// Selects between channels on the multiplexer
void mux_select(uint8_t channel);

// Returns the current channel
uint8_t mux_get(void);

// Initializes color sensor
void sensor_init(void);

// prints the values given to it
void sensor_printvalues(uint16_t* values);

// populates the values array with crgb data
void sensor_get(uint8_t channel, uint16_t* values);

// checks colors, sets value for a sensor
void check_color(uint8_t channel, uint16_t* values, uint8_t* sensors);

// shoves blocks of specified color
void shove(uint8_t color, uint8_t* sensors);

#endif
