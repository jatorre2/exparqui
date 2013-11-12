#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <msp430.h>
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
}

int main (void)
{
	init();
	uint8_t n = 0;
	uint8_t c = 1;
	while(1) {
		if(c==1) {
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
		DEBUG("\n\r");
	}
	return 0;
}
