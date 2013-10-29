#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <in430.h>

#include "msp430.h"
#include "HAL_LCD.h"
#include "test.h"
#include "led.h"
#include "timer.h"
#include "button.h"
#include "adc.h"
#include "accel.h"

void init() {
	WDTCTL = WDTPW + WDTHOLD;
  	__delay_cycles(3000);
	LCD_init();
  	LCD_backlightInit();
	_BIS_SR(GIE);
	UCSCTL4 |= SELM__DCOCLK;
	__enable_interrupt();
}

extern volatile uint8_t flag1,flag2, is_in_LPM0, state_led;

void app_run() {
    button_init();
    accel_init();
    int8_t x0=0, y0=0, z0=0;
    int8_t x,y,z,xx,yy,zz;
    while(1) {
        x = accel_read(0x06);
        __delay_cycles(100);
        y = accel_read(0x07);
        __delay_cycles(100);
        z = accel_read(0x08);
        __delay_cycles(100);

        xx = x-x0;
        yy = y-y0;
        zz = z-z0;

        LCD_reset_address();
        LCD_printf("X: %d     \n\rY: %d     \n\rZ: %d    ", xx,yy,zz);
        if(flag1) {
            x0 = x;
            y0 = y;
            z0 = z;
        }
    }
}


void app_run1() {
	button_init();
	//LCD_reset_address();
	uint32_t percentage = (uint32_t)TA0CCR0*100;
	percentage/=(uint32_t)0x0FFF;
	LCD_printf("\rT: %04d (%03u%)",TA0CCR0, percentage);
	while(1){
	  if(flag1 && flag2){
	    if(is_in_LPM0){
		_NOP();
		LPM0;
	    }
	  }
	  else if(flag1){
		  //LCD_cleanpage(0);
		  //LCD_reset_address();
		  uint32_t percentage = (uint32_t)TA0CCR0*100;
		  percentage/=(uint32_t)0x0FFF;
		  LCD_printf("\rT: %04d (%03u%)",TA0CCR0, percentage);
	  }


	}


}

void app_run2() {
   adc_start();
   uint16_t newv = 1;
   uint16_t oldv = 0;
   uint16_t total = 0;
   uint8_t c = 0; //Contador de muestras tomadas
   uint8_t n = 5; //Numero de muestras a promediar
   while(1) {
       while(adc_busy()) {
       //LCD_printf(".");
       }
       //LCD_printf("wena");
       total += ADC12MEM0;
       c++;
       if(c >= n) {
        c = 0;
        newv = total / n;
        total= 0;
        if(newv != oldv) {
            oldv = newv;
            uint32_t per = (uint32_t)newv*100;
	    per/=(uint32_t)4095;

            LCD_printf("\rT: %04d (%03u%)",newv,per);
            frequency_set(newv);
        }
       }
       adc_start();
   }
}



int main (void)
{
	init();
	led_init();
	timer_init();
	timer_start();
 	pot_init();
    	adc_init();
	LCD_clear();
	app_run();

	//app_run2();




	return 0;
}
