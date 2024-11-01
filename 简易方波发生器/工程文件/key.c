#include <REGX52.H>

extern unsigned char State;

void KeyNumber()
{
	static unsigned char count1;
	static unsigned int count2;
	static unsigned int count3;
	static unsigned int count4;

	static bit flag1=0;
	static bit flag2=1;

	static bit KeyDown=0;
	static bit KeyDown2=0;
	
	if(KeyDown==1)
	{
		count2++;
		if(count2>10000)
		{
			count2=0;
			flag1=1;

			if(flag2)
			{
				flag2=0;
				State=1;
			}
		}
		
		
		if(P3_0==1)
		{		
			KeyDown=0;
			flag2=1;
			count2=0;
				if(flag1)
				  {
					flag1=0;
				}
				else
				{
					State=2;
				}	
		}
		if(KeyDown2)
		{
			State=3;  
		}
	}
	else
	{
		if(P3_0==0)
		{
			count1++;
			if(count1>20)
			{
				count1=0;
				if(P3_0==0)
				{
					KeyDown=1;
				}
				///////
				for(count3;count3<600;count3++)
				{
					count4++;
					if(count4>30)
					{
						count4=0;
						if(P3_0==0)
						{
							KeyDown2=1;
						}
					}
				}
				//////
			}
		
		}
		else
		{
			count1=0;
		}
	}
}
