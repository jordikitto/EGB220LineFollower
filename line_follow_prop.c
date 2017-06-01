#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>

// CONSTANTS
#define timer_target 0.001 // seconds (intervals for timer)

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
float speed_max_percent = 0.5;
float speed_slow = 0.25;
float speed_current_right = 0;
float speed_current_left = 0;

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

// Marker stuff
int colored_marker_count = 0;
int black_marker_count = 0;
int colored_markers_seen = 0;

int already_stopped = 0;

int seen_colored = 0;

// Straight detection
int straight_count = 0;

// Interrupts
ISR(TIMER0_OVF_vect) {

}

ISR(TIMER1_OVF_vect) { // No prescaling (AKA FAST)
	// if (RightWheelRotated(timer_count, speed_current_right)) {
	// 	if (rotations_right_time_previous != timer_count && rotations_right_count > 0) {
	// 		UpdateVelocityRight(rotations_right_time_previous, timer_count);
	// 		speed_current_right = getVelocityRight();
	// 	}
	// 	rotations_right_count++;
	// 	rotations_right_time_previous = timer_count; // record rotation for next time
	// }

	// if (LeftWheelRotated(timer_count, speed_current_left)) {
	// 	if (rotations_left_time_previous != timer_count && rotations_left_count > 0) {
	// 		UpdateVelocityLeft(rotations_left_time_previous, timer_count);
	// 		speed_current_left = getVelocityLeft();
	// 	}
	// 	rotations_left_count++;
	// 	rotations_left_time_previous = timer_count;
	// }
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

	speed_max = SET_SPEED(speed_max_percent);

	adc_init();
	led_init();
	switches_init();

	sei(); // Enable global interrupts
	setupMotor2AndTimer0();
	setupMotor1AndTimer1();
	setupTimer3();

	// while(1) {
	// 	if (isWhite(ReadSensorRight(0))) {
	// 		led_set_green();
	// 	} else {
	// 		led_reset_bot();
	// 	}

	// 	if (isWhite(ReadSensorRight(1))) {
	// 		led_set_red();
	// 	} else {
	// 		led_reset_top();
	// 	}
	// }

	// while(1) {
	// 	MOTORRIGHT_FORWARD = SET_SPEED(speed_slow);
	// 	MOTORLEFT_FORWARD = SET_SPEED(speed_slow);
	// }

	//setup_color_marker_sensing();

	// Testing triggers
	//test_color_readings();
	//test_encoders();

	// while(1) {
	// 	float value = ReadSensorRight_raw(1);
	// 	blink_value(value);
	// }

	setup_to_start();

	while(1) {

		if (getRightMarkerCount() % 2 == 0) {
			if (!already_stopped) {
				already_stopped = 1;
				Brake_Left();
				Brake_Right();
				_delay_ms(2000);
			}
		} else {
			already_stopped = 0;
			//MOTORRIGHT_FORWARD = SET_SPEED(speed_max_percent);
			//MOTORLEFT_FORWARD = SET_SPEED(speed_max_percent);
		}

		// Read sensors
		float sensor_right_out = ReadSensorMid(0); // outer right
		float sensor_right_mid = ReadSensorMid(1); // middle right
		float sensor_left_mid = ReadSensorMid(2); // middle left
		float sensor_left_out = ReadSensorMid(3); // outer left	

		// Read side sensors
		ReadMarkers(timer_count);

		// if (isColoredMarker(ReadSensorLeft_raw(1))) {
		// 	colored_marker_count++;

		// 	if (colored_marker_count > 20) {
		// 		led_set_red();
		// 		colored_markers_seen++;

		// 		if (colored_markers_seen % 2 == 1) { // If seen first green line
		// 			speed_max = SET_SPEED(speed_slow);
		// 		} else { // Else seen red line
		// 			speed_max = SET_SPEED(speed_max_percent);
		// 			led_reset();
		// 		}

		// 		colored_marker_count = 0;
				
		// 	}

		// } else {
		// 	colored_marker_count = 0;
		// }

		// Color current_color = get_detected_color_simple(ReadSensorLeft_raw(1));

		// if (current_color == GREEN && !seen_colored) {
		// 	colored_marker_count++;

		// 	if (colored_marker_count > 5) {
		// 		seen_colored = 1;
		// 	}

			
		// } else {
		// 	colored_marker_count = 0;
		// }

		// if (seen_colored) {
		// 	if (current_color == BLACK) {
		// 		black_marker_count++;

		// 		if (black_marker_count > 5) {
		// 			led_set_red();
		// 			colored_markers_seen++;

		// 			if (colored_markers_seen % 2 == 1) { // If seen first green line
		// 				speed_max = SET_SPEED(speed_slow);
		// 			} else { // Else seen red line
		// 				speed_max = SET_SPEED(speed_max_percent);
		// 				led_reset();
		// 			}

		// 			seen_colored = 0;
		// 			colored_marker_count = 0;
		// 			black_marker_count = 0;
		// 		}
		// 	} else if (current_color == WHITE) {
		// 		//seen_colored = 0;
		// 		colored_marker_count = 0;
		// 		black_marker_count = 0;
		// 	}
		// }

		// Detect straights
		float speed_right = MOTORRIGHT_FORWARD;
		float speed_left = MOTORLEFT_FORWARD;

		if ((speed_right > (speed_max * 0.7)) && (speed_left > (speed_max * 0.7))) {
			float difference = fabs(speed_right - speed_left);

			if (difference < 30) {
				straight_count++;

				if (straight_count > 100) {
					led_set_red();
				} else {
					led_reset_top();
				}
			} else {
				straight_count = 0;
			}
		}




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
				//led_set_green();
				break;

			case LEFT:
				MOTORRIGHT_FORWARD = speed_max*1.3;
				Brake_Left();
				//led_set_blue();
				break;

			case RIGHT:
				Brake_Right();
				MOTORLEFT_FORWARD = speed_max*1.3;
				//led_set_red();
				break;

			default:
				break;
				//led_reset();
		}
		

		// Note 1 is black, 0 is white

		

	}

	Brake_Right();
	Brake_Left();
	led_reset();

	return 0;
}