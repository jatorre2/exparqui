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
#include "ff.h"
#include "HAL_SDCard.h"
#include "mmc.h"
#include "integer.h"

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


void app_runCSD(){
  LCD_clear();
  BYTE csd[16];
  if(detectCard(csd)){
   LCD_printf("Tarjeta detectada\n");
    //for(uint8_t i=0;i<16;i++)
      //LCD_printf("%02x-",csd[i]);

    for(uint8_t i=0; i<8;i++){
      BYTE swap = csd[15-i];
      csd[15-i] = csd[i];
      csd[i] = swap;
    }

    csd_bf a;
    memcpy(&a,csd,16);

   if(a.CSD_STRUCTURE == 0)
     LCD_printf("CSD Version 1.0\n\r");
   else if(a.CSD_STRUCTURE == 1)
     LCD_printf("CSD Version 2.0\n\r");
   

    uint64_t size_mmc = (uint64_t)(a.C_SIZE+1);
    uint64_t multiplier = (uint64_t)1<<(uint64_t)(a.C_SIZE_MULT+2+a.READ_BL_LEN);
    size_mmc*=multiplier;

    LCD_printf("Capacidad: \n\r%llu\n\r",size_mmc);
    LCD_printf("Bytes por sector: %u",a.READ_BL_LEN);
    /*
    if(version == 0){
      LCD_printf("Version de la tarjeta: 1.0");

    }
    else{
      LCD_printf("Version de la tarjeta: 2.0");
    }*/

  }
  else{//Fail
	LCD_printf("Fail");
  }
}


int main (void)
{
	init();
	led_init();
	SDCard_init();
	//timer_init();
	//timer_start();
 	//pot_init();
    	//adc_init();
	//LCD_clear();
	app_runCSD();
	_NOP();

	while(1);

	return 0;
}
