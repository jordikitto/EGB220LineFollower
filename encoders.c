// Code for the encoders

// Ports and Pins
#define ENCODER_LEFT 8
#define ENCODER_RIGHT 9

#define ENCODER_THRESHOLD 0.05
#define WHEEL_CIRCUMFERENCE 220 // (mm)
#define SPEED_AT_MAX_POWER 1000 // mm per second

// For detecting 1 rotation only
int rightwheel_seen = 0;
double rightwheel_trigger_time = 0;
int leftwheel_seen = 0;
double leftwheel_trigger_time = 0;

double rightwheel_wait_time; // Time to wait until allowing wheel trigger 
								// to be seen again! (seconds)
double leftwheel_wait_time;

float velocity_right = 0; // mm per second
float velocity_left = 0; // mm per second

void setupTimer3() {
	TCCR3A = 0; // Clear register

	double resolution = 0.000064;

	float count = (timer_target / resolution) - 1;

	OCR3A = count; // target = resolution * (count + 1). This is count.

	// Turn on compare mode
	OUTPUT_HIGH(TCCR3B, WGM32);

	// Set prescaler to 1024
	OUTPUT_HIGH(TCCR3B, CS30);
	OUTPUT_HIGH(TCCR3B, CS32);

	// Enable compare interrupt
	OUTPUT_HIGH(TIMSK3, OCIE3A);
}

float ReadEncoderLeft() {
	return adc_read_sensor(ENCODER_LEFT);
}

float ReadEncoderRight() {
	return adc_read_sensor(ENCODER_RIGHT);
}

int TriggeredEncoderLeft() {
	if (ReadEncoderLeft() < ENCODER_THRESHOLD) {
		return 1;
	} else {
		return 0;
	}
}

void UpdateVelocityLeft(int previous_timer, int current_timer) {
	int change_time = current_timer - previous_timer;
	velocity_left = WHEEL_CIRCUMFERENCE / (change_time * timer_target);
}

float getVelocityLeft() {
	return velocity_left;
}

int LeftWheelRotated(int current_timer, float current_speed) {
	double current_time = current_timer * timer_target;

	// Update wait time based on current speed
	double current_frequency = (current_speed * SPEED_AT_MAX_POWER)/WHEEL_CIRCUMFERENCE;
	leftwheel_wait_time = (1/current_frequency) / 2;

	if (TriggeredEncoderLeft()) {
		if (leftwheel_seen) {
			return 0;
		} else {
			leftwheel_seen = 1;
			leftwheel_trigger_time = current_time;
			return 1;
		}
		
	} else {
		if (leftwheel_seen) {
			if (current_time - leftwheel_trigger_time > leftwheel_wait_time) {
				leftwheel_seen = 0;
			}
		}
		return 0;
	}
}

float setMotorSpeedLeft(int desired_speed, float previous_speed) {
	if (getVelocityLeft() > desired_speed) {
		previous_speed = previous_speed - 0.05;
		MOTORLEFT_FORWARD = SET_SPEED(previous_speed);
		return previous_speed;
	} else {
		previous_speed = previous_speed + 0.05;
		MOTORLEFT_FORWARD = SET_SPEED(previous_speed);
		return previous_speed;
	}
}

int TriggeredEncoderRight() {
	if (ReadEncoderRight() < ENCODER_THRESHOLD) {
		return 1;
	} else {
		return 0;
	}
}

int RightWheelRotated(int current_timer, float current_speed) {
	double current_time = current_timer * timer_target;

	// Update wait time based on current speed
	double current_frequency = (current_speed * SPEED_AT_MAX_POWER)/WHEEL_CIRCUMFERENCE;
	rightwheel_wait_time = (1/current_frequency) / 2;

	if (TriggeredEncoderRight()) {
		if (rightwheel_seen) {
			return 0;
		} else {
			rightwheel_seen = 1;
			rightwheel_trigger_time = current_time;
			return 1;
		}
		
	} else {
		if (rightwheel_seen) {
			if (current_time - rightwheel_trigger_time > rightwheel_wait_time) {
				rightwheel_seen = 0;
			}
		}
		return 0;
	}
}

void UpdateVelocityRight(int previous_timer, int current_timer) {
	float change_time = current_timer - previous_timer;
	velocity_right = WHEEL_CIRCUMFERENCE / (change_time * timer_target);
}

float getVelocityRight() {
	return velocity_right;
}

float setMotorSpeedRight(int desired_speed, float previous_speed) {
	if (getVelocityRight() > desired_speed) {
		previous_speed = previous_speed - 0.05;
		MOTORRIGHT_FORWARD = SET_SPEED(previous_speed);
		return previous_speed;
	} else {
		previous_speed = previous_speed + 0.05;
		MOTORRIGHT_FORWARD = SET_SPEED(previous_speed);
		return previous_speed;
	}
}

void test_encoders() {
	while(1) {
		MOTORRIGHT_FORWARD = SET_SPEED(0.5);
		MOTORLEFT_FORWARD = SET_SPEED(0.5);

		if (TriggeredEncoderRight()) {
			led_set_red();
		} else {
			led_reset_top();
		}

		if (TriggeredEncoderLeft()) {
			led_set_green();
		} else {
			led_reset_bot();
		}
	}
}

void test_timer_ping_led(int timer_count) { // Must go in Timer3 ISR with timer_count++;
	float timer_set_time = 1.5; // (seconds) When to ping
	int timer_set_count = timer_set_time / timer_target;

	if (timer_count % timer_set_count == 0) {
		led_set_green();
		//_delay_ms(500);
	} else {
		led_reset_bot();
	}
}