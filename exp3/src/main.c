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




FATFS fs;
char write_buffer[10];

int write_log(uint16_t k){

    FIL file;
    FRESULT fr;

    fr = f_open(&file, "log.txt", FA_WRITE);
    if(fr != FR_OK) return -1;

    sprintf(write_buffer, "%d", k);
    UINT a;
    fr = f_write(&file, write_buffer, 10, &a);

    if(fr != FR_OK) return -1;

    f_close(&file);
    if(fr != FR_OK) return -1;
    return 1;
}

extern volatile uint8_t flag2,num_push;

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

void app_run2(){
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
    LCD_cleanpage(0);

    while(1){
      if(flag2){
	LCD_reset_address();
        LCD_printf("\rVeces: %04u",num_push);
      }
    }






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
        button_init();
	//timer_init();
	//timer_start();
 	//pot_init();
    	//adc_init();
	//LCD_clear();
	app_run2();
	_NOP();

	while(1);

	return 0;
}
