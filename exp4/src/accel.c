#include <msp430.h>
#include <stdlib.h>
#include <stdio.h>
#include "accel.h"

uint8_t accel_read(uint8_t address) {
    uint8_t result;

    address = address << 2;

    ACCEL_CS_OUT &= ~ACCEL_CS_BIT;

    UCA0TXBUF = address;

    while (!(UCA0IFG & UCRXIFG)) ;

    UCA0RXBUF;
    UCA0TXBUF = 0x00;

    while(UCA0STAT & UCBUSY);
    //while(! ACCEL_INT)

    result = UCA0RXBUF;

    ACCEL_CS_OUT |= ACCEL_CS_BIT;

    return result;
}

void accel_write(uint8_t address, uint8_t data){

    address = address << 2;
    address |= 0x02;
    ACCEL_CS_OUT &= ~ACCEL_CS_BIT;

    UCA0TXBUF = address;

    while(UCA0STAT & UCBUSY);

    UCA0RXBUF;
    UCA0TXBUF = data;

    while(UCA0STAT & UCBUSY);
    //while(! ACCEL_INT)


    ACCEL_CS_OUT |= ACCEL_CS_BIT;

}

void accel_init() {

    //CS in low
    ACCEL_CS_SEL &= ~ACCEL_CS_BIT;
    ACCEL_CS_DIR |= ACCEL_CS_BIT;
    ACCEL_CS_OUT &= ~ACCEL_CS_BIT;

    //Power on 3.6
    ACCEL_PWR_SEL &= ~ACCEL_PWR_BIT;
    ACCEL_PWR_DIR |= ACCEL_PWR_BIT;
    ACCEL_PWR_OUT |= ACCEL_PWR_BIT;

    //Setup 3.3, 3.4 to simo, somi
    ACCEL_SOMI_SEL |= ACCEL_SOMI_BIT;
    ACCEL_SIMO_SEL |= ACCEL_SIMO_BIT;

    //Setup 2.7 to UCA0CLK
    ACCEL_SCK_SEL |= ACCEL_SCK_BIT;

    // Initialize USCI_A0 for SPI Master operation
    // Put state machine in reset
    UCA0CTL1 |= UCSWRST;
    //3-pin, 8-bit SPI master
    UCA0CTL0 = UCCKPH | UCMSB | UCMST | UCMODE_0 | UCSYNC;
    // Clock phase - data captured first edge, change second edge
    // MSB
    // Use SMCLK, keep RESET
    UCA0CTL1 = UCSSEL_2 | UCSWRST;
    UCA0BR0 = 20;
    UCA0BR1 = 0;
    // Release USCI state machine
    UCA0CTL1 &= ~UCSWRST;
    UCA0IFG &= ~UCRXIFG;


    __delay_cycles(10000);

    accel_write(0x02, 0x92);

    ACCEL_CS_OUT |= ACCEL_CS_BIT;

}


#ifndef interrupt
#define interrupt(x) void __attribute__((interrupt (x)))
#endif
interrupt (PORT2_VECTOR) PORT2_ISR()
{
	if (ACCEL_INT_IFG & ACCEL_INT_BIT){

		// Tell the microcontroller that the interrupt has been processed
		ACCEL_INT_IFG &= ~ACCEL_INT_BIT;

        }
}

