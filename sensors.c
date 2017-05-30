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

float color_start;
float color_finish;
int color_readings = 100;

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

void setup_color_marker_sensing() {
	led_reset();

	// Record start color on button press
	led_set_green();
	while(!switch_top_pressed());
	// Record value from sensor
	led_reset();

	float readings_start[color_readings];

	int index = 0;
	while (index < color_readings) {
		readings_start[index] = ReadSensorRight(1); // Takes average of both
		index++;
		_delay_ms(10);
	}

	// Get average value
	color_start = get_sum(readings_start, color_readings)/color_readings;

	// Ping LED
	led_reset();
	_delay_ms(250);
	led_set_green();
	_delay_ms(250);
	led_reset();

	// Record end color on button press
	led_set_red();
	while(!switch_top_pressed());
	led_reset();

	// Record value from sensor
	float readings_finish[color_readings];

	index = 0;
	while (index < color_readings) {
		readings_finish[index] = ReadSensorRight(1); // Takes average of both
		index++;
		_delay_ms(10);
	}

	// Get average value
	color_finish = get_sum(readings_finish, color_readings)/color_readings;

	// Ping LED
	led_reset();
	_delay_ms(250);
	led_set_red();
	_delay_ms(250);
	led_reset();
}

int within_start_color_range(float reading) {
	// Gets a similarity reading
	float start_percent = ABS(reading - color_start)/color_start;
	float finish_percent = ABS(reading - color_start)/color_start;

	// If similarity is closer to start color, return true
	if (start_percent < finish_percent) {
		return 1;
	} else {
		return 0;
	}
}

int within_finish_color_range(float reading) {
	// Gets a similarity reading
	float start_percent = ABS(reading - color_finish)/color_finish;
	float finish_percent = ABS(reading - color_finish)/color_finish;

	// If similarity is closer to finish color, return true
	if (start_percent > finish_percent) {
		return 1;
	} else {
		return 0;
	}
}

void test_color_readings() {
	while(1) {
		if (within_start_color_range(ReadSensorRight(1))) {
			led_set_green();
		} else {
			led_reset();
		}

		if (within_finish_color_range(ReadSensorRight(1))) {
			led_set_red();
		} else {
			led_reset();
		}
	}
}