// Code for switchs

#define SWITCH_PORT PORTC
#define SWITCH_DDR DDRC
#define SWITCH_PIN PINC
#define SWITCH_TOP_PIN 6
#define SWITCH_BOT_PIN 7

void switches_init() {
	SET_INPUT(SWITCH_DDR, SWITCH_TOP_PIN);
	SET_INPUT(SWITCH_DDR, SWITCH_BOT_PIN);
}

