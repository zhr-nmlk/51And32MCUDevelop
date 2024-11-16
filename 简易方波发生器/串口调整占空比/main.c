#include <REGX52.H>
#include "UART.h"
#include "Nixie.h"
#include "Delay1ms.h"

unsigned char UART_flag,R_I;
unsigned int T=50;//��������
unsigned char * Phase_Read;
unsigned int Phase=0;
char Phase_Record[]="Phase=---";
unsigned char dutyfactor=50;
unsigned int Tcount;

void Timer0Init(void)		//100΢��@11.0592MHz
{
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TMOD |= 0x01;		//���ö�ʱ��ģʽ
	TL0 = 0xA4;		//���ö�ʱ��ֵ
	TH0 = 0xFF;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0=1;
	EA=1;
}

void Timer2Init(void)		//10����@11.0592MHz
{
	T2MOD = 0;		//��ʼ��ģʽ�Ĵ���
	T2CON = 0;		//��ʼ�����ƼĴ���
	TL2 = 0x00;		//���ö�ʱ��ֵ
	TH2 = 0xDC;		//���ö�ʱ��ֵ
	RCAP2L = 0x00;		//���ö�ʱ����ֵ
	RCAP2H = 0xDC;		//���ö�ʱ����ֵ
	TR2 = 1;		//��ʱ��2��ʼ��ʱ
	ET2=1;
}

void squarewave1()
{
	if(Tcount >= T) 
	{				//һ��PWM����
		Tcount = 0;					//��һ��PWM�����ڣ����¼���
		
	}  
	//ռ�ձȣ��ߵ�ƽʱ��ռ����ʱ��ı���

	//��һ��PWM�����ڣ��������ڼ����Ƿ�С��ռ�ձ�
	if( Tcount <= (dutyfactor*T*0.01) ) 
	{		//���ڼ���ֵ<ռ�ձ�
		P2_7 = 1;
	} 
	else 
	{
		P2_7 = 0;
	}
}

void squarewave2()
{
	if( Tcount >= T ) 
	{				//һ��PWM����
		Tcount = 0;					//��һ��PWM�����ڣ����¼���
		
	}  
	//ռ�ձȣ��ߵ�ƽʱ��ռ����ʱ��ı���

	//��һ��PWM�����ڣ��������ڼ����Ƿ�С��ռ�ձ�
	if(   ( (T* (Phase/360*1.0) ) <= Tcount ) && (Tcount <= (dutyfactor*T*0.01) + (T* (Phase/360*1.0)) ) ) 
	{		//���ڼ���ֵ<ռ�ձ�
		P2_6 = 1;
	}
	else 
	{
		P2_6 = 0;
	}
	
}

void main()
{
	Timer0Init();
	Timer2Init();
	UartInit();
	while(1)
	{
		squarewave1();
		squarewave2();
		if(UART_flag==1)
		{
			Phase_Read=UART_ReadData();
		}
//		Nixie_SetBuf(5,Phase/100);
//		Nixie_SetBuf(6,Phase%100/10);
//		Nixie_SetBuf(7,Phase%10);
	}
}

unsigned char T2Count,T0Count;

void Timer0_Routine() interrupt 1		//��ʱ��1�ж� 100us
{
	TL0 = 0xA4;		//���ö�ʱ��ֵ
	TH0 = 0xFF;		//���ö�ʱ��ֵ
	Tcount++;
//	T0Count++;
//	if(T0Count>=10)
//	{
//		T0Count=0;
//		Nixie_Loop();
//	}
}
void Timer2_Rountine(void) interrupt 5//��ʱ��2�ж�	10ms
{
	TF2=0;
	if(UART_flag==1)
	{
		T2Count++;
		if(T2Count>=100)
		{
			T2Count=0;
			
			if(R_I>=6)
			{
				R_I=0;

				Phase=(Phase_Read[6]-48)*100+(Phase_Read[7]-48)*10+(Phase_Read[8]-48);
//				Nixie_SetBuf(5,Phase/100);
//				Nixie_SetBuf(6,Phase%100/10);
//				Nixie_SetBuf(7,Phase%10);
			}else{
				R_I=0;
			}
			UART_flag=0;
		}
	}
}