// For controlling motors for a line following robot

// PORTS AND PINS
#define MOTORRIGHT_FORWARD OCR1A
#define MOTORRIGHT_REVERSE OCR1B
#define MOTORLEFT_REVERSE OCR0A
#define MOTORLEFT_FORWARD OCR0B

// PREDEFINES
void setupMotor2AndTimer0();
void setupMotor1AndTimer1();


// Functions
void setupMotor2AndTimer0() {
	SET_OUTPUT(DDRB, 7); // Make Pin OC0A of PB7 an output
	SET_OUTPUT(DDRD, 0); // Make Pin OC0B or PD0 an output

	SET_OUTPUT(TCCR0A, 7); // Configure control register A Output A to PWM fast mode (OCR0A)
	SET_OUTPUT(TCCR0A, 5); // Configure Output B to PWM fast mode (OCR0B)
	SET_OUTPUT(TCCR0A, 1); // Fast PWM waveform
	SET_OUTPUT(TCCR0A, 0); // Fast PWM waveform 

	SET_OUTPUT(TIMSK0, 0); // Enable overflow interrupt

	OCR0A = 0; // Set forward channel to 100%
	OCR0B = 0; // Set backward channel to 0%

	SET_OUTPUT(TCCR0B, 2); // Start timer (prescaling @ 256)
}

void setupMotor1AndTimer1() {
	SET_OUTPUT(DDRB, 6); // Make Pin OC1B of PB6 an output
	SET_OUTPUT(DDRB, 5); // Make Pin OC1A of PB5 an output

	SET_OUTPUT(TCCR1A, 7); // Configure control register A of 16bit timer 1 for FAST PWM
	SET_OUTPUT(TCCR1A, 5); // Configure Output B for Fast PWM
	// Set PWM generation mode (Mode 5) 8-bit operation
	SET_OUTPUT(TCCR1B, 3); // WGM12
	SET_OUTPUT(TCCR1A, 0); // WGM10

	SET_OUTPUT(TIMSK1, 0); // Enable overflow interrupt

	OCR1A = 0; // Forward 100%
	OCR1B = 0; // Backward 0%

	SET_OUTPUT(TCCR1B, 2); // Start timer (no prescaling)
}