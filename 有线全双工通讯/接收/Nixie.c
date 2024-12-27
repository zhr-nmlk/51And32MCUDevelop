#include <REGX52.H>
#include "Delay1ms.h"

unsigned char Nixie_Buf[12]={0,1,2,3,4,5,6,7,8,9,10,11};

unsigned char NixieTable[]={0x7E,0x48,0x3D,0x6D,0x4B,0x67,0x77,0x4C,0x7F,0x6F,0x80,0x00};

void Nixie_SetBuf(unsigned char Location,Number)
{
	Nixie_Buf[Location] = Number;
}

void Nixie_Scan(Number)
{
	P0=0x00;

	P0=NixieTable[Number];
}
void Nixie_Loop(void)
{
//	static unsigned char i=1;
	
	Nixie_Scan(Nixie_Buf[1]);
//	i++;
//	if(i>=12)
//	{
//		i=1;
//	}
}