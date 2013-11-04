#include <stdint.h>

void LCD_init(void);
void LCD_backlightInit(void);
void LCD_writeCommand(uint8_t* sCmd, uint8_t i);
void LCD_setAllPixels(uint8_t set);
void LCD_writeData(uint8_t data);
void LCD_clear(void);
void LCD_setPixel(uint8_t i, uint8_t j,uint8_t set);
uint8_t LCD_getPixel(uint8_t i, uint8_t j);
void LCD_cleanpage(uint8_t page);
int LCD_putchar(int character);
int16_t LCD_printf(const char * fmt, ...);
void LCD_draw_img();
void LCD_adjust_contrast(uint8_t set);
void LCD_reset_address();
void LCD_println(const char * st);
