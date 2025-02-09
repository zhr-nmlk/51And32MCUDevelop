#ifndef __UART_APP_H__
#define __UART_APP_H__
	
#include "bsp_system.h"
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void uart_proc(void);

#endif
