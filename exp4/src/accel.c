#include <msp430.h>
#include <stdlib.h>
#include <stdio.h>



//Pines
#define ACCEL_CS_BIT BIT5 //CS P3.5
#define ACCEL_SIMO_BIT BIT3 //SIMO P3.3
#define ACCEL_INT_BIT BIT5 //INT P2.5
#define ACCEL_SCK_BIT BIT7 //SCK P2.7
#define ACCEL_PWR_BIT BIT6 //PWR P3.6
#define ACCEL_SOMI_BIT BIT4 //SOMI P3.4

//Puertos

#define ACCEL_CS_OUT P3OUT
#define ACCEL_CS_SEL P3SEL
#define ACCEL_CS_DIR P3DIR
#define ACCEL_CS_IN  P3IN
#define ACCEL_CS_REN P3REN
#define ACCEL_CS_IE  P3IE
#define ACCEL_CS_IES P3IES
#define ACCEL_CS_IFG P3IFG

#define ACCEL_SIMO_OUT P3OUT
#define ACCEL_SIMO_SEL P3SEL
#define ACCEL_SIMO_DIR P3DIR
#define ACCEL_SIMO_IN  P3IN
#define ACCEL_SIMO_REN P3REN
#define ACCEL_SIMO_IE  P3IE
#define ACCEL_SIMO_IES P3IES
#define ACCEL_SIMO_IFG P3IFG

#define ACCEL_INT_OUT P2OUT
#define ACCEL_INT_SEL P2SEL
#define ACCEL_INT_DIR P2DIR
#define ACCEL_INT_IN  P2IN
#define ACCEL_INT_REN P2REN
#define ACCEL_INT_IE  P2IE
#define ACCEL_INT_IES P2IES
#define ACCEL_INT_IFG P2IFG

#define ACCEL_SCK_OUT P2OUT
#define ACCEL_SCK_SEL P2SEL
#define ACCEL_SCK_DIR P2DIR
#define ACCEL_SCK_IN  P2IN
#define ACCEL_SCK_REN P2REN
#define ACCEL_SCK_IE  P2IE
#define ACCEL_SCK_IES P2IES
#define ACCEL_SCK_IFG P2IFG

#define ACCEL_PWR_OUT P3OUT
#define ACCEL_PWR_SEL P3SEL
#define ACCEL_PWR_DIR P3DIR
#define ACCEL_PWR_IN  P3IN
#define ACCEL_PWR_REN P3REN
#define ACCEL_PWR_IE  P3IE
#define ACCEL_PWR_IES P3IES
#define ACCEL_PWR_IFG P3IFG

#define ACCEL_SOMI_OUT P3OUT
#define ACCEL_SOMI_SEL P3SEL
#define ACCEL_SOMI_DIR P3DIR
#define ACCEL_SOMI_IN  P3IN
#define ACCEL_SOMI_REN P3REN
#define ACCEL_SOMI_IE  P3IE
#define ACCEL_SOMI_IES P3IES
#define ACCEL_SOMI_IFG P3IFG





uint8_t accel_read(uint8_t address) {
    uint8_t result;

    address = address << 2;

    ACCEL_CS_OUT &= ~ACCEL_CS_BIT;

    UCA0TXBUF = address;

    while(UCA0STAT & UCBUSY);

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
    ACCEL_PWR_SEL |= ACCEL_PWR_BIT;
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
    UCA0BR0 = 0x40;
    UCA0BR1 = 0;
    // Release USCI state machine
    UCA0CTL1 &= ~UCSWRST;
    UCA0IFG &= ~UCRXIFG;

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

