#include <REGX52.H>
#include "key.h"
#include "Nixie.h"
#include "I2C.h"
#include "AT24C02.h"
#include "UART.h"

#define MODE1 1		//ģʽ1��2��3
#define MODE2 2
#define MODE3 3

#define S_key    1             //����
#define D_key    2             //˫��
#define L_key    3             //����

unsigned char UART_flag=0,R_I=0;
unsigned char State=1;
unsigned char Key,Key1,Key2,time_ok;
unsigned char Hz=10,dutyfactor=50;//��ʼ��
unsigned char Key_Up_Count,Key_Down_Count,Tcount,dutyfactor_up,dutyfactor_down,time_count_read;
unsigned int T;//��������
unsigned char * Hz_Duty;
char Hz_Duty_Record[]="f=--,d=---.";
void squarewave1()
{
	if(Tcount >= T) 
	{				//һ��PWM����
		Tcount = 0;					//��һ��PWM�����ڣ����¼���
		
	}  
	//ռ�ձȣ��ߵ�ƽʱ��ռ����ʱ��ı���

	//��һ��PWM�����ڣ��������ڼ����Ƿ�С��ռ�ձ�
	if(Tcount <= (dutyfactor*T*0.01) ) 
	{		//���ڼ���ֵ<ռ�ձ�
		P2_7 = 1;
	} else {
		P2_7 = 0;
	}
}

void squarewave2()
{
	if(Tcount >= T) 
	{				//һ��PWM����
		Tcount = 0;					//��һ��PWM�����ڣ����¼���
		
	}  
	//ռ�ձȣ��ߵ�ƽʱ��ռ����ʱ��ı���

	//��һ��PWM�����ڣ��������ڼ����Ƿ�С��ռ�ձ�
	if( (Tcount <= (dutyfactor*T*0.01) + (T/5*1.0) ) && ( Tcount >= (T/5*1.0) )  ) 
	{		//���ڼ���ֵ<ռ�ձ�
		P2_6 = 1;
	} else {
		P2_6 = 0;
	}
}
void modeshift()
{
	switch(State)
	{
		case MODE1:
				if(time_ok)		//ģʽ�л�
				{
					time_ok=0;
					Key=key_read();
					if(Key==S_key)
						State=1;
					if(Key==D_key)
						State=2;
					if(Key==L_key)
						State=3;
				}
				if(P3_3==0)
				{
					if(time_count_read<=250) time_count_read++;
				}else time_count_read=0;
				if(time_count_read==250)
				{
					time_count_read=0;
					Hz=AT24C02_ReadByte(0);
					dutyfactor=AT24C02_ReadByte(1);
				}
				T=1000/Hz;
				if(UART_flag==1)
				{
					Hz_Duty=UART_ReadData();
				}
				
				Nixie(3,Hz/10);Nixie(4,Hz%10);//��ʾƵ��
				Nixie(5,dutyfactor/100);Nixie(6,dutyfactor%100/10);Nixie(7,dutyfactor%10);//��ʾռ�ձ�
				break;
		case MODE2:
				Nixie(4,1);
				Nixie(5,Hz/10);
				Nixie(6,Hz%10);
				if(P3_1==0)
				{
					if(Key_Down_Count<=250)Key_Down_Count++;
				}else Key_Down_Count=0;
				if(Key_Down_Count>=250)
				{	
					Key_Down_Count=0;
					if(Hz>0)Hz--;
				}
				
				if(P3_2==0)
				{
					if(Key_Up_Count<=250)Key_Up_Count++;
				}else Key_Up_Count=0;
				if(Key_Up_Count>=250)
				{	
					Key_Up_Count=0;
					if(Hz<50)Hz++;
				}
				
				if(time_ok)
				{
					time_ok=0;
					Key=key_read();
					if(Key==S_key)
					{
						UART_SendString(Hz_Duty_Record);
						State=1;
					}
				}
				
				T=1000/Hz;
				Hz_Duty_Record[2]=Hz/10+48;
				Hz_Duty_Record[3]=Hz%10+48;
				Hz_Duty_Record[7]=dutyfactor/100+48;
				Hz_Duty_Record[8]=dutyfactor%100/10+48;
				Hz_Duty_Record[9]=dutyfactor%10+48;
				break;
		case MODE3:
				Nixie(4,2);
				Nixie(5,dutyfactor/100);
				Nixie(6,dutyfactor%100/10);
				Nixie(7,dutyfactor%10);
				if(P3_1==0)
				{
					if(dutyfactor_up<=250)dutyfactor_up++;
				}else dutyfactor_up=0;
				if(dutyfactor_up==250)
				{	
					dutyfactor_up=0;
					if(dutyfactor>0)dutyfactor-=10;
				}
				
				if(P3_2==0)
				{
					if(dutyfactor_down<=250)dutyfactor_down++;
				}else dutyfactor_down=0;
				if(dutyfactor_down==250)
				{	
					dutyfactor_down=0;
					if(dutyfactor<100)dutyfactor+=10;
				}
				
				if(time_ok)
				{
					time_ok=0;
					Key=key_read();
					if(Key==S_key)
					{
						UART_SendString(Hz_Duty_Record);
						State=1;
					}
				}
				Hz_Duty_Record[2]=Hz/10+48;
				Hz_Duty_Record[3]=Hz%10+48;
				Hz_Duty_Record[7]=dutyfactor/100+48;
				Hz_Duty_Record[8]=dutyfactor%100/10+48;
				Hz_Duty_Record[9]=dutyfactor%10+48;
				break;
	}
}
void Timer0Init(void)		//1����@11.0592MHz
{
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TMOD |= 0x01;		//���ö�ʱ��ģʽ
	TL0 = 0x66;		//���ö�ʱ��ֵ
	TH0 = 0xFC;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0 = 1;
	EA=1;
}

//void Timer1Init(void)		//10����@11.0592MHz
//{
//	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
//	TMOD |= 0x10;		//���ö�ʱ��ģʽ
//	TL1 = 0x00;		//���ö�ʱ��ֵ
//	TH1 = 0xDC;		//���ö�ʱ��ֵ
//	TF1 = 0;		//���TF1��־
//	TR1 = 1;		//��ʱ��1��ʼ��ʱ
//	ET1=1;
//}

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

void main()
{
	P2_5=0;
	Timer0Init();
//	Timer1Init();
	Timer2Init();
	UartInit();
	while(1)
	{
		squarewave1();
		squarewave2();
		modeshift();
	}
}

unsigned int T0Count,Tflag,T2Count;
void Timer0_Routine() interrupt 1		//��ʱ��1�ж� 1ms
{
	TL0 = 0x66;		//���ö�ʱ��ֵ
	TH0 = 0xFC;		//���ö�ʱ��ֵ
	Tcount++;
	T0Count++;
	if(T0Count>=10)
	{
		T0Count=0;
		time_ok=1;
	}
}

void Timer2_Rountine(void) interrupt 5//��ʱ��2�ж�	10ms
{
	TF2=0;
	Tflag++;
		switch(Tflag)
		{
			case 1:
				if(Hz!=10 || dutyfactor!=50)
					AT24C02_WriteByte(0,Hz);  //дhz��10ms��дdutyfactor
				break;
			case 2:if(Hz!=10 || dutyfactor!=50)
					AT24C02_WriteByte(1,dutyfactor);
				break;
		}
		if(Tflag>=2)
			Tflag=0;	
	
	if(UART_flag==1)
	{
		T2Count++;
		if(T2Count>=100)
		{
			T2Count=0;
			
			if(R_I>=6)
			{
				R_I=0;
				Hz=(Hz_Duty[2]-48)*10+(Hz_Duty[3]-48);
				dutyfactor=(Hz_Duty[7]-48)*100+(Hz_Duty[8]-48)*10+(Hz_Duty[9]-48);
			}else{
				R_I=0;
			}
			UART_flag=0;
		}
	}
}