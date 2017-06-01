// For getting and working with inputs from sensors

// PORTS AND PINS
#define SENSORMID_LEFTMID 4
#define SENSORMID_LEFTOUTER 5
#define SENSORMID_RIGHTMID 1
#define SENSORMID_RIGHTOUTER 0

#define SENSORRIGHT_INNER 6
#define SENSORRIGHT_OUTER 7

#define SENSORLEFT_INNER 11
#define SENSORLEFT_OUTER 10

#define isBlack(sensor_value) (sensor_value > threshold_black)
#define isWhite(sensor_value) (sensor_value < threshold_white)

// Variables
float threshold_black = 0.5;
float threshold_white = 0.2;

typedef enum {GREEN, RED, BLACK, WHITE}  Color;

float color_green;
float color_white;

float color_sense_threshold = 20; // percent

int right_marker_count = 0;
int marker_seen = 0;
int marker_seen_time_count = 0;

int left_triggered = 0;
int right_triggered = 0;

// Functions

// Func: Returns an individual sensor reading from middle sensorboard
float ReadSensorMid(int sensor_num) {
	switch (sensor_num) {
		case 0:
			return adc_read_sensor(SENSORMID_RIGHTOUTER);
			break;
		case 1:
			return adc_read_sensor(SENSORMID_RIGHTMID);
			break;
		case 2:
			return adc_read_sensor(SENSORMID_LEFTMID);
			break;
		case 3:
			return adc_read_sensor(SENSORMID_LEFTOUTER);
			break;
		default:
			break;
	}
}

float ReadSensorRight(int sensor_num) {
	switch (sensor_num) {
		case 0:
			return adc_read_sensor(SENSORRIGHT_INNER);
			break;
		case 1:
			return adc_read_sensor(SENSORRIGHT_OUTER);
			break;
		default:
			break;
	}
}

float ReadSensorRight_raw(int sensor_num) {
	switch (sensor_num) {
		case 0:
			return adc_read_sensor_raw(SENSORRIGHT_INNER);
			break;
		case 1:
			return adc_read_sensor_raw(SENSORRIGHT_OUTER);
			break;
		default:
			break;
	}
}

float ReadSensorLeft(int sensor_num) {
	switch (sensor_num) {
		case 0:
			return adc_read_sensor(SENSORLEFT_INNER);
			break;
		case 1:
			return adc_read_sensor(SENSORLEFT_OUTER);
			break;
		default:
			break;
	}
}

float ReadSensorLeft_raw(int sensor_num) {
	switch (sensor_num) {
		case 0:
			return adc_read_sensor_raw(SENSORLEFT_INNER);
			break;
		case 1:
			return adc_read_sensor_raw(SENSORLEFT_OUTER);
			break;
		default:
			break;
	}
}

int getRightMarkerCount() {
	return right_marker_count;
}

void ReadMarkers(int timer_count_input) {

	if (marker_seen) {
		float elapsed_time = (timer_count_input - marker_seen_time_count) * timer_target;
		if (elapsed_time > 0.5) {

			//led_reset();

			if (left_triggered && right_triggered) {
				//led_set_green_and_red();
			} else if (left_triggered) {
				led_set_green();
				_delay_ms(50);
				led_reset_bot();
			} else if (right_triggered) {
				//led_set_red();
				right_marker_count++;
			}

			left_triggered = 0;
			right_triggered = 0;
			marker_seen = 0;

			return;
		} else {

			int left_triggered_current = isWhite(ReadSensorLeft(1));
			int right_triggered_current = isWhite(ReadSensorRight(1));

			if (left_triggered_current && right_triggered_current) {
				left_triggered = 1;
				right_triggered = 1;
			} else if (left_triggered_current) {
				left_triggered = 1;
			} else if (right_triggered_current) {
				right_triggered = 1;
			}

			return;
		}
	}
	

	int left_triggered_current = isWhite(ReadSensorLeft(1));
	int right_triggered_current = isWhite(ReadSensorRight(1));

	if (left_triggered_current || right_triggered_current) {
		marker_seen = 1;
	}

	if (marker_seen) {
		marker_seen_time_count = timer_count_input;
	}

}

void setup_color_marker_sensing() {
	led_reset();

	// Record green color on button press
	led_set_green();
	while(!switch_top_pressed());
	// Record value from sensor
	color_green = ReadSensorLeft_raw(1);

	led_reset();
	_delay_ms(100);
	led_set_green();
	_delay_ms(100);

	led_reset();
	// Record white color on button press
	led_set_cyan();
	while(!switch_top_pressed());
	// Record value from sensor
	color_white = ReadSensorLeft_raw(1);

	led_reset();
	_delay_ms(100);
	led_set_cyan();
	_delay_ms(100);

	led_reset();

	// blink_value(color_green);

	// led_reset();
	// _delay_ms(500);
	// led_set_cyan();
	// _delay_ms(500);

	// blink_value(color_white);

}

Color get_detected_color(float reading) {
	float green = (ABS(reading - color_green)/color_green)*100;
	float white = (ABS(reading - color_white)/color_white)*100;

	float vals[] = {green, white};
	float min = get_min(vals, 2);

	if (min == white && white < color_sense_threshold) {
		return WHITE;
	}
	if (min == green && green < color_sense_threshold) {
		return GREEN;
	}

	return BLACK;
}

Color get_detected_color_simple(float reading) {
	float diff_green = fabs(color_green - reading);
	float diff_white = fabs(color_white - reading);

	float vals[] = {diff_green, diff_white};
	float min = get_min(vals, 2);

	if (min < 15) {
		if (min == diff_green) {
			return GREEN;
		}
		if (min == diff_white) {
			return WHITE;
		}
	} else {
		return BLACK;
	}
}

int isColoredMarker() {
	Color current_color = get_detected_color_simple(ReadSensorLeft_raw(1));

	if (current_color == GREEN) {
		return 1;
	} else {
		return 0;
	}
}



void test_color_readings() {
	while (1) {
		Color current_color = get_detected_color_simple(ReadSensorLeft_raw(1));

		led_reset();
		switch (current_color) {
			case GREEN:
				led_set_green();
				break;
			case WHITE:
				led_set_red();
				break;
			default:
				led_reset();
		}
	}
}