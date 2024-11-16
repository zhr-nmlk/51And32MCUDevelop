#include <REGX52.H>
#include "UART.h"
#include "Nixie.h"
#include "Delay1ms.h"

unsigned char UART_flag,R_I;
unsigned int T=50;//定义周期
unsigned char * Phase_Read;
unsigned int Phase=0;
char Phase_Record[]="Phase=---";
unsigned char dutyfactor=50;
unsigned int Tcount;

void Timer0Init(void)		//100微秒@11.0592MHz
{
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0xA4;		//设置定时初值
	TH0 = 0xFF;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0=1;
	EA=1;
}

void Timer2Init(void)		//10毫秒@11.0592MHz
{
	T2MOD = 0;		//初始化模式寄存器
	T2CON = 0;		//初始化控制寄存器
	TL2 = 0x00;		//设置定时初值
	TH2 = 0xDC;		//设置定时初值
	RCAP2L = 0x00;		//设置定时重载值
	RCAP2H = 0xDC;		//设置定时重载值
	TR2 = 1;		//定时器2开始计时
	ET2=1;
}

void squarewave1()
{
	if(Tcount >= T) 
	{				//一个PWM周期
		Tcount = 0;					//下一个PWM周期内，重新计数
		
	}  
	//占空比：高电平时间占周期时间的比例

	//在一个PWM周期内，根据周期计数是否小于占空比
	if( Tcount <= (dutyfactor*T*0.01) ) 
	{		//周期计数值<占空比
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
	{				//一个PWM周期
		Tcount = 0;					//下一个PWM周期内，重新计数
		
	}  
	//占空比：高电平时间占周期时间的比例

	//在一个PWM周期内，根据周期计数是否小于占空比
	if(   ( (T* (Phase/360*1.0) ) <= Tcount ) && (Tcount <= (dutyfactor*T*0.01) + (T* (Phase/360*1.0)) ) ) 
	{		//周期计数值<占空比
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

void Timer0_Routine() interrupt 1		//定时器1中断 100us
{
	TL0 = 0xA4;		//设置定时初值
	TH0 = 0xFF;		//设置定时初值
	Tcount++;
//	T0Count++;
//	if(T0Count>=10)
//	{
//		T0Count=0;
//		Nixie_Loop();
//	}
}
void Timer2_Rountine(void) interrupt 5//定时器2中断	10ms
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