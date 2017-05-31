#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// CONSTANTS
#define timer_target 0.01 // seconds (intervals for timer)

// Code Includes
#include "macros.h"
#include "multiLED.c"
#include "motors.c"
#include "adc.c"
#include "switches.c"
#include "encoders.c"
#include "helpers.c"
#include "sensors.c"

enum mode {LEFT, NORMAL, RIGHT};
enum mode current_mode = NORMAL;
enum mode emergency_mode = NORMAL;

int speed_max;
float speed_current_right;
float speed_current_left;

int sensor_out_dist = 15;
int sensor_mid_dist = 5;

float last_error = 0.0;
float integral = 0;

// Velocity variables
int timer_count = 0; // How many times we've reached timer_target
int rotations_right_count = 0;
int rotations_right_time_previous = 0;
int rotations_left_count = 0;
int rotations_left_time_previous = 0;
float change;

// Interrupts
ISR(TIMER0_OVF_vect) {

}

ISR(TIMER1_OVF_vect) { // No prescaling (AKA FAST)
	if (RightWheelRotated(timer_count, speed_current_right)) {
		if (rotations_right_time_previous != timer_count && rotations_right_count > 0) {
			UpdateVelocityRight(rotations_right_time_previous, timer_count);
			speed_current_right = getVelocityRight();
		}
		rotations_right_count++;
		rotations_right_time_previous = timer_count; // record rotation for next time
	}

	if (LeftWheelRotated(timer_count, speed_current_left)) {
		if (rotations_left_time_previous != timer_count && rotations_left_count > 0) {
			UpdateVelocityLeft(rotations_left_time_previous, timer_count);
			speed_current_right = getVelocityLeft();
		}
		rotations_left_count++;
		rotations_left_time_previous = timer_count;
	}
}

// Timer keeper
ISR(TIMER3_COMPA_vect) {
	timer_count++;
}

void setup_to_start() {
	// Start countdown
	led_set_green_and_red();
	_delay_ms(500);
	led_reset();
	_delay_ms(500);
	led_set_green();
	_delay_ms(500);
	led_reset();
	_delay_ms(500);
}

int main() {

	speed_max = SET_SPEED(0.5);

	adc_init();
	led_init();
	switches_init();

	sei(); // Enable global interrupts
	setupMotor2AndTimer0();
	setupMotor1AndTimer1();
	setupTimer3();

	setup_color_marker_sensing();

	// Testing triggers
	test_color_readings();
	//test_encoders();

	setup_to_start();

	while(1) {

		// Read sensors
		float sensor_right_out = ReadSensorMid(0); // outer right
		float sensor_right_mid = ReadSensorMid(1); // middle right
		float sensor_left_mid = ReadSensorMid(2); // middle left
		float sensor_left_out = ReadSensorMid(3); // outer left	

		led_reset();

		// Detect if off-line/derailed
		if (isBlack(sensor_right_out) && isBlack(sensor_right_mid) && isBlack(sensor_left_mid) && isBlack(sensor_left_out)) {
			current_mode = emergency_mode;
		}

		// Detect mode
		if (current_mode == NORMAL) { // Must be coming from center of line (AKA Normal mode)
			// Record which direction to go in event of emergency line 'derail'
			if (isBlack(sensor_right_out) && isBlack(sensor_right_mid) && isBlack(sensor_left_mid)) {
				emergency_mode = LEFT;
				
			} 
			else if (isBlack(sensor_left_out) && isBlack(sensor_left_mid) && isBlack(sensor_right_mid)) {
				emergency_mode = RIGHT;
				
			} 
		} else if (isWhite(sensor_left_out) || isWhite(sensor_right_out)) { // Centered on line
			current_mode = NORMAL;
			emergency_mode = NORMAL;
			
		}

		// Calculate error (using weighted average)
		float num = -1*sensor_out_dist*(sensor_left_out - sensor_right_out) + -1*sensor_mid_dist*(sensor_left_mid - sensor_right_mid);
		float denom = sensor_right_out + sensor_right_mid + sensor_left_mid + sensor_left_out;

		// PID
		float error = num/denom; // proportional
		float derivative = error - last_error;
		last_error = error;
		integral += error;

		// Setup values for switch
		float speed_turn = (error/15) * speed_max + integral/500;
		//float speed_turn = error/15 + integral/ + derivative*0.5;

		// Act based on mode
		switch (current_mode) {
			case NORMAL:
				Brakes_Release();

				if (error > 0.05) { // positive, go right
					MOTORRIGHT_FORWARD = speed_max;
					MOTORLEFT_FORWARD = speed_max - ABS(speed_turn);
					
				} else if (error < -0.05) { // negative, go left
					MOTORRIGHT_FORWARD = speed_max - ABS(speed_turn);
					MOTORLEFT_FORWARD = speed_max;
				} else {
					MOTORLEFT_FORWARD = speed_max;
					MOTORRIGHT_FORWARD = speed_max;
				}
				led_set_green();
				break;

			case LEFT:
				MOTORRIGHT_FORWARD = speed_max*1.3;
				Brake_Left();
				led_set_blue();
				break;

			case RIGHT:
				Brake_Right();
				MOTORLEFT_FORWARD = speed_max*1.3;
				led_set_red();
				break;

			default:
				led_reset();
		}
		

		// Note 1 is black, 0 is white

		

	}

	Brake_Right();
	Brake_Left();
	led_reset();

	return 0;
}