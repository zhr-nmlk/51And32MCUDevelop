#include <REGX52.H>
#include "UART.h"

unsigned char StrBuf[20];
unsigned char R_I;
//unsigned char cnt;
void UartInit(void)		//4800bps@11.0592MHz
{
//	Timer0Init();
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xFA;		//设定定时初值
	TH1 = 0xFA;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	EA=1;
	ES=1;
}
//void Timer0Init(void)		//250微秒@11.0592MHz
//{
//	TMOD &= 0xF0;		//设置定时器模式
//	TMOD |= 0x02;		//设置定时器模式
//	TL0 = 0x1A;		//设置定时初值
//	TH0 = 0x1A;		//设置定时重载值
//	TF0 = 0;		//清除TF0标志
//	TR0 = 0;		//定时器0开始计时
//	ET0=1;
//}

//void Timer0_Routinr() interrupt 1
//{
//	cnt++;
//	if(cnt>100)
//	{
//		TR0=0;
//		cnt=0;
//		
////		StrBuf[R_I]='\0';
//		R_I=0;
//	}
//}
void UART_SendByte(unsigned char Byte)
{
	SBUF=Byte;
	while(TI==0);
	TI=0;
}

void UART_SendString(unsigned char * str)
{
	unsigned char i;
	while(str[i] != '\0')
	{
		UART_SendByte(str[i]);
		i++;
	}
	i=0;
}

unsigned char * UART_ReadData(void)
{
	return StrBuf;
}

void UART_Routine () interrupt 4
{
	if(RI==1)
	{
//		TR0=1;
//		cnt=0;
//		TL0 = 0x1A;		//设置定时初值
//		TH0 = 0x1A;		//设置定时重载值
		StrBuf[R_I++]=SBUF;
		RI=0;
	}
	
}