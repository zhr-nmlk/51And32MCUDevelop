void Delay1ms(unsigned int xms)		
{
	unsigned char i, j;
	while(xms){
	
		i = 12;
		j = 199;
		do
		{
			while (--j);
		} while (--i);	
	
		xms--;
	}

}