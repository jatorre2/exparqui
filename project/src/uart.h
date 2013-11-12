#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <msp430.h>

extern void uart_init(void);
extern uint8_t uart_getchar(void);
extern int uart_putchar(int tx_byte);
int16_t uart_printf(const char * fmt, ...);

#endif /* UART_H */

