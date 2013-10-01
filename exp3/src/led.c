#include "led.h"
void led_init(void) {
	//LED_SEL &= ~(LED_2 | LED_3);
	//LED_DIR |=   LED_2 | LED_3;
	//LED_OUT |=  (LED_2 | LED_3);
	
	LED1_SEL &= ~LED_1;
	LED1_DIR |=  LED_1;
	LED1_OUT |=  LED_1;

}

// Toggles the led state
void led_toggle(uint8_t led_num) {
	switch(led_num) {
		case 1: LED1_OUT ^= LED_1;break;		
		//case 2: LED_OUT ^= LED_2; break;
		//case 3: LED_OUT ^= LED_3; break;
		default: break;
	}
}

// Changes the state of led 'led_num'
// state 0: led_num off
// state 1: led_num on
void led_state(uint8_t led_num, uint8_t state) {
	switch(led_num) {
		case 1: LED1_OUT = (state & BIT0) ? LED1_OUT | BIT0 : LED1_OUT & ~BIT0; break;
//		case 2: LED_OUT = (state & BIT0) ? LED_OUT | BIT2 : LED_OUT & ~BIT2; break;
//		case 3: LED_OUT = (state & BIT0) ? LED_OUT | BIT3 : LED_OUT & ~BIT3; break;
		default: break;
	}
}
