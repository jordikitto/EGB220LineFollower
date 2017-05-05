#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Code Includes
#include "macros.h"
#include "multiLED.h"
#include "motors.h"
#include "adc.h"

enum mode {LEFT, NORMAL, RIGHT};
enum mode current_mode = NORMAL;

int speed_max = 130; // 77.5 is optimal
float threshold = 0.5;

// Interrupts
ISR(TIMER0_OVF_vect) {

}

ISR(TIMER1_OVF_vect) {

}

int main() {

	adc_init();
	led_init();

	sei(); // Enable global interrupts
	setupMotor2AndTimer0();
	setupMotor1AndTimer1();

	while(1) {
		// Read sensors
		float sensor_left_mid = adc_read_sensor(SENSORMID_LEFTMID); // middle left
		float sensor_left_out = adc_read_sensor(SENSORMID_LEFTOUTER); // outer left
		float sensor_right_out = adc_read_sensor(SENSORMID_RIGHTOUTER); // outer right
		float sensor_right_mid = adc_read_sensor(SENSORMID_RIGHTMID); // middle right

		float sensor_left_avg = (sensor_left_mid + sensor_left_out) / 2;
		float sensor_right_avg = (sensor_right_mid + sensor_right_out) / 2;

		led_reset();

		// Detect mode
		if (sensor_right_avg > 0.5 && sensor_left_mid > 0.5) {
			if (current_mode == NORMAL) {
				current_mode = LEFT;
				
			}

		} 
		else if (sensor_left_avg > 0.5 && sensor_right_mid > 0.5) {
			if (current_mode == NORMAL) {
				current_mode = RIGHT;
				
			}
		} 
		else if (sensor_left_mid < 0.2 || sensor_right_mid < 0.2) {
			current_mode = NORMAL;
		}

		if (current_mode == NORMAL) {
			float sensor_total = sensor_left_avg - sensor_right_avg;
			float speed_turn = speed_max * sensor_total;

			MOTORRIGHT_REVERSE = 0;
			MOTORLEFT_REVERSE = 0;

			if (speed_turn > 0) { // positive, go left
				MOTORLEFT_FORWARD = speed_max;
				MOTORRIGHT_FORWARD = speed_max - ABS(speed_turn);
			} else if (speed_turn < 0) { // negative, go right
				MOTORLEFT_FORWARD = speed_max - ABS(speed_turn);
				MOTORRIGHT_FORWARD = speed_max;
			}
			led_set_green();
		}
		else if (current_mode == LEFT) {
			MOTORRIGHT_FORWARD = speed_max*1.3;
			MOTORLEFT_FORWARD = 255;
			MOTORLEFT_REVERSE = 255;
			led_set_blue();
		} else if (current_mode == RIGHT) {
			MOTORRIGHT_FORWARD = 255;
			MOTORRIGHT_REVERSE = 255;
			MOTORLEFT_FORWARD = speed_max*1.3;
			led_set_red();
		}

		

		// Note 1 is black, 0 is white

		

	}

	return 0;
}