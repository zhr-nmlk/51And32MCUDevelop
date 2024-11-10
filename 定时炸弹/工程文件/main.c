#include <REGX52.H>
#include "Nixie.h"
#include "Delay1ms.h"
#include "UART.h"
#include "led.h"

sbit BEEP=P2^5;	//将P2.5管脚定义为BEEP
int Tcounter,direct,dutyfactor,Tcount;
unsigned int num=15,num1,num2;
unsigned int password=5,usernum=0,password1,password2,password3;
int count;
int x,y,z;
int beepflat=0;
void Timer0_Init(void)		//1毫秒@11.0592MHz
{
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x66;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	
	ET0=1;	//设置中断
	EA=1;
}

//void Timer1_Init(void)		//1毫秒@11.0592MHz
//{
//	TMOD &= 0x0F;		//设置定时器模式
//	TMOD |= 0x10;		//设置定时器模式
//	TL1 = 0x48;		//设置定时初值
//	TH1 = 0xFF;		//设置定时初值
//	TF1 = 0;		//清除TF1标志
//	TR1 = 0;		//定时器1开始计时
//	
//	ET1=1;
//	EA=1;
//}

void Timer2Init(void)		//50毫秒@11.0592MHz
{
	T2MOD = 0;		//初始化模式寄存器
	T2CON = 0;		//初始化控制寄存器
	TL2 = 0x48;		//设置定时初值
	TH2 = 0xFF;		//设置定时初值
	RCAP2L = 0x48;		//设置定时重载值
	RCAP2H = 0xFF;		//设置定时重载值
	TR2 = 0;		//定时器2开始计时

	ET2=1;
	EA=1;
}



void breatheLED() 
{
	if(Tcount >= 100) 
	{				//一个PWM周期(100次*200us)
		Tcount = 0;					//下一个PWM周期内，重新计数
		
		//根据方向判断占空比增加或减少
		if(direct == 0)
		{			//正方向，占空比增加
			dutyfactor++;			//下一个PWM周期，占空比增加1
		} else if(direct == 1){		//负方向，占空比减少
			dutyfactor--;			//下一个PWM周期，占空比减少1
		}
	}
	
	//根据占空比值判断方向是否需要改变
	if(dutyfactor <= 0)
	{			//占空比减少到0
		direct = 0;					//正方向，占空比开始增加
	} else if(dutyfactor >= 100) {	//占空比增加到100
		direct = 1;					//负方向，占空比开始减少
	}
	
	//占空比：通电时间相对于总时间所占的比例（此程序中：低电平时间相对于PWM周期所占的比例） 
	//一个PWM周期计数100次，占空比相当于低电平的计数周期次数。即：0~占空比：低电平，灯亮；占空比~100：高电平，灯灭 
	//在一个PWM周期内，根据周期计数是否小于占空比，判断LED灯的亮灭
	if(Tcount < dutyfactor) 
	{		//周期计数值<占空比
		P2_0 = 0;
		P2_6 = 0;
		P2_7 = 0;//当前PWM周期的当前计数周期内灯亮
	} else {
		P2_0 = 1;
		P2_6 = 1;
		P2_7 = 1;//当前PWM周期的当前计数周期内灯灭
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
	TL0 = 0x66;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
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
//	TL1 = 0x48;		//设置定时初值
//	TH1 = 0xFF;		//设置定时初值

//}	

void UART_Routine(void) interrupt 4
{
	if(RI==1)
	{
		usernum = SBUF;
		RI=0; //清零
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