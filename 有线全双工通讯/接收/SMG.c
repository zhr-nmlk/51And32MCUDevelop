#include <REGX52.H>
#include "Delay1ms.h"

unsigned char SMG_Buf[12]={0,1,2,3,4,5,6,7,8,9,10,11};

unsigned char SMGTable[]={0x05,0xDD,0x46,0x54,0x9C,0x34,0x24,0x5D,0x04,0x14};

void SMG_SetBuf(unsigned char Location,Number)
{
	SMG_Buf[Location] = Number;
}

void SMG_Scan(unsigned char Location,Number)
{
	P2=0x00;
	
	switch(Location)
	{
		case 1: P0_3=1; P0_2=0;P3_6=0;P3_7 = 0;break;
		case 2: P0_3=0; P0_2=1;P3_6=0;P3_7 = 0;break;
		case 3: P3_4=0; P3_5=0;P3_6=1;P3_7 = 0;break;
		case 4: P3_4=0; P3_5=0;P3_6=0;P3_7 = 1;break;
	}
	P2=SMGTable[Number];
}
void SMG_Loop(void)
{
	static unsigned char i=1;
	
	SMG_Scan(i,SMG_Buf[i]);
	i++;
	if(i>=3)
	{
		i=1;
	}
}