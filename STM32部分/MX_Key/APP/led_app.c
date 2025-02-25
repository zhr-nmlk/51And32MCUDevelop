#include "led_app.h"

void led_disp(uint8_t *ucled)
{
	uint8_t temp = 0x00;
	static uint8_t temp_old = 0x00;
	
	for(uint8_t i=0;i<8;i++)
	{
		temp |= ucled[i] << (7-i);//  ucled [8]={1,0,1,0,1,0,1,0}
	}
	
	if(temp != temp_old)
	{
		GPIOC->ODR &= 0x00ff;
		GPIOC->ODR |= ~(temp << 8);
		GPIOD->BSRR |= 0x01 << 2;
		GPIOD->BRR |= 0x01 << 2;
		temp_old = temp;
	}
}

uint8_t ucled[8]={0,0,0,0,0,0,0,1};

void led_proc(void)
{
	led_disp(ucled);
}

