// For utilising ADC inputs with a microcontroller

// PREDEFINES
void adc_init();
uint16_t adc_read(uint8_t ch);
float adc_read_sensor(int sensor);

// Functions
void adc_init()
{
    // AREF = AVcc
    ADMUX = (1<<REFS0);
 
    // ADC Enable and pre-scaler of 128
    // 8000000/128 = 62500
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

uint16_t adc_read(uint8_t ch)
{
    // select the corresponding channel 0~7
    // ANDing with '7' will always keep the value
    // of 'ch' between 0 and 7
    //ch &= 0b00000111;  // AND operation with 7
    ADMUX = (ADMUX & 0xF8)|ch;     // clears the bottom 3 bits before ORing
 
    // start single conversion
    // write '1' to ADSC
    ADCSRA |= (1<<ADSC);
 
    // wait for conversion to complete
    // ADSC becomes '0' again
    // till then, run loop continuously
    while(ADCSRA & (1<<ADSC));
 
    return (ADC);
}

float adc_read_sensor(int sensor) {

  uint16_t adc_result = adc_read(sensor); // change number to sensor pin number

  float max_adc = 1023.0;
  float max_lcd_adc = (float)adc_result / max_adc; // value between 0 and 1

  // char debug[20];
  // snprintf(debug, 20, "%f", (float)adc_result);
  // show_debug_message(debug);

  return max_lcd_adc;
}