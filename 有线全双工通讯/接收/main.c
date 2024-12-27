#include <REGX52.H>
#include "Key.h"
#include "Nixie.h"
#include "UART.h"
#include "SMG.h"

#define mode0 0
#define mode1 1
void Timer0Init(void)		//1毫秒@11.0592MHz
{
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x33;		//设置定时初值
	TH0 = 0xFE;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0=1;
	EA=1;
}

void Timer2Init(void)		//10毫秒@11.0592MHz
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

void init(void)
{
	EX0=1;
	IT0=1;   //
}

void init1(void)
{
	EX1=1;
	IT1=1;   
}
unsigned int periodCount = 0,periodCount2=0;
unsigned int highTimeCount =0,highTimeCount2=0;
bit edgeDetected = 0,edge2=0;

//unsigned char UART_flag,R_I;

//unsigned char keynum;
unsigned char freq,duty,freq2,duty2;
bit T0flag,T0flag2=0;

unsigned int phase;
unsigned int p_count;
unsigned int phase_re;

bit send_flag=0;
//char hz_duty_record[]="ch1:f=**,p=***";
char hz_duty_record[]="get x=**";
unsigned char state=0;

void modechange(unsigned char state)
{
	switch (state)
	{
		case mode0:
			
			if(freq>=45)
			{
				P2=0xE0;
			}else if(freq>=35)
			{
				P2=0xF0;
			}else if(freq>=25)
			{
				P2=0xF8;
			}else if(freq>=15)
			{
				P2=0xFC;
			}else if(freq>=5)
			{
				P2=0xFE;
			}
			Nixie_SetBuf(1,(duty+5)/10);
			SMG_SetBuf(2,(duty+3)/10);
			SMG_SetBuf(4,(freq+5)/10);
			SMG_SetBuf(3,0);
			SMG_SetBuf(1,0);
			break;
		case mode1:
			if(freq2>=45)
			{
				P2=0xE0;
			}else if(freq2>=35)
			{
				P2=0xF0;
			}else if(freq2>=25)
			{
				P2=0xF8;
			}else if(freq2>=15)
			{
				P2=0xFC;
			}else if(freq2>=5)
			{
				P2=0xFE;
			}
//			Nixie_SetBuf(1,(duty2+5)/10);
			SMG_SetBuf(2,(duty2+5)/10);
			SMG_SetBuf(4,(freq2+5)/10);
			SMG_SetBuf(3,0);
			SMG_SetBuf(1,0);
			break;
	}
}
void main()
{
	Timer0Init();
	P2=0xFE;
	init();
//	init1();
	Timer2Init();
	UartInit();
	while(1)
	{
//			modechange(state);
			if(send_flag==1)
			{
				
				send_flag=0;
//				hz_duty_record[11]=(duty+3)/10+48;
//				hz_duty_record[6]=(freq)/10+48;
//				hz_duty_record[7]=(freq)%10+48;
//				hz_duty_record[11]=(phase)/100+48;
//				hz_duty_record[12]=(phase)%100/10+48;
//				hz_duty_record[13]=(phase)%10+48;
				hz_duty_record[6]=freq%10+48;
				hz_duty_record[7]=phase_re+48;
//				hz_duty_record[20]=(freq2+5)/10+48;
//				hz_duty_record[25]=(duty2+3)/10+48;
//				SMG_SetBuf(1,freq/10);
//				SMG_SetBuf(2,freq%10);
				UART_SendString(hz_duty_record);
			}
			SMG_SetBuf(1,freq%10);
			SMG_SetBuf(2,phase_re);
			if(phase>180)
			{
				phase_re=(phase-175)/36;
			}
			else
			{
				phase_re=(phase+180)/36;
			}
//			KEY_Scan_Deive();
//			if(Key_Read_Data==1)
//			{
//				state=1;
//			}
//			if(Key_Read_Data==3)
//			{
//				state=0;
//			}
	}
}

unsigned int T0Count,T0Count1,T0Count2,T2count;
void Init0_Rountine() interrupt 0
{
	if(edgeDetected == 0)
	{
		edgeDetected =1;
		periodCount =0;
		highTimeCount = 0;
		T0flag=1;
	}
	else
	{
		edgeDetected = 0;
		T0flag=0;
		freq = 2000/periodCount;
		phase = p_count*360/periodCount;
		duty = highTimeCount*100/periodCount;
	}
}

//void Init1_Rountine() interrupt 2
//{
//	if(edge2==0)
//	{
//		edge2=1;
//		periodCount2 = 0;
//		highTimeCount2=0;
//		T0flag2=1;
//	}
//	else
//	{
//		edge2=0;
//		T0flag2=0;
//		freq2 = 1000/periodCount2;
//		duty2 = highTimeCount2*100/periodCount2;
//	}
//}
unsigned char nowstate2,laststate2;
void Timer0_Routine() interrupt 1
{
	TL0 = 0x33;		//设置定时初值
	TH0 = 0xFE;		//设置定时初值
	T0Count++;
	T0Count1++;
	T0Count2++;
	
	if(T0flag==1)
	{
		if(edgeDetected ==1)
		{
			periodCount++;
		}
		if(P3_3==1 &&  laststate2==0 )
		{
			p_count=periodCount;
		}
//		if(P3_2==1 && edgeDetected ==1)
//		{
//			highTimeCount++;
//		}
	}
//	if(T0flag2==1)
//	{
//		if(edge2 ==1)
//		{
//			periodCount2++;
//		}
//		if(P3_3==1 && edge2 == 1)
//		{
//			highTimeCount2++;
//		}
//	}	
	laststate2=P3_3;
}

void Timer2_Rountine(void) interrupt 5//定时器2中断	1ms
{
	TF2=0;
	T2count++;
	SMG_Loop();
	if(T2count>=1000)
	{
		T2count=0;
		send_flag=1;
	}
}
