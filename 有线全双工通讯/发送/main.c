#include <REGX52.H>
#include "key.h"
#include "Nixie.h"
#include "I2C.h"
#include "AT24C02.h"
#include "key-hz.h"
#include "key-duty.h"
#include "UART.h"

//#include "UART.h"
//f频率在1~9，闭区间，占空比50%（暂定），相位差36度的1到9倍。
#define MODE1 1		//模式1，2，3
//#define MODE2 2
//#define MODE3 3	//模式暂不改变

#define S_key    1             //单键
#define D_key    2             //双键
#define L_key    3             //长键

unsigned char X_Num=11;		//X_Num的值为[1,99]
unsigned char eeprom_flag=0;
unsigned char UART_flag=0,R_I=0;
unsigned char Key,Key1,Key2,time_ok;
unsigned char Hz=13,dutyfactor1=50;//初始化//两路方波频率保持同步，只改变相位差,频率范围暂定11到19赫兹
unsigned int Tcount;
unsigned int T;//定义周期
float phase=324;
unsigned char * X_read;
char Hz_Duty_Record[]="set x=--";
unsigned char UART_flag,R_I;

void squarewave1()
{
	if(Tcount<=T)
	{

		if(Tcount <= (T/2) ) 
		{		//周期计数值<占空比
			P2_7 = 1;
		} else {
			P2_7 = 0;
		}
		if(phase<=180)
		{
			if( (Tcount <= (T/2) + (T*(phase/360)) ) && ( Tcount >= (T*(phase/360)*1.0) )  ) 
			{		//周期计数值<占空比
				P2_6 = 1;
			} else {
				P2_6 = 0;
			}
		}
		else
		{
			if( (Tcount <= (T/2) + (T*((phase-180)/360)) ) && ( Tcount >= (T*((phase-180)/360)*1.0) )  ) 
			{		//周期计数值<占空比
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

void Timer0Init(void)		//1毫秒@11.0592MHz
{
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x33;		//设置定时初值
	TH0 = 0xFE;		//设置定时初值
	TF0 = 0;		//清除TF0标志 
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;
	EA=1;
}

//void Timer1Init(void)		//10毫秒@11.0592MHz
//{
//	TMOD &= 0x0F;		//设置定时器模式
//	TMOD |= 0x10;		//设置定时器模式
//	TL1 = 0x00;		//设置定时初值
//	TH1 = 0xDC;		//设置定时初值
//	TF1 = 0;		//清除TF1标志
//	TR1 = 1;		//定时器1开始计时
//	ET1=1;
//}

void Timer2Init(void)		//1毫秒@11.0592MHz
{
	T2MOD = 0;		//初始化模式寄存器
	T2CON = 0;		//初始化控制寄存器
	TL2 = 0x66;		//设置定时初值
	TH2 = 0xFC;		//设置定时初值
	RCAP2L = 0x66;		//设置定时重载值
	RCAP2H = 0xFC;		//设置定时重载值
	TR2 = 1;		//定时器2开始计时
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
			if(Key==S_key)	//改变十位数字
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
void Timer0_Routine() interrupt 1		//定时器0中断 
{
	TL0 = 0x33;		//设置定时初值
	TH0 = 0xFE;		//设置定时初值
	
	Tcount++;
}

void Timer2_Rountine(void) interrupt 5//定时器2中断	1ms
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