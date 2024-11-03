#include <REGX52.H>
#include "key.h"
#include "Nixie.h"

#define MODE1 1		//模式1，2，3
#define MODE2 2
#define MODE3 3

#define S_key    1             //单键
#define D_key    2             //双键
#define L_key    3             //长键

unsigned char State=1;
unsigned char Key,Key1,Key2,time_ok;
unsigned char Hz=10,dutyfactor=50;//初始化
unsigned char Key_Up_Count,Key_Down_Count,Tcount,dutyfactor_up,dutyfactor_down; 
unsigned int T;//定义周期
void bo() 
{
	if(Tcount >= T) 
	{				//一个PWM周期
		Tcount = 0;					//下一个PWM周期内，重新计数
		
	}  
	//占空比：高电平时间占周期时间的比例

	//在一个PWM周期内，根据周期计数是否小于占空比
	if(Tcount < (dutyfactor*T*0.01)) 
	{		//周期计数值<占空比
		P2_7 = 1;
	} else {
		P2_7 = 0;
	}
}

void modeshift()
{
	switch(State)
	{
		case MODE1:
				bo();	//pwm产生方波
				if(time_ok)		//模式切换
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
				Nixie(3,Hz/10);Nixie(4,Hz%10);//显示频率
				Nixie(5,dutyfactor/100);Nixie(6,dutyfactor%100/10);Nixie(7,dutyfactor%10);//显示占空比
				break;
		case MODE2:
				Nixie(4,1);
				Nixie(5,Hz/10);
				Nixie(6,Hz%10);
				if(P3_1==0)
				{
					if(Key_Down_Count<=250)Key_Down_Count++;
				}else Key_Down_Count=0;
				if(Key_Down_Count==250)
				{	
					Key_Down_Count=0;
					if(Hz>0)Hz--;
				}
				
				if(P3_3==0)
				{
					if(Key_Up_Count<=250)Key_Up_Count++;
				}else Key_Up_Count=0;
				if(Key_Up_Count==250)
				{	
					Key_Up_Count=0;
					if(Hz<50)Hz++;
				}
					if(time_ok)
					{
						time_ok=0;
						Key=key_read();
						if(Key==S_key)
							State=1;
					}

				T=10000/Hz;
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
				
				if(P3_3==0)
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
						State=1;
				}
				break;
	}
}
void Timer0Init(void)		//10毫秒@11.0592MHz
{
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x00;		//设置定时初值
	TH0 = 0xDC;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;
	EA=1;
}

void Timer1Init(void)		//100微秒@11.0592MHz
{
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0xA4;		//设置定时初值
	TH1 = 0xFF;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	ET1=1;
}

void main()
{
	Timer0Init();
	Timer1Init();
	P2_5=0;
	while(1)
	{
		modeshift();
	}
}

unsigned int T0Count,T1Count;
void Timer0_Routine() interrupt 1
{
	TL0 = 0x00;		//设置定时初值
	TH0 = 0xDC;		//设置定时初值
	time_ok=1;
	T0Count++;
	if(T0Count>=1000)
	{
		T0Count=0;	
	}
}

void Timer1_Routine() interrupt 3
{
	TL1 = 0xA4;		//设置定时初值
	TH1 = 0xFF;		//设置定时初值

	Tcount++;

}
