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
#include "rtc.h"

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
char read_buffer[10];

int write_log(uint16_t k){

    FIL file;
    FRESULT fr;

    fr = f_open(&file, "log.txt", FA_OPEN_ALWAYS | FA_WRITE);
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


int read_file(){

    FIL file;
    FRESULT fr;
    FILINFO fno;
    fr = f_stat("log.txt",&fno);

    while(fr!= FR_OK && fr!=FR_NO_FILE ){
      fr = f_stat("log.txt",&fno);
    }
    LCD_printf("\n\r%d",fr);
    if(fr == FR_NO_FILE){
      fr = f_open(&file, "log.txt", FA_CREATE_NEW | FA_READ);
      LCD_printf("Archivo no encontrado, se creara uno");
    } 
    else if(fr == FR_OK){
      fr = f_open(&file, "log.txt", FA_OPEN_EXISTING | FA_READ);
      LCD_printf("Archivo encontrado");
    }
    else{
      LCD_printf("inv");
      return -1;
    }

    UINT a;
    fr = f_read(&file, read_buffer, 10, &a);

    if(fr != FR_OK) return -1;

    f_close(&file);
    if(fr != FR_OK) return -1;

    num_push = (uint8_t)atoi(read_buffer);


    return 1;
}

void print_date(){
    FRESULT fr;
    FILINFO fno;
    fr = f_stat("log.txt",&fno);
    while(fr!= FR_OK){
      fr = f_stat("log.txt",&fno);
    }
    TIME_DATA data = RTC_get_time();

    DWORD datef=fno.fdate;
    DWORD timef=fno.ftime;

    LCD_printf("\n\r%u %u",data.minute,data.second); 

}

void LCD_print_time(TIME_DATA data) {
	//LCD_set_pointer(0,0);
	LCD_printf("\n\r%u/%02u/%02u\n\r", data.year, data.month, data.day);
	LCD_printf("%02u:%02u:%02u", data.hour, data.minute, data.second);
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

    while(f_mount(0, &fs) != FR_OK);
    csd_bf a;
    memcpy(&a,csd,16);
    LCD_cleanpage(0);
    read_file();


    while(1){

     // P8DIR |= BIT1;
     // P8OUT ^= BIT1;
      if(flag2){
	LCD_reset_address();
        LCD_printf("\rVeces: %04u",num_push);
        write_log(num_push);
        LCD_print_time(RTC_get_time());
      }

      //__delay_cycles(1000);
    }






  }
  else{//Fail
	LCD_printf("Fail");
  }
}



int main (void)
{
	init();
        _NOP();
	led_init();
        _NOP();
	SDCard_init();
        _NOP();
        button_init();
	//timer_init();
	//timer_start();
 	//pot_init();
    	//adc_init();
	//LCD_clear();
	RTC_init();
	app_run2();

	while(1);

	return 0;
}
