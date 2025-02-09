#include "lcd_app.h"

void lcdSprintf(uint8_t Line, char *format,...)
{
	char String[21];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	LCD_DisplayStringLine(Line,(u8*)String);
	
}

uint8_t count =0;
void lcd_proc(void)
{
	lcdSprintf(Line0,"TEST UART APP");
	lcdSprintf(Line1,"UART DATA: %s",uart_rx_buffer);
	
}

