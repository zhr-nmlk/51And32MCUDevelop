#include "uart_app.h"

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart -> Instance == USART1)
	{
		uart_rx_ticks = HAL_GetTick();
		uart_rx_index++;
		HAL_UART_Receive_IT(&huart1,&uart_rx_buffer[uart_rx_index],1);
		
	}
}

void uart_proc(void)
{
	if(uart_rx_index == 0) 
		return;
	
	if(HAL_GetTick() - uart_rx_ticks > 100)
	{
		printf("uart data:%s\n",uart_rx_buffer);
		
		memset(uart_rx_buffer,0,uart_rx_index);
		uart_rx_index = 0;
		huart1.pRxBuffPtr = uart_rx_buffer;
	}
}
