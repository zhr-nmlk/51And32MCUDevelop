#include <REGX52.H>

//void UART_Init()		//4800bps@11.0592MHz，初始化
//{
//	PCON &= 0x7F;		//波特率不倍速
//	SCON = 0x50;		//8位数据,可变波特率
//	TMOD &= 0x0F;		//清除定时器1模式位
//	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
//	TL1 = 0xFA;		//设定定时初值
//	TH1 = 0xFA;		//设定定时器重装值
//	ET1 = 1;		//禁止定时器1中断
//	TR1 = 0;		//启动定时器1
//	
//	EA=1;			//总中断
//	ES=0;			//启动串口中断

//}

void UartInit(void)		//9600bps@11.0592MHz
{
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xFD;		//设定定时初值
	TH1 = 0xFD;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	
	EA=1;
	ES=1;
}

void UART_SendByte(unsigned char Byte)//发送数据
{
	SBUF = Byte;
	while(TI==0);
	TI=0;
}


//模板
//void UART_Routine(void) interrupt 4
//{
//	if(RI==1)
//	{

//		RI=0; //清零
//	}
//}