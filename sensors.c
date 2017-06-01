// For getting and working with inputs from sensors

// PORTS AND PINS
#define SENSORMID_LEFTMID 4
#define SENSORMID_LEFTOUTER 5
#define SENSORMID_RIGHTMID 1
#define SENSORMID_RIGHTOUTER 0

#define SENSORRIGHT_INNER 6
#define SENSORRIGHT_OUTER 7

#define SENSORLEFT_INNER 10
#define SENSORLEFT_OUTER 11

#define isBlack(sensor_value) (sensor_value > threshold_black)
#define isWhite(sensor_value) (sensor_value < threshold_white)

// Variables
float threshold_black = 0.5;
float threshold_white = 0.2;

typedef enum {GREEN, RED, BLACK, WHITE}  Color;

float color_green;
float color_red;
float color_white;

float color_sense_threshold = 5; // percent

int right_marker_count = 0;
int markers_processing = 0;
int markers_processed = 0;
int markers_processing_timer;
int markers_wait_timer;
int markers_waiting = 0;

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

int getRightMarkerCount() {
	return right_marker_count;
}

void ReadMarkers(int timer_count_input) {

	// If we are waiting, check to see if time has elapsed, else, go away
	if (markers_waiting) {
		float time_elapsed_wait = (timer_count_input - markers_wait_timer) * timer_target;
		if (time_elapsed_wait > 0.2) {
			markers_waiting = 0; // We have waited, and are no longer waiting
			markers_processed = 0; // We have now restarted, and not processed anything
		} else {
			return;
		}
	}

	// Come here second when we are processing now, only after time elapsed, move to post-processing
	// knowing we are probably about midway on the line
	if (markers_processing) {
		float time_elapsed_process = (timer_count_input - markers_processing_timer) * timer_target;
		if (time_elapsed_process > 0.001) {
			markers_processing = 0;
		} else {
			return;
		}
	}

	// Save variables for checking (optimises)
	int triggered_left = isWhite(ReadSensorLeft(0));
	int triggered_right = isWhite(ReadSensorRight(1));

	// Come here first, record the time and say we are processing, if we have not processed
	if ((triggered_right || triggered_left) && !markers_processed) {
		markers_processing = 1;
		markers_processing_timer = timer_count_input;
	}

	// Come here third for post processing



	if (triggered_right && triggered_left) {
		led_set_red();
		_delay_ms(50);
		led_reset();
	} else {
		if (triggered_right) {
			right_marker_count++;
			led_set_cyan();
			_delay_ms(50);
			led_reset();
		}

		if (triggered_left) {
			led_set_green();
			_delay_ms(50);
			led_reset();
		}
	}


	// Once post processed, and not waiting, start to wait
	if (!markers_waiting) {
		// We have now processed
		markers_processed = 1;

		// Wait a bit until seeing next marker
		// Record current time for waiting
		markers_wait_timer = timer_count_input;
		// Remember we are waiting
		markers_waiting = 1;
	}


}

void setup_color_marker_sensing() {
	led_reset();

	// Record green color on button press
	led_set_green();
	while(!switch_top_pressed());
	// Record value from sensor
	color_green = ReadSensorMid(1);

	led_reset();
	_delay_ms(100);
	led_set_green();
	_delay_ms(100);

	led_reset();
	// Record red color on button press
	led_set_red();
	while(!switch_top_pressed());
	// Record value from sensor
	color_red = ReadSensorMid(1);

	led_reset();
	_delay_ms(100);
	led_set_red();
	_delay_ms(100);

	led_reset();
	// Record white color on button press
	led_set_cyan();
	while(!switch_top_pressed());
	// Record value from sensor
	color_white = ReadSensorMid(1);

	led_reset();
	_delay_ms(100);
	led_set_cyan();
	_delay_ms(100);

	led_reset();
}

Color get_detected_color(float reading) {
	float green = (ABS(reading - color_green)/color_green)*100;
	float red = (ABS(reading - color_red)/color_red)*100;
	float white = (ABS(reading - color_white)/color_white)*100;

	float vals[] = {green, red, white};
	float min = get_min(vals, 3);

	if (min == white && white < color_sense_threshold) {
		return WHITE;
	}
	if (min == green && green < color_sense_threshold) {
		return GREEN;
	}
	if (min == red && red < color_sense_threshold) {
		return RED;
	}

	return BLACK;
}

void test_color_readings() {
	while (1) {
		Color current_color = get_detected_color(ReadSensorMid(1));

		led_reset();
		switch (current_color) {
			case RED:
				led_set_cyan();
				break;
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