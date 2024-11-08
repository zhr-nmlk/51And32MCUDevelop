#ifndef __UART_H__
#define __UART_H__

void UartInit(void);		//4800bps@11.0592MHz
void Timer0Init(void);		//250Œ¢√Î@11.0592MHz
void UART_SendByte(unsigned char Byte);
void UART_SendString(unsigned char * str);
unsigned char * UART_ReadData(void);

#endif
