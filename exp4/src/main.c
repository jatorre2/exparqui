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
#include "maze.h"

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
    led_init();
    led_state(1,0);
    led_state(2,0);
    led_state(3,0);
    int8_t x0=0, y0=0, z0=0;
    int8_t x,y,z,xx,yy,zz;
    while(1) {
        if(ACCEL_INT_OUT) {
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
                __delay_cycles(100);
	            x0 = x;
                y0 = y;
                z0 = z;
            }

	    if(abs(xx)>20)
		led_state(1,1);
	    else
		led_state(1,0);
	    if(abs(yy)>20)
		led_state(2,1);
	    else
		led_state(2,0);
	    if(abs(zz)>20)
		led_state(3,1);
	    else
		led_state(3,0);




        }
    }
}

void app_run2(){

  LCD_clear();
  accel_init(); 
  led_state(1,0);
  LCD_draw_img();
  __delay_cycles(2000);
  //LCD_printf("holaa");
  button_init();
    int8_t x0=0, y0=0, z0=0;
    int8_t x,y,z,xx,yy,zz;

    while(1) {

        if(ACCEL_INT_OUT) {
	  //  LCD_printf("chao");
            x = accel_read(0x06);
            __delay_cycles(100);
            y = accel_read(0x07);
            __delay_cycles(100);
    //        z = accel_read(0x08);
            __delay_cycles(100);

            xx = x-x0;
            yy = y-y0;
            zz = z-z0;

           // LCD_reset_address();
           // LCD_printf("X: %d     \n\rY: %d     \n\rZ: %d    ", xx,yy,zz);
            if(flag1) {
                __delay_cycles(100);
	            x0 = x;
                y0 = y;
              //  z0 = z;
            }

	    if(xx>20)
		moveRight();
	    if(xx<-20)
		moveLeft();
	    if(yy>20)
		moveUp();
	    if(yy<-20)
		moveDown();
	    if(abs(zz)>20)
		led_state(3,1);
	    else
		led_state(3,0);




        }
    }

}

int main (void)
{
	init();
	_NOP();
	led_init();
	//timer_init();
	//timer_start();
 	//pot_init();
    	//adc_init();
	LCD_clear();
	app_run2();

	//app_run2();




	return 0;
}
