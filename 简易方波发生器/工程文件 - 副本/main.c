#include <REGX52.H>
#include "key.h"

#define MODE1 1
#define MODE2 2
#define MODE3 3

unsigned char State=3;
unsigned char Key,time_ok;
void modeshift()
{
	switch(State)
	{
		case MODE1:P2_0=0;P2_1=1;P2_2=1;break;
		case MODE2:P2_0=1;P2_1=0;P2_2=1;break;
		case MODE3:P2_0=1;P2_1=1;P2_2=0;break;
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

void main()
{
	Timer0Init();
	while(1)
	{
		if(time_ok)
		{
			time_ok=0;
			Key=key_read();
			if(Key==1)
				State=1;
			if(Key==2)
				State=2;
			if(Key==3)
				State=3;
		}
		modeshift();
	}
}

void Timer0_Routine() interrupt 1
{
	TL0 = 0x00;		//设置定时初值
	TH0 = 0xDC;		//设置定时初值
	time_ok=1;
}