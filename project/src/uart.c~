#include "uart.h"

void uart_init(void)
{
	P4SEL |= BIT4 | BIT5;
	P4MAP4 = PM_UCA1TXD;
	P4MAP5 = PM_UCA1RXD;
	UCA1CTL1 |= UCSWRST;
	UCA1CTL0 = 0x00;
	UCA1CTL1 = UCSSEL__SMCLK | UCSWRST;
	// Para estos parametros consideren que el SMCLK corre a la frecuencia
	// de DCOCLKDIV, que es 1048576 Hz (DCO/2)
	// Revisen la pagina 760 del family guide para otros baudrates
	UCA1BR1 = (109 & 0xFF00) >> 8;                         
	UCA1BR0 = 109 & 0x00FF;
	UCA1MCTL = UCBRS_2 | UCBRF_0;
	UCA1CTL1 &= ~UCSWRST;
	UCA1IE |= UCRXIE;
	__delay_cycles(1000);
}

uint8_t uart_getchar(void)
{
    uint8_t rx_byte;

    while(!(UCA1IFG & UCRXIFG));
    rx_byte = UCA1RXBUF;
    return rx_byte;
}

int16_t uart_printf(const char * fmt, ...) {
        int16_t len;
        va_list args;
        va_start(args, fmt);
        len = vuprintf(&uart_putchar, fmt, args);
        va_end(args);
        return len;
}

__attribute__((critical)) int uart_putchar(int tx_byte)
{
    while(!(UCA1IFG & UCTXIFG));
    UCA1TXBUF = (uint8_t) tx_byte;
    return 0;
}

#ifndef interrupt
#define interrupt(x) void __attribute__((interrupt(x)))
#endif
interrupt(USCI_A1_VECTOR) USCI_A1_ISR(void)
{
	if(UCA1IFG & UCRXIFG)
	{
		P1OUT ^= BIT0;
		UCA1RXBUF;
	}
}
