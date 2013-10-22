#include <msp430.h>


//Pines
#define ACCEL_CS_BIT BIT5 //CS P3.5
#define ACCEL_SIMO_BIT BIT3 //SIMO P3.3
#define ACCEL_INT_BIT BIT5 //INT P2.5
#define ACCEL_SCK_BIT BIT7 //SCK P2.7
#define ACCEL_PWR_BIT BIT6 //PWR P3.6
#define ACCEL_SOMI_BIT BIT4 //SOMI P3.4

//Puertos

#define ACCEL_CS_OUT P3OUT
#define ACCEL_CS_SEL P3SEL
#define ACCEL_CS_DIR P3DIR
#define ACCEL_CS_IN  P3IN  
#define ACCEL_CS_REN P3REN 
#define ACCEL_CS_IE  P3IE  
#define ACCEL_CS_IES P3IES 
#define ACCEL_CS_IFG P3IFG

#define ACCEL_SIMO_OUT P3OUT
#define ACCEL_SIMO_SEL P3SEL
#define ACCEL_SIMO_DIR P3DIR
#define ACCEL_SIMO_IN  P3IN  
#define ACCEL_SIMO_REN P3REN 
#define ACCEL_SIMO_IE  P3IE  
#define ACCEL_SIMO_IES P3IES 
#define ACCEL_SIMO_IFG P3IFG

#define ACCEL_INT_OUT P2OUT
#define ACCEL_INT_SEL P2SEL
#define ACCEL_INT_DIR P2DIR
#define ACCEL_INT_IN  P2IN  
#define ACCEL_INT_REN P2REN 
#define ACCEL_INT_IE  P2IE  
#define ACCEL_INT_IES P2IES 
#define ACCEL_INT_IFG P2IFG

#define ACCEL_SCK_OUT P2OUT
#define ACCEL_SCK_SEL P2SEL
#define ACCEL_SCK_DIR P2DIR
#define ACCEL_SCK_IN  P2IN  
#define ACCEL_SCK_REN P2REN 
#define ACCEL_SCK_IE  P2IE  
#define ACCEL_SCK_IES P2IES 
#define ACCEL_SCK_IFG P2IFG

#define ACCEL_PWR_OUT P3OUT
#define ACCEL_PWR_SEL P3SEL
#define ACCEL_PWR_DIR P3DIR
#define ACCEL_PWR_IN  P3IN  
#define ACCEL_PWR_REN P3REN 
#define ACCEL_PWR_IE  P3IE  
#define ACCEL_PWR_IES P3IES 
#define ACCEL_PWR_IFG P3IFG

#define ACCEL_SOMI_OUT P3OUT
#define ACCEL_SOMI_SEL P3SEL
#define ACCEL_SOMI_DIR P3DIR
#define ACCEL_SOMI_IN  P3IN  
#define ACCEL_SOMI_REN P3REN 
#define ACCEL_SOMI_IE  P3IE  
#define ACCEL_SOMI_IES P3IES 
#define ACCEL_SOMI_IFG P3IFG


void accel_init() {

    //Power on

}

#ifndef interrupt
#define interrupt(x) void __attribute__((interrupt (x))) 
#endif
interrupt (PORT2_VECTOR) PORT2_ISR()
{
	if (ACCEL_INT_IFG & ACCEL_INT_BIT){




		// Tell the microcontroller that the interrupt has been processed
		ACCEL_INT_IFG &= ~ACCEL_INT_BIT;

        }    
}
