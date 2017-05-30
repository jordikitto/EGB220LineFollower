/*
 * For use with LED that can take 3 different inputs
 * By Jordi Kitto
 */

// PORTS AND PINS
#define PORTBLUE PORTE
#define PINBLUE 6
#define DDRBLUE DDRE
#define PORTRED PORTB
#define PINRED 2
#define DDRRED DDRB
#define PORTGREEN PORTB
#define PINGREEN 1
#define DDRGREEN DDRB

// FUNCTIONS
void led_init() {
	SET_OUTPUT(DDRBLUE, PINBLUE);
	SET_OUTPUT(DDRRED, PINRED);
	SET_OUTPUT(DDRGREEN, PINGREEN);
}

void led_set_green() {
	OUTPUT_HIGH(PORTGREEN, PINGREEN);
	OUTPUT_LOW(PORTBLUE, PINBLUE);
}

void led_set_red() {
	OUTPUT_HIGH(PORTRED, PINRED);
}

void led_set_blue() {
	OUTPUT_LOW(PORTGREEN, PINGREEN);
	OUTPUT_HIGH(PORTBLUE, PINBLUE);
}

void led_set_green_and_red() {
	OUTPUT_HIGH(PORTGREEN, PINGREEN);
	OUTPUT_HIGH(PORTRED, PINRED);
	OUTPUT_LOW(PORTBLUE, PINBLUE);
}

void led_set_cyan() {
	OUTPUT_HIGH(PORTGREEN, PINGREEN);
	OUTPUT_HIGH(PORTBLUE, PINBLUE);
}

void led_set_blue_and_red() {
	OUTPUT_LOW(PORTGREEN, PINGREEN);
	OUTPUT_HIGH(PORTRED, PINRED);
	OUTPUT_HIGH(PORTBLUE, PINBLUE);
}

void led_set_cyan_and_red() {
	OUTPUT_HIGH(PORTGREEN, PINGREEN);
	OUTPUT_HIGH(PORTRED, PINRED);
	OUTPUT_HIGH(PORTBLUE, PINBLUE);
}

void led_reset() {
	OUTPUT_LOW(PORTGREEN, PINGREEN);
	OUTPUT_LOW(PORTRED, PINRED);
	OUTPUT_LOW(PORTBLUE, PINBLUE);
}

void led_reset_top() {
	OUTPUT_LOW(PORTRED, PINRED);
}

void led_reset_bot() {
	OUTPUT_LOW(PORTGREEN, PINGREEN);
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
	led_set_green_and_red();
	_delay_ms(500);
	led_set_blue_and_red();
	_delay_ms(500);
	led_set_cyan_and_red();
	_delay_ms(500);
}