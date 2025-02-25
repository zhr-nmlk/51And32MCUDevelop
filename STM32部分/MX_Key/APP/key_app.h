#ifndef __KEY_APP_H__
#define __KEY_APP_H__

#include "bsp_system.h"

typedef struct
{
    GPIO_TypeDef *gpiox;
    uint16_t pin;
    uint16_t ticks;
    uint8_t level;
    uint8_t id;
    uint8_t state;
    uint8_t debouce_cnt;
    uint8_t repeat;
    
}button;

void key_init(void);
void key_task(button *btn);
void key_state(void);

#endif
