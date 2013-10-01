#include "test.h"


void wait_for_button_and_clear(void) {
	while(P1IN & BIT7);
	while((P1IN & BIT7) == 0);
	LCD_clear();
}

void test(void) {
	// Init button
	P1SEL &= ~BIT7;
	P1DIR &= ~BIT7;
	P1REN |= BIT7;
	P1OUT |= BIT7;

	//Set 1:
	// Test \n and \r behaviour
	LCD_printf("Test1\r\n");
	LCD_printf("Test2\n");
	LCD_printf("Test3\n");
	wait_for_button_and_clear();

	//Set 2 :
	// Print N numbers with a newline
	for(uint8_t i = 0; i < 20; i++) {
	  LCD_printf("%02u\r\n", i);
	}
	wait_for_button_and_clear();

	// Set 3:
	// Test tabs
	LCD_printf("Tab test:\r\n");
	LCD_printf("\tTabbed line\r\n");
	wait_for_button_and_clear();

	// Set 4:
	// Test \r overwrite
	LCD_printf("Test1\r");
	LCD_printf("Test2");

	// Set 5:
	// Print all chars
	for(uint8_t i = ' '; i < (' ' + 105); i++) {
	  LCD_printf("%c", i);
	}
	while(1); 
}

