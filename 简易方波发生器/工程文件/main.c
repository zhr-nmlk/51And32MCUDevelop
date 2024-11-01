#include <REGX52.H>
#include "key.h"

#define MODE1 1
#define MODE2 2
#define MODE3 3

unsigned char State=3;


void modeshift()
{
	switch(State)
	{
		case MODE1:P2_1=0;P2_0=1;P2_2=1;break;
		case MODE2:P2_0=0;P2_1=1;P2_2=1;break;
		case MODE3:P2_0=1;P2_1=1;P2_2=0;break;
	}
}

void main()
{
	while(1)
	{
		KeyNumber();
		modeshift();
	}
}