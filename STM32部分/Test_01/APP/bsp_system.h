#ifndef BSP_SYSTEM_H
#define BSP_SYSTEM_H

#include "main.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stdarg.h"

#include "system.h"
#include "scheduler.h"
#include "led_app.h"
#include "key_app.h"
#include "lcd.h"
#include "lcd_app.h"
#include "uart_app.h"

extern uint8_t ucled[8];
extern uint16_t uart_rx_index;
extern uint32_t uart_rx_ticks;
extern uint8_t uart_rx_buffer[128];

#endif
