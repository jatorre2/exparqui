#include <stdlib.h>
#include <stdio.h>

#include <msp430.h>
#include "adc.h"


#define POT BIT5

#define POT_DIR P6DIR
#define POT_OUT P6OUT
#define POT_SEL P6SEL

//Inicializacion de puertos del ADC
void adc_init(void) {
    ADC12CTL0 &= ~ADC12ENC;

    //Set and Hold en 128bits
    ADC12CTL0 |= ADC12SHT0_6;

    //on-off
    ADC12CTL0 |= ADC12ON;


    ADC12CTL1 |= ADC12SHP;

    //MCLK
    ADC12CTL1 |= ADC12SSEL_2;

    ADC12MCTL0 |= ADC12INCH_5;

    ADC12CTL0 |= ADC12ENC;

}

//Inicializacion de puertos del potenciometro
void pot_init(void) {
    P8SEL &= ~BIT0;
    P8DIR |= BIT0;
    P8OUT |= BIT0;

    POT_SEL |= POT;
}

void adc_start() {
    ADC12CTL0 |= ADC12SC;
}

uint16_t adc_busy() {
    return ADC12CTL1 & ADC12BUSY;
}
