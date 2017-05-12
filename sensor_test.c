#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Code Includes
#include "macros.h"
#include "multiLED.h"
#include "motors.h"
#include "adc.h"

int speed_max = 80;
int speed = 0; // 77.5 is optimal

// Interrupts
ISR(TIMER0_OVF_vect) {

}

ISR(TIMER1_OVF_vect) {

}

int main() {

	adc_init();
	led_init();

	sei(); // Enable global interrupts

	while(1) {
		float sensor_left_mid = adc_read_sensor(SENSORMID_LEFTMID); // middle left
		float sensor_left_out = adc_read_sensor(SENSORMID_LEFTOUTER); // outer left
		float sensor_right_out = adc_read_sensor(SENSORMID_RIGHTOUTER); // outer right
		float sensor_right_mid = adc_read_sensor(SENSORMID_RIGHTMID); // middle right

		led_reset();

		if (sensor_left_out < 0.9) {
			led_set_red();
		}
		if (sensor_left_mid < 0.9) {
			led_set_green();
		}
		if (sensor_right_out < 0.9) {
			led_set_purple();
		}
		if (sensor_right_mid < 0.9) {
			led_set_blue();
		}

		// Note 1 is black, 0 is white
	}

	return 0;
}