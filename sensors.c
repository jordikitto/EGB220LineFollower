// For getting and working with inputs from sensors

// PORTS AND PINS
#define SENSORMID_LEFTMID 4
#define SENSORMID_LEFTOUTER 5
#define SENSORMID_RIGHTMID 7
#define SENSORMID_RIGHTOUTER 6

#define isBlack(sensor_value) (sensor_value > threshold_black)
#define isWhite(sensor_value) (sensor_value < threshold_white)

// Variables
float threshold_black = 0.5;
float threshold_white = 0.2;

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