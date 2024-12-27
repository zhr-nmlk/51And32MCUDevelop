#include <REGX52.H>

#define KEY_NO 0
#define KEY_S 1
#define KEY_L 3

unsigned char Key_Read_Data=0;

void KEY_Scan_Deive()
{
	static bit flag1=0;
	static bit flag2=1;
	static bit key_down=0;
	static unsigned char count1=0;
	static unsigned int count2=0;
	if(key_down==1)
	{
		count2++;
		if(count2>3000)
		{
			count2=0;
			flag1=1;
			if(flag2)
			{
				flag2=0;
				Key_Read_Data=KEY_L;
				
			}
		}
		if(P2_6==1)
		{
			key_down=0; 
			flag2=1;
			count2=0;
			if(flag1)
			{
				flag1=0;
			}
			else
			{
				Key_Read_Data=KEY_S;
			}
		}
	}
	else
	{
		if(P2_6==0)
		{
			count1++;
			if(count1>100);
			{
				count1=0;
				if(P2_6==0)
				{
					key_down=1;
				}
			}
		}
		else
		{
			count1=0;
		}
	}
}