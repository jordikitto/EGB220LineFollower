// Functions that can be reused and aren't specific, although they may have a specific use

float get_max(float numbers[], int count) {
	float max = numbers[0];

	for (int i = 1; i < count; i++) {
		if (numbers[i] > max) {
			max = numbers[i];
		}
	}

	return max;
}

float get_min(float numbers[], int count) {
	float min = numbers[0];

	for (int i = 1; i < count; i++) {
		if (numbers[i] < min) {
			min = numbers[i];
		}
	}

	return min;
}

float get_sum(float numbers[], int count) {
	float sum = 0;

	for (int i = 0; i < count; i++) {
		sum += numbers[i];
	}

	return sum;
}

// void blink_value(float value) {
// 	// Up to 5 figures > 0
// 	// Uses int truncation to floor

// 	// Run away if value greater than 5 figures
// 	if (value > 100000) {
// 		led_set_green_and_red();
// 		_delay_ms(2000);
// 		return;
// 	}

// 	if ((value / 10000) > 1) {
// 		double count = floor(value / 10000);
// 		value = value - (10000 * count);

// 		for (int i = 0; i < count; i++) {
// 			led_set_green();
// 			_delay_ms(201);
// 			led_reset();
// 			_delay_ms(201);
// 		}
// 	}

// 	led_set_red();
// 	_delay_ms(401);
// 	led_reset();

// 	if ((value / 1000) > 1) {
// 		double count = floor(value / 1000);
// 		value = value - (1000 * count);

// 		for (int i = 0; i < count; i++) {
// 			led_set_green();
// 			_delay_ms(201);
// 			led_reset();
// 			_delay_ms(201);
// 		}
// 	}

// 	led_set_red();
// 	_delay_ms(401);
// 	led_reset();

// 	if ((value / 100) > 1) {
// 		double count = floor(value / 100);
// 		value = value - (100 * count);

// 		for (int i = 0; i < count; i++) {
// 			led_set_green();
// 			_delay_ms(201);
// 			led_reset();
// 			_delay_ms(201);
// 		}
// 	}

// 	led_set_red();
// 	_delay_ms(401);
// 	led_reset();

// 	if ((value / 10) > 1) {
// 		double count = floor(value / 10);
// 		value = value - (10 * count);

// 		for (int i = 0; i < count; i++) {
// 			led_set_green();
// 			_delay_ms(201);
// 			led_reset();
// 			_delay_ms(201);
// 		}
// 	}

// 	led_set_red();
// 	_delay_ms(401);
// 	led_reset();

// 	if ((value / 10) > 1) {
// 		double count = floor(value / 10);
// 		value = value - (10 * count);

// 		for (int i = 0; i < count; i++) {
// 			led_set_green();
// 			_delay_ms(201);
// 			led_reset();
// 			_delay_ms(201);
// 		}
// 	}

// 	led_set_red();
// 	_delay_ms(401);
// 	led_reset();

// 	if ((value / 1) > 1) {
// 		double count = floor(value / 1);
// 		value = value - (1 * count);

// 		for (int i = 0; i < count; i++) {
// 			led_set_green();
// 			_delay_ms(201);
// 			led_reset();
// 			_delay_ms(201);
// 		}
// 	}

// 	// Press bottom switch to continue
// 	while(!switch_bot_pressed());

// }