#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <msp430.h>
#include "HAL_LCD.h"
#include "uart.h"

#define DEBUG(...) do {uart_printf(__VA_ARGS__);} while(0);

void led_init() {
	P1SEL &= ~BIT0;
	P1DIR |= BIT0;
	P1OUT |= BIT0;
}

void init() {
	WDTCTL = WDTPW + WDTHOLD;
	led_init();
	uart_init();
	__enable_interrupt();
        LCD_init();
	LCD_backlightInit();
}

extern volatile uint8_t flag_uart;
extern volatile uint8_t buffer[140];
extern volatile uint8_t index;

uint8_t buffer_main[140];

int main (void)
{
	init();
	LCD_clear();
	while(1) {
		/*if(c==1) {
			n=n+1;
			if(n > 60) {
				c=0;
			}
		}
		else{
			n=n-1;
			if(n == 0)
				c = 1;
		}
		for(int i = 0; i < n; i++)
			DEBUG(".");
		DEBUG("\n\r");*/
		if(flag_uart){
			while(flag_uart){
				flag_uart=0;
				__delay_cycles(10000);
			}
			__disable_interrupt();
			buffer[index] = '\0';
			memcpy(buffer_main,(void*)buffer,index);
			index = 0;
			__enable_interrupt();
			LCD_printf("%s",buffer_main);
			memset((void*)buffer_main,0,140);
			flag_uart = 0;
		}
	}
	return 0;
}
