#include "button.h"
void button_init(void) {
        BUTTON1_IE  &= ~BUTTON1_BIT; // Disable interrupts while setting up
        BUTTON1_DIR &= ~BUTTON1_BIT; // Treat as input (P1DIR7 = 0)
        BUTTON1_IES |=  BUTTON1_BIT; // Falling edge interrupt (1)
        BUTTON1_OUT |=  BUTTON1_BIT; // Pull up
        BUTTON1_REN |=  BUTTON1_BIT; // Pull up enabled
        BUTTON1_IFG &= ~BUTTON1_BIT;
        BUTTON1_SEL &= ~BUTTON1_BIT; // I/O function
        BUTTON1_IE  |=  BUTTON1_BIT; // Enable interrupt

        BUTTON2_IE  &= ~BUTTON2_BIT; // Disable interrupts while setting up
        BUTTON2_DIR &= ~BUTTON2_BIT; // Treat as input (P2DIR7 = 0)
        BUTTON2_IES |=  BUTTON2_BIT; // Falling edge interrupt (1)
        BUTTON2_OUT |=  BUTTON2_BIT; // Pull up
        BUTTON2_REN |=  BUTTON2_BIT; // Pull up enabled
        BUTTON2_IFG &= ~BUTTON2_BIT;
        BUTTON2_SEL &= ~BUTTON2_BIT; // I/O function
        BUTTON2_IE  |=  BUTTON2_BIT; // Enable interrupt

}





volatile uint8_t state_led = 0; 
volatile uint8_t flag1 = 0,flag2 = 0, is_in_LPM0 = 0, num_push = 0;

//In uniarch there is no more signal.h to sugar coat the interrupts definition, so we do it here
#ifndef interrupt
#define interrupt(x) void __attribute__((interrupt (x))) 
#endif
/*
interrupt (PORT1_VECTOR) PORT1_ISR()
{
	if (BUTTON1_IFG & BUTTON1_BIT)
	{
		
		if(flag1){
		  BUTTON1_IES |= BUTTON1_BIT; flag1 = 0;
		}
		else{
		  BUTTON1_IES &= ~BUTTON1_BIT; flag1 = 1;
		}

		// Put interrupt code here
		if(flag1 && flag2){
		  if(is_in_LPM0){
		    is_in_LPM0 = 0;
		    LPM0_EXIT;
		  }
		  else{
		    is_in_LPM0 = 1;
	    
		  }
		}
		
		else if(flag1){
		  TA0CCR0 = 0x0FFF;
		  state_led = (state_led+1)%4;
		
		  TA0CCR0>>=state_led;
		}
		// Tell the microcontroller that the interrupt has been processed
		BUTTON1_IFG &= ~BUTTON1_BIT;

	}
}
*/

interrupt (PORT2_VECTOR) PORT2_ISR()
{
	if (BUTTON2_IFG & BUTTON2_BIT)
	{
		if(flag2){
		  BUTTON2_IES |= BUTTON2_BIT; flag2 = 0;
		}
		else{
		  BUTTON2_IES &= ~BUTTON2_BIT; flag2 = 1;
		}

		if(flag2){
		  num_push++;
		}
	

		// Put interrupt code here

		// Tell the microcontroller that the interrupt has been processed
		BUTTON2_IFG &= ~BUTTON2_BIT;
	}
}
