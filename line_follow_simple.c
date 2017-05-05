#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Code Includes
#include "macros.h"
#include "multiLED.h"
#include "motors.h"
#include "adc.h"

int speed = 80; // 77.5 is optimal

// Interrupts
ISR(TIMER0_OVF_vect) {

}

ISR(TIMER1_OVF_vect) {
	float sensor_left_mid = adc_read_sensor(SENSORMID_LEFTMID); // middle left
	float sensor_left_out = adc_read_sensor(SENSORMID_LEFTOUTER); // outer left
	float sensor_right_out = adc_read_sensor(SENSORMID_RIGHTOUTER); // outer right
	float sensor_right_mid = adc_read_sensor(SENSORMID_RIGHTMID); // middle right

	float sensor_left_avg = (sensor_left_mid + sensor_left_out) / 2;
	float sensor_right_avg = (sensor_right_mid + sensor_right_out) / 2;

	// Note 1 is black, 0 is white

	if (sensor_left_avg < 0.5 && sensor_right_avg > 0.5) { // If right black reading
			// Go right
			MOTORRIGHT_FORWARD = 0;
			MOTORLEFT_FORWARD = speed;
			led_set_red();
		}
		else if (sensor_right_avg < 0.5 && sensor_left_mid > 0.5) { // If left black reading
			// Go left
			MOTORRIGHT_FORWARD = speed;
			MOTORLEFT_FORWARD = 0;
			led_set_red();
		}
		else if (sensor_right_avg < 0.5 && sensor_left_avg < 0.5) { // If both white
			MOTORRIGHT_FORWARD = speed;
			MOTORLEFT_FORWARD = speed;
			led_set_green();
		}
		else {
			MOTORRIGHT_FORWARD = 0;
			MOTORLEFT_FORWARD = 0;
			led_set_yellow();
		}


}

int main() {

	adc_init();
	led_init();

	sei(); // Enable global interrupts
	setupMotor2AndTimer0();
	setupMotor1AndTimer1();

	while(1) {
		// Do the things
	}

	return 0;
}