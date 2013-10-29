#include <msp430.h>
#include <stdio.h>
#include "led.h"
void timer_init() {
	// Reset timer
	TA0CTL = TACLR;
	// Select ACLK, TimerA counter divisor in 1, halted,
	TA0CTL = TASSEL__ACLK | ID__1;
	// Compare mode
	TA0CCTL0 &= ~CAP;
	// Enables the CCR0 interrupt
	TA0CCTL0 |= CCIE;
}

void timer_start() {
	//Since the crystal attached to LFXT is 32768 Hz,
	//when the counter reaches this value, exactly 0.5 seconds should have passed
	TA0CCR0 = 0x0FFF;
	TA0CTL |= MC_1;//Sets the TimerA count mode in UP, and thus starting the Timer

}

void timer_disable() {
	//Halt timer
	TA0CTL &= ~MC_3;
	//Disables the CCR0 interrupt
	TA0CCTL0 &= ~CCIE;
}

void frequency_set(uint16_t f) {
    TA0CCR0 = 0x2FFF + (0x0FFF-f) * 0xD; //para variar desde 0x2FFF a 0xDFFF
}

void frequency_toggle() {
    TA0CCR0=TA0CCR0 / 2;
    if(TA0CCR0 < 0x0FFF)
        TA0CCR0 = 0xFFFF;
}


//In uniarch there is no more signal.h to sugar coat the interrupts definition, so we do it here
#ifndef interrupt
#define interrupt(x) void __attribute__((interrupt (x)))
#endif
//CCR0 timer interrupt, which toggles the LED
interrupt(TIMER0_A0_VECTOR) TIMERA0_ISR()
{
    if(TA0CTL & TAIFG) {
       // led_toggle(1);
    }
}
