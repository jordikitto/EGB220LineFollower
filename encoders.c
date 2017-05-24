// Code for the encoders

// Ports and Pins
#define ENCODER_LEFT 8
#define ENCODER_RIGHT 9

float ReadEncoderLeft() {
	return adc_read_sensor(8);
}

float ReadEncoderRight() {
	return adc_read_sensor(9);
}