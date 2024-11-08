#include <REGX52.H>
#include "UART.h"

unsigned char StrBuf[20];
unsigned char R_I;
//unsigned char cnt;
void UartInit(void)		//4800bps@11.0592MHz
{
//	Timer0Init();
	PCON &= 0x7F;		//�����ʲ�����
	SCON = 0x50;		//8λ����,�ɱ䲨����
	TMOD &= 0x0F;		//�����ʱ��1ģʽλ
	TMOD |= 0x20;		//�趨��ʱ��1Ϊ8λ�Զ���װ��ʽ
	TL1 = 0xFA;		//�趨��ʱ��ֵ
	TH1 = 0xFA;		//�趨��ʱ����װֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1
	EA=1;
	ES=1;
}
//void Timer0Init(void)		//250΢��@11.0592MHz
//{
//	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
//	TMOD |= 0x02;		//���ö�ʱ��ģʽ
//	TL0 = 0x1A;		//���ö�ʱ��ֵ
//	TH0 = 0x1A;		//���ö�ʱ����ֵ
//	TF0 = 0;		//���TF0��־
//	TR0 = 0;		//��ʱ��0��ʼ��ʱ
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
//		TL0 = 0x1A;		//���ö�ʱ��ֵ
//		TH0 = 0x1A;		//���ö�ʱ����ֵ
		StrBuf[R_I++]=SBUF;
		RI=0;
	}
	
}