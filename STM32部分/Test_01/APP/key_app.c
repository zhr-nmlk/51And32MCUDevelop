#include "key_app.h"

uint8_t key_val = 0;
uint8_t key_old = 0;
uint8_t key_down = 0;
uint8_t key_up = 0;

uint8_t key_read(void)
{
	uint8_t temp =0;
	
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0) == GPIO_PIN_RESET)
	{
		temp = 1;
	}
	
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1) == GPIO_PIN_RESET)
	{
		temp = 2;
	}
	
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2) == GPIO_PIN_RESET)
	{
		temp = 3;
	}
	
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == GPIO_PIN_RESET)
	{
		temp = 4;
	}
	
	return temp;
}

void key_proc(void)
{
	key_val = key_read();
	
	key_down = key_val & (key_old ^ key_val);
	
	key_up = ~key_val & (key_old ^ key_val);
	
	key_old = key_val;
	
	if(key_down != 0)
	{
		ucled[key_down] ^= 1;
	}
}
