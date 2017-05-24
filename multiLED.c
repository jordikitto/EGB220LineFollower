/*
 * For use with LED that can take 3 different inputs
 * By Jordi Kitto
 */

// PORTS AND PINS
#define PORTGREEN PORTE
#define PINGREEN 6
#define PORTRED PORTB
#define PINRED 2
#define PORTBLUE PORTB
#define PINBLUE 1

// FUNCTIONS
void led_init() {
	SET_OUTPUT(DDRB, PINBLUE);
	SET_OUTPUT(DDRB, PINRED);
	SET_OUTPUT(DDRE, PINGREEN);
}

void led_set_green() {
	OUTPUT_HIGH(PORTGREEN, PINGREEN);
	OUTPUT_LOW(PORTRED, PINRED);
	OUTPUT_LOW(PORTBLUE, PINBLUE);
}

void led_set_red() {
	OUTPUT_LOW(PORTGREEN, PINGREEN);
	OUTPUT_HIGH(PORTRED, PINRED);
	OUTPUT_LOW(PORTBLUE, PINBLUE);
}

void led_set_blue() {
	OUTPUT_LOW(PORTGREEN, PINGREEN);
	OUTPUT_LOW(PORTRED, PINRED);
	OUTPUT_HIGH(PORTBLUE, PINBLUE);
}

void led_set_yellow() {
	OUTPUT_HIGH(PORTGREEN, PINGREEN);
	OUTPUT_HIGH(PORTRED, PINRED);
	OUTPUT_LOW(PORTBLUE, PINBLUE);
}

void led_set_cyan() {
	OUTPUT_HIGH(PORTGREEN, PINGREEN);
	OUTPUT_LOW(PORTRED, PINRED);
	OUTPUT_HIGH(PORTBLUE, PINBLUE);
}

void led_set_purple() {
	OUTPUT_LOW(PORTGREEN, PINGREEN);
	OUTPUT_HIGH(PORTRED, PINRED);
	OUTPUT_HIGH(PORTBLUE, PINBLUE);
}

void led_set_white() {
	OUTPUT_HIGH(PORTGREEN, PINGREEN);
	OUTPUT_HIGH(PORTRED, PINRED);
	OUTPUT_HIGH(PORTBLUE, PINBLUE);
}

void led_reset() {
	OUTPUT_LOW(PORTGREEN, PINGREEN);
	OUTPUT_LOW(PORTRED, PINRED);
	OUTPUT_LOW(PORTBLUE, PINBLUE);
}

void led_test_all() {
	led_set_blue();
	_delay_ms(500);
	led_set_red();
	_delay_ms(500);
	led_set_green();
	_delay_ms(500);
	led_set_cyan();
	_delay_ms(500);
	led_set_purple();
	_delay_ms(500);
	led_set_yellow();
	_delay_ms(500);
	led_set_white();
	_delay_ms(500);
}