// Code for the encoders

// Ports and Pins
#define ENCODER_LEFT 0  //8
#define ENCODER_RIGHT 1  //

#define ENCODER_THRESHOLD 0.05

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

int TriggeredEncoderRight() {
	if (ReadEncoderRight() < ENCODER_THRESHOLD) {
		return 1;
	} else {
		return 0;
	}
}