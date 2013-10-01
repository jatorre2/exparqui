#include <msp430.h>
#include <stdint.h>

#define LED_1 BIT0
#define LED_2 BIT1
#define LED_3 BIT2

#define LED1_OUT P1OUT
#define LED1_SEL P1SEL
#define LED1_DIR P1DIR

#define LED_OUT P8OUT
#define LED_SEL P8SEL
#define LED_DIR P8DIR

void led_init(void);
void led_toggle(uint8_t led_num);
void led_state(uint8_t led_num, uint8_t state);
