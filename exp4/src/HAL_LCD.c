#include <stdlib.h>
#include <stdio.h>

#include <msp430.h>
#include "HAL_LCD.h"
#include "FontTable.h"
#include "output.h"

// For all commands, CD signal must = 0
#define SET_POWER_CONTROL             0x2F  //Set Power control - booster, regulator, and follower
                                            // on
#define SET_ELECTRONIC_VOLUME_MSB     0x81  //Set Electronic Volume "PM" to adjust contrast
#define SET_ELECTRONIC_VOLUME_LSB     0x0F  //Set Electronic Volume "PM" to adjust contrast (PM =
                                            // last 5 bits)
#define SET_ALL_PIXEL_ON              0xA4  //Disable all pixel on (last bit 1 to turn on all pixels
                                            // - does not affect memory)
#define SET_DISPLAY_ENABLE            0xAF  //Enable display (exit sleep mode & restore power)
#define SET_SEG_DIRECTION             0xA1  //Mirror SEG (column) mapping (set bit0 to mirror
                                            // display)
#define SET_COM_DIRECTION             0xC8  //Mirror COM (row) mapping (set bit3 to mirror display)

//Extras
#define SET_PAGE_ADDRESS	      0xB0
#define SET_COL_ADDRESS		      0x00


// Pins from MSP430 connected to LCD
#define CD              BIT6
#define CS              BIT4
#define RST             BIT7
#define BACKLT          BIT6
#define SPI_SIMO        BIT1
#define SPI_CLK         BIT3

// Ports
#define CD_RST_DIR      P5DIR
#define CD_RST_OUT      P5OUT
#define CS_BACKLT_DIR   P7DIR
#define CS_BACKLT_OUT   P7OUT
#define CS_BACKLT_SEL   P7SEL
#define SPI_SEL         P4SEL
#define SPI_DIR         P4DIR


//Globals 


uint8_t screen[8][102];
uint8_t curr_page=0,curr_col=0;




// LCD Initialization Commands
uint8_t LCD_initMacro[] = {
    SET_POWER_CONTROL,
    SET_ELECTRONIC_VOLUME_MSB,
    SET_ELECTRONIC_VOLUME_LSB,
    SET_DISPLAY_ENABLE,
    SET_SEG_DIRECTION,
    SET_COM_DIRECTION,
};

static const uint8_t n_init_commands = 6;

void LCD_init(void)
{
    // Port initialization for LCD operation
    CD_RST_DIR |= RST;
    // Reset is active low
    CD_RST_OUT &= ~RST;
    // Reset is active low
    CD_RST_OUT |= RST;
    // Chip select for LCD
    CS_BACKLT_DIR |= CS;
    // CS is active low
    CS_BACKLT_OUT &= ~CS;
    // Command/Data for LCD
    CD_RST_DIR |= CD;
    // CD Low for command
    CD_RST_OUT &= ~CD;

    // P4.1 option select SIMO
    SPI_SEL |= SPI_SIMO;
    SPI_DIR |= SPI_SIMO;
    // P4.3 option select CLK
    SPI_SEL |= SPI_CLK;
    SPI_DIR |= SPI_CLK;

    // Initialize USCI_B1 for SPI Master operation
    // Put state machine in reset
    UCB1CTL1 |= UCSWRST;
    //3-pin, 8-bit SPI master
    UCB1CTL0 = UCCKPH | UCMSB | UCMST | UCMODE_0 | UCSYNC;
    // Clock phase - data captured first edge, change second edge
    // MSB
    // Use SMCLK, keep RESET
    UCB1CTL1 = UCSSEL_2 | UCSWRST;
    UCB1BR0 = 0x02;
    UCB1BR1 = 0;
    // Release USCI state machine
    UCB1CTL1 &= ~UCSWRST;
    UCB1IFG &= ~UCRXIFG;

    LCD_writeCommand(LCD_initMacro, n_init_commands);

    // Deselect chip
    CS_BACKLT_OUT |= CS;
}

void LCD_backlightInit(void)
{
    // Turn on Backlight
    CS_BACKLT_SEL &= ~BACKLT;
    CS_BACKLT_DIR |=  BACKLT;
    CS_BACKLT_OUT |=  BACKLT;
}

void LCD_writeCommand(uint8_t *sCmd, uint8_t cmd_len)
{
    // CS Low
    P7OUT &= ~CS;
    // CD Low
    P5OUT &= ~CD;
    while (cmd_len)
    {
        // USCI_B1 TX buffer ready?
        while (!(UCB1IFG & UCTXIFG)) ;
        // Transmit data
        UCB1TXBUF = *sCmd;
        // Increment the pointer on the array
        sCmd++;
        // Decrement the Byte counter
        cmd_len--;
    }
    // Wait for all TX/RX to finish
    while (UCB1STAT & UCBUSY) ;
    // Dummy read to empty RX buffer and clear any overrun conditions
    UCB1RXBUF;
    // CS High
    P7OUT |= CS;
}


void LCD_setAllPixels(uint8_t set)
{
    uint8_t cmd[] = {SET_ALL_PIXEL_ON + set};
    LCD_writeCommand(cmd, 1);
}

void LCD_writeData(uint8_t data)
{
    // CS Low
    P7OUT &= ~CS;
   //P7OUT &= ~CS;
    
    P5OUT |= CD;
    // USCI_B1 TX buffer ready?
    while (!(UCB1IFG & UCTXIFG)) ;
    // Transmit data
    UCB1TXBUF = data;
    // Wait for all TX/RX to finish
    while (UCB1STAT & UCBUSY) ;
    // Dummy read to empty RX buffer and clear any overrun conditions
    UCB1RXBUF;
    // CS High
    P7OUT |= CS;
}

void LCD_clear(void){
    for(uint8_t i=0x00;i<8;i++){
	for(uint8_t j=0x00;j<102;j++){

	uint8_t lsb= (j & 0x0F);
	uint8_t msb= (j & 0xF0)>>4;

	uint8_t cmd[] = {SET_PAGE_ADDRESS+i,lsb,msb+0x10};
        LCD_writeCommand(cmd, 3);
	LCD_writeData(0x00);
	//LCD_setAllPixels(1);
        screen[i][j] = 0;
        }
    }

    curr_page=0,curr_col=0;

}

//Enciende o apaga el pixel en la i-esima fila, j-esima columna
//0<=i<=63, 0<=j<=101

void LCD_setPixel(uint8_t i, uint8_t j,uint8_t set){

    uint8_t lsb = (j & 0x0F);
    uint8_t msb = (j & 0xF0)>>4;
    uint8_t p = 7-(i/8);
    uint8_t bit = 7-(i%8);

    uint8_t cmd[] = {SET_PAGE_ADDRESS+p,lsb,msb+0x10};

    if(set==1)
	screen[p][j]|=1<<bit;
    else
	screen[p][j]&= ~(1<<bit);	 

    LCD_writeCommand(cmd, 3);    
    LCD_writeData(screen[p][j]); 


}


//page from 0 to 7 (up to bottom)
void LCD_cleanpage(uint8_t page){
    for(uint8_t i=0; i<102;i++){
        uint8_t lsb = (i & 0x0F);
        uint8_t msb = (i & 0xF0)>>4;
        uint8_t cmd[] = {SET_PAGE_ADDRESS+0x07-page,lsb,msb+0x10};
        LCD_writeCommand(cmd, 3);    
        LCD_writeData(0x00);
    }

}


// Auxiliary function for LCD_printf, this one does all the work.
int LCD_putchar(int character) {
    character = (uint8_t) character;
    // Handle the character



   if(character == '\n'){
       curr_page++;
   }
   else if (character == '\t'){
       curr_col += (0x06 * 2);
       if(curr_col>=102){
           curr_col = 0;
           curr_page++;
       }
   }
   else if (character == '\r'){
       curr_col = 0;
   }
   else{

   if(character >= 96+0x20 || character <0x20)
	character = (uint8_t)'?';

    uint8_t start = character-0x20;
    
    for(uint8_t i=0;i<6;i++){
        uint8_t lsb = ((curr_col+i) & 0x0F);
        uint8_t msb = ((curr_col+i) & 0xF0)>>4;
        uint8_t cmd[] = {SET_PAGE_ADDRESS+0x07-curr_page,lsb,msb+0x10};
        LCD_writeCommand(cmd, 3);    
        screen[curr_page][curr_col] = FONT6x8[6*start+i];
        LCD_writeData(screen[curr_page][curr_col]);      
    }
     
    curr_col+=0x06;
    
    if(curr_col>=102){
        curr_col = 0;
        curr_page++;
    }
    

  
  }

  if(curr_page<7){
   //   LCD_cleanpage(curr_page+1);
  }
  else if(curr_page>=8){
    curr_page = 0;
  }

  return 0;

}


int16_t LCD_printf(const char * fmt, ...) {
    int16_t len;
    va_list args;
    va_start(args, fmt);
    len = vuprintf(&LCD_putchar, fmt, args);
    va_end(args);
    return len;
}

void LCD_draw_img(){

  uint16_t ind=2;

    for(uint8_t j = 0; j<8;j++){
      for(uint8_t i = 0; i<102;i++){
        uint8_t lsb = (i & 0x0F);
        uint8_t msb = (i & 0xF0)>>4;
        uint8_t cmd[] = {SET_PAGE_ADDRESS+7-j,lsb,msb+0x10};
        LCD_writeCommand(cmd, 3);     
        LCD_writeData(image[ind]);      
        ind++;     
      }
    }


}


uint8_t contrast = 32;

void LCD_adjust_contrast(uint8_t set){
    if(set>0 && contrast<63)
	contrast++;
    else if(contrast>0)
	contrast--;   
    uint8_t cmd[] = {0x81,contrast};
    LCD_writeCommand(cmd, 2);    

}

void LCD_reset_address(){
    curr_page=0,curr_col=0;	
}

void LCD_println(const char * st) {
    LCD_printf("%s\n\r",st);
}
