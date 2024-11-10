#include <REGX52.H>
#include "Nixie.h"
#include "Delay1ms.h"
#include "UART.h"
#include "led.h"

sbit BEEP=P2^5;	//��P2.5�ܽŶ���ΪBEEP
int Tcounter,direct,dutyfactor,Tcount;
unsigned int num=15,num1,num2;
unsigned int password=5,usernum=0,password1,password2,password3;
int count;
int x,y,z;
int beepflat=0;
void Timer0_Init(void)		//1����@11.0592MHz
{
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TMOD |= 0x01;		//���ö�ʱ��ģʽ
	TL0 = 0x66;		//���ö�ʱ��ֵ
	TH0 = 0xFC;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	
	ET0=1;	//�����ж�
	EA=1;
}

//void Timer1_Init(void)		//1����@11.0592MHz
//{
//	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
//	TMOD |= 0x10;		//���ö�ʱ��ģʽ
//	TL1 = 0x48;		//���ö�ʱ��ֵ
//	TH1 = 0xFF;		//���ö�ʱ��ֵ
//	TF1 = 0;		//���TF1��־
//	TR1 = 0;		//��ʱ��1��ʼ��ʱ
//	
//	ET1=1;
//	EA=1;
//}

void Timer2Init(void)		//50����@11.0592MHz
{
	T2MOD = 0;		//��ʼ��ģʽ�Ĵ���
	T2CON = 0;		//��ʼ�����ƼĴ���
	TL2 = 0x48;		//���ö�ʱ��ֵ
	TH2 = 0xFF;		//���ö�ʱ��ֵ
	RCAP2L = 0x48;		//���ö�ʱ����ֵ
	RCAP2H = 0xFF;		//���ö�ʱ����ֵ
	TR2 = 0;		//��ʱ��2��ʼ��ʱ

	ET2=1;
	EA=1;
}



void breatheLED() 
{
	if(Tcount >= 100) 
	{				//һ��PWM����(100��*200us)
		Tcount = 0;					//��һ��PWM�����ڣ����¼���
		
		//���ݷ����ж�ռ�ձ����ӻ����
		if(direct == 0)
		{			//������ռ�ձ�����
			dutyfactor++;			//��һ��PWM���ڣ�ռ�ձ�����1
		} else if(direct == 1){		//������ռ�ձȼ���
			dutyfactor--;			//��һ��PWM���ڣ�ռ�ձȼ���1
		}
	}
	
	//����ռ�ձ�ֵ�жϷ����Ƿ���Ҫ�ı�
	if(dutyfactor <= 0)
	{			//ռ�ձȼ��ٵ�0
		direct = 0;					//������ռ�ձȿ�ʼ����
	} else if(dutyfactor >= 100) {	//ռ�ձ����ӵ�100
		direct = 1;					//������ռ�ձȿ�ʼ����
	}
	
	//ռ�ձȣ�ͨ��ʱ���������ʱ����ռ�ı������˳����У��͵�ƽʱ�������PWM������ռ�ı����� 
	//һ��PWM���ڼ���100�Σ�ռ�ձ��൱�ڵ͵�ƽ�ļ������ڴ���������0~ռ�ձȣ��͵�ƽ��������ռ�ձ�~100���ߵ�ƽ������ 
	//��һ��PWM�����ڣ��������ڼ����Ƿ�С��ռ�ձȣ��ж�LED�Ƶ�����
	if(Tcount < dutyfactor) 
	{		//���ڼ���ֵ<ռ�ձ�
		P2_0 = 0;
		P2_6 = 0;
		P2_7 = 0;//��ǰPWM���ڵĵ�ǰ���������ڵ���
	} else {
		P2_0 = 1;
		P2_6 = 1;
		P2_7 = 1;//��ǰPWM���ڵĵ�ǰ���������ڵ���
	}
}

//unsigned int num=15,num1,num2;
//unsigned int password=5,usernum=5,password1,password2,password3;
//int count;
//int x,y,z;
//int Tcounter,direct,dutyfactor;
void main()
{	
	UartInit();
	Timer0_Init();
//	Timer1_Init();
	Timer2Init();

	BEEP=0;
	while(1)
	{			
		Nixie(1,num1);
		Nixie(2,num2);
		Nixie(4,usernum);

		if(P3_1==0)
		{
			if(x<=10)x++;
		}else x=0;
		if(x==10)P2_0=~P2_0;
			if(P2_0==0)
				password1=1;
			else
				password1=0;
		
		if(P3_0==0)
		{
			if(y<=10)y++;
		}else y=0;
		if(y==10)P2_6=~P2_6;
			if(P2_6==0)
				password2=1;
			else
				password2=0;
		
		if(P3_2==0)
		{
			if(z<=10)z++;
		}else z=0;
		if(z==10)P2_7=~P2_7;
			if(P2_7==0)
				password3=1;
			else
				password3=0;
				
		if(num==0 && usernum!=password)
		{
			TR2=1;
//			TR1=1;
			beepflat=1;
		}
		if(usernum==password)
		{
			
			TR0=0;
			TR2=1;
			breatheLED();
			Nixie(1,0);
			Nixie(2,0);

		}
			
	}
}

unsigned int T0Counter,T1Counter,T2counter;
void Timer0_Routine() interrupt 1
{
	TL0 = 0x66;		//���ö�ʱ��ֵ
	TH0 = 0xFC;		//���ö�ʱ��ֵ
//	Nixie(1,num1);
//	Nixie(2,num2);
//	Nixie(4,usernum);
	if(beepflat==1)
	{
		BEEP=~BEEP;
	}
	T0Counter++;
	if(T0Counter>=1000)
	{
		usernum=(password1*4)+(password2*2)+(password3*1);
		UART_SendByte(usernum);
		T0Counter=0;
		if(num!=0)
		{
			num--;
			num1=num/10;
			num2=num%10;	
			
		}
//		if(num==0)
//		{
//			TR2=1;
//			TR1=1;
//			
//		}		
	}
}

//void Timer1_Rountine(void) interrupt 3
//{
//	TL1 = 0x48;		//���ö�ʱ��ֵ
//	TH1 = 0xFF;		//���ö�ʱ��ֵ

//}	

void UART_Routine(void) interrupt 4
{
	if(RI==1)
	{
		usernum = SBUF;
		RI=0; //����
	}
}

void Timer2_Rountine(void) interrupt 5
{
	TF2=0;
	EXF2=0;
	
	if(usernum != password)
	{
		T2counter++;
		if(T2counter>=500)
		{
			T2counter=0;
			count++;
			switch(count)
			{
				case 1:P2_0=1;P2_6=1;P2_7=0;break;
				case 2:P2_0=1;P2_6=0;P2_7=1;break;
				case 3:P2_0=0;P2_6=1;P2_7=1;break;
			}
			if(count>=3)
			{
				count=0;
			}
		}
	}
	if(usernum == password)
	{
//		Tcounter++;
		Tcount++;

	}
	
}