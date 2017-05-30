// For utilising ADC inputs with a microcontroller

// PREDEFINES
void adc_init();
uint16_t adc_read(uint8_t ch);
float adc_read_sensor(int sensor);

// Functions
void adc_init()
{
	// Set ADC Voltage Reference
    // AREF = AVcc
    //ADMUX = (1<<REFS0);
    OUTPUT_HIGH(ADMUX, REFS0); // Maybe remove?
 
    // ADC Enable and pre-scaler of 128
    //ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);

    // Enable ADC
    OUTPUT_HIGH(ADCSRA, ADEN);

    // Set pre-scaler to 128
    // Optimal range is 50 - 200 MHz
    // 8000000/128 = 62500 = 62.5 MHz
    OUTPUT_HIGH(ADCSRA, ADPS2);
    OUTPUT_HIGH(ADCSRA, ADPS1);
    OUTPUT_HIGH(ADCSRA, ADPS0);

    // Enable ADC High Speed Mode
    //OUTPUT_HIGH(ADCSRB, ADHSM);

}

uint16_t adc_read(uint8_t ch)
{
    // select the corresponding channel 0~7
    // ANDing with '7' will always keep the value
    // of 'ch' between 0 and 7
    ch &= 0b00000111;  // AND operation with 7
    ADMUX = (ADMUX & 0xF8)|ch;     // clears the bottom 3 bits before ORing
 
    // start single conversion
    // write '1' to ADSC
    //ADCSRA |= (1<<ADSC);

    // Initiate single conversion
    OUTPUT_HIGH(ADCSRA, ADSC);
 
    // wait for conversion to complete
    // conversion is not complete when still '1'
    // ADSC becomes '0' again; conversion complete
    // till then, run loop continuously
    //while(ADCSRA & (1<<ADSC));
    while(INPUT_READ(ADCSRA, ADSC) == 1);
 
    return (ADC);
}

float adc_read_sensor(int sensor) {

	// Select voltage reference for necessary ADC readings
	if (sensor > 7) { // Adjust to read ADC8 to ADC13
		OUTPUT_LOW(ADMUX, REFS0);
		OUTPUT_HIGH(ADMUX, REFS1);

		// Adjust sensor number to loop back in byte
		// E.g. 8 becomes 0
		sensor = (sensor % 7) - 1;

	} else { // Adjust to read ADC0 to ADC7
		OUTPUT_HIGH(ADMUX, REFS0);
		OUTPUT_LOW(ADMUX, REFS1);
	}

  uint16_t adc_result = adc_read(sensor); // change number to sensor pin number

  float max_adc = 1023.0;
  float max_lcd_adc = (float)adc_result / max_adc; // value between 0 and 1

  return max_lcd_adc;
}