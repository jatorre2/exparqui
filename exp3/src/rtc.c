#include <msp430.h>
#include <stdint.h>

#include "rtc.h"
#include "rtc_init.h"
#include "app.h"

//In uniarch there is no more signal.h to sugar coat the interrupts definition, so we do it here
#define interrupt(x) void __attribute__((interrupt (x))) 

volatile struct TIME_DATA last_time_data;
volatile struct TIME_DATA temp_time_data;

TIME_DATA RTC_get_time(void) {
	TIME_DATA data;
	data.year = RTCYEAR;
	data.month = RTCMON;
	data.day  = RTCDAY;
	data.hour = RTCHOUR;
	data.minute = RTCMIN;
	data.second = RTCSEC;
	return data;	
}

void RTC_set_values(TIME_DATA time_data) {
	RTCYEAR    = time_data.year;
	RTCMON	   = time_data.month;
	RTCDAY     = time_data.day;
	RTCHOUR    = time_data.hour;
	RTCMIN 	   = time_data.minute;
	RTCSEC	   = time_data.second;
}

void LFXT_Start(void)
{
  // If the drive setting is not already set to maximum
  // Set it to max for LFXT startup
  if ((UCSCTL6 & XT1DRIVE_3)!= XT1DRIVE_3) { 
    UCSCTL6_L |= (XT1DRIVE1_L | XT1DRIVE0_L); // Highest drive setting for XT1startup
  }

  while (SFRIFG1 & OFIFG) {   // Check OFIFG fault flag
    UCSCTL7 &= ~(DCOFFG | XT1LFOFFG | XT2OFFG); // Clear OSC flaut Flags fault flags
    SFRIFG1 &= ~OFIFG;        // Clear OFIFG fault flag
  }
  
  UCSCTL6 = (UCSCTL6 & ~(XT1DRIVE_3)) | (0x0040); // set requested Drive mode
}

void RTC_init() {
	// Turn LFXT1 on
	P5OUT &= ~BIT4;
	UCSCTL6 = 0x00;
	//LFXT_Start();
	// RTC configuration
	RTCCTL01 = RTCMODE + RTCHOLD; // RTC calendar, Binary count, events every minute
	//RTCCTL0 = RTCTEVIE | RTCRDYIE; // Time events and RTC ready interrupts enabled
	RTCCTL0 = 0x00; //RTCRDYIE; // RTC ready interrupts enabled
	RTCCTL0 &= ~(RTCTEVIFG | RTCRDYIFG); // Turn down the interrupt flags just in case.
	TIME_DATA init_data;
	init_data.year = INITYEAR;
	init_data.month = INITMON;
	init_data.day = INITDAY;
	init_data.hour = INITHOUR;
	init_data.minute = INITMIN;
	init_data.second = INITSEC;
	RTC_set_values(init_data);
	
	RTCCTL01 &= ~RTCHOLD; 
	RTCPS1CTL = RT1IP_5;
	RTCPS0CTL = RT0IP_7;
}
int RTC_update_time(TIME_DATA data) {
	if((RTCCTL1 & RTCRDY) == RTCRDY) {
		data.year   = RTCYEAR;
		data.month  = RTCMON;
		data.day    = RTCDAY;
		data.hour   = RTCHOUR;
		data.minute = RTCMIN;
		data.second = RTCSEC;
		return 0;
	}
	return -1;
}

void RTC_seconds_handler() {
//	LCD_print_time(last_time_data);
	P8DIR |= BIT2;
	P8OUT |= BIT2;
	__delay_cycles(200);
	P8OUT &= ~BIT2;
}

#ifndef RTCOFIFG
	#define RTCOFIFG BIT3
#endif
static volatile uint16_t rtc_time_event = 0;
static volatile uint16_t rtc_ready      = 0;
interrupt (RTC_VECTOR) RTC_ISR() {
	P8DIR |= BIT1;
	P8OUT ^= BIT1;
	rtc_time_event = (RTCCTL0 & RTCTEVIFG) == RTCTEVIFG;
	rtc_ready      = ((RTCCTL1 & RTCRDY) == RTCRDY);
	if(rtc_time_event) {
	//	RTC_event_handler();
	} 
	if(rtc_ready) {
		RTC_update_time(temp_time_data);
		if(temp_time_data.second != last_time_data.second) {
			last_time_data = temp_time_data;
			RTC_seconds_handler();
		}
	}
	RTCCTL0 &= ~(RTCTEVIFG | RTCRDYIFG | RTCAIFG | RTCOFIFG); // Turn off flags
}

