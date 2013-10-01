#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <in430.h>
#include <msp430.h>

#include "ff.h"
#include "HAL_SDCard.h"

void init() {
	WDTCTL = WDTPW + WDTHOLD;
	_BIS_SR(GIE);
}

void app_run() {

}

int main (void)
{
	init();
	app_run();
	while(1);
	return 0;
}
