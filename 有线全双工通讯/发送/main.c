#include <REGX52.H>
#include "key.h"
#include "Nixie.h"
#include "I2C.h"
#include "AT24C02.h"
#include "key-hz.h"
#include "key-duty.h"
#include "UART.h"

//#include "UART.h"
//fƵ����1~9�������䣬ռ�ձ�50%���ݶ�������λ��36�ȵ�1��9����
#define MODE1 1		//ģʽ1��2��3
//#define MODE2 2
//#define MODE3 3	//ģʽ�ݲ��ı�

#define S_key    1             //����
#define D_key    2             //˫��
#define L_key    3             //����

unsigned char X_Num=11;		//X_Num��ֵΪ[1,99]
unsigned char eeprom_flag=0;
unsigned char UART_flag=0,R_I=0;
unsigned char Key,Key1,Key2,time_ok;
unsigned char Hz=13,dutyfactor1=50;//��ʼ��//��·����Ƶ�ʱ���ͬ����ֻ�ı���λ��,Ƶ�ʷ�Χ�ݶ�11��19����
unsigned int Tcount;
unsigned int T;//��������
float phase=324;
unsigned char * X_read;
char Hz_Duty_Record[]="set x=--";
unsigned char UART_flag,R_I;

void squarewave1()
{
	if(Tcount<=T)
	{

		if(Tcount <= (T/2) ) 
		{		//���ڼ���ֵ<ռ�ձ�
			P2_7 = 1;
		} else {
			P2_7 = 0;
		}
		if(phase<=180)
		{
			if( (Tcount <= (T/2) + (T*(phase/360)) ) && ( Tcount >= (T*(phase/360)*1.0) )  ) 
			{		//���ڼ���ֵ<ռ�ձ�
				P2_6 = 1;
			} else {
				P2_6 = 0;
			}
		}
		else
		{
			if( (Tcount <= (T/2) + (T*((phase-180)/360)) ) && ( Tcount >= (T*((phase-180)/360)*1.0) )  ) 
			{		//���ڼ���ֵ<ռ�ձ�
				P2_6 = 0;
			} else {
				P2_6 = 1;
			}
		}
	}else
	{
		Tcount=0;
	}
}

void Timer0Init(void)		//1����@11.0592MHz
{
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TMOD |= 0x01;		//���ö�ʱ��ģʽ
	TL0 = 0x33;		//���ö�ʱ��ֵ
	TH0 = 0xFE;		//���ö�ʱ��ֵ
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

void Timer2Init(void)		//1����@11.0592MHz
{
	T2MOD = 0;		//��ʼ��ģʽ�Ĵ���
	T2CON = 0;		//��ʼ�����ƼĴ���
	TL2 = 0x66;		//���ö�ʱ��ֵ
	TH2 = 0xFC;		//���ö�ʱ��ֵ
	RCAP2L = 0x66;		//���ö�ʱ����ֵ
	RCAP2H = 0xFC;		//���ö�ʱ����ֵ
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
	X_Num=AT24C02_ReadByte(0);
	Hz=X_Num/10+11;
	T=2000/Hz;
	phase=(X_Num%10)*36;
//	phase=(X_Num%10)*36;
//	left_side=Tt1*(phase/360);
//	righ_side=(Tt1/2) + Tt1*(phase/360);
	Nixie_SetBuf(1,0);Nixie_SetBuf(2,0);Nixie_SetBuf(3,X_Num/10);Nixie_SetBuf(4,X_Num%10);
	Nixie_SetBuf(5,0);Nixie_SetBuf(6,0);Nixie_SetBuf(7,0);Nixie_SetBuf(8,0);
	while(1)
	{
		squarewave1();
		
		if(UART_flag==1)
		{
			X_read=UART_ReadData();
		}
		if(time_ok)		
		{
			time_ok=0;
			Key=key_read();
			Key2=key_read_hz();
			if(Key==S_key)	//�ı�ʮλ����
			{
				if(X_Num<=90)
				{
					X_Num=X_Num+10;
					Nixie_SetBuf(3,X_Num/10);Nixie_SetBuf(4,X_Num%10);
					AT24C02_WriteByte(0,X_Num);
					Hz=X_Num/10+11;
					T=2000/Hz;
				
				}
				
			}
			if(Key==D_key)
			{	
				if(X_Num>=10)
				{
					X_Num=X_Num-10;
					Nixie_SetBuf(3,X_Num/10);Nixie_SetBuf(4,X_Num%10);
					AT24C02_WriteByte(0,X_Num);
					
					Hz=X_Num/10+11;
					T=2000/Hz;
					
					
				}
			}
			
			if(Key2==S_key)
			{
				if(X_Num<99)
				{
					X_Num=X_Num+1;
					Nixie_SetBuf(3,X_Num/10);Nixie_SetBuf(4,X_Num%10);
					AT24C02_WriteByte(0,X_Num);
					Hz=X_Num/10+11;
					T=2000/Hz;
					phase=(X_Num%10)*36;
				}
			}
			if(Key2==D_key)
			{	
				if(X_Num>0)
				{
					X_Num=X_Num-1;
					Nixie_SetBuf(3,X_Num/10);Nixie_SetBuf(4,X_Num%10);
					AT24C02_WriteByte(0,X_Num);
					Hz=X_Num/10+11;
					T=2000/Hz;
					phase=(X_Num%10)*36;
				}
			}
		}
	}
}

unsigned int T0Count,Tflag,T2Count;
void Timer0_Routine() interrupt 1		//��ʱ��0�ж� 
{
	TL0 = 0x33;		//���ö�ʱ��ֵ
	TH0 = 0xFE;		//���ö�ʱ��ֵ
	
	Tcount++;
}

void Timer2_Rountine(void) interrupt 5//��ʱ��2�ж�	1ms
{
	TF2=0;
	T0Count++;
	if(T0Count>=10)
	{
		T0Count=0;
		time_ok=1;
	}
	Nixie_Loop();
	if(UART_flag==1)
	{
		T2Count++;
		if(T2Count>=1000)
		{
			T2Count=0;
			
			if(R_I>=4)
			{
				R_I=0;

				X_Num=(X_read[6]-48)*10+(X_read[7]-48);
				Nixie_SetBuf(3,X_Num/10);Nixie_SetBuf(4,X_Num%10);
				Hz=X_Num/10+11;
				T=2000/Hz;
				phase=(X_Num%10)*36;
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