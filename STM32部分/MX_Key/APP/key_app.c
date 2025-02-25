#include "key_app.h"

//typedef struct
//{
//    GPIO_TypeDef *gpiox;
//    uint16_t pin;
//    uint16_t ticks;
//    uint8_t level;
//    uint8_t id;
//    uint8_t state;
//    uint8_t debouce_cnt;
//    uint8_t repeat;
//    
//}button;

button btns[4];

void key_init(void)
{
    //初始化第一个按键
    btns[0].gpiox = GPIOB;
    btns[0].pin = GPIO_PIN_0;
    btns[0].level =1;
    btns[0].id = 0;

    //初始化第二个按键
    btns[1].gpiox = GPIOB;
    btns[1].pin = GPIO_PIN_1;
    btns[1].level =1;
    btns[1].id = 1;

    //初始化第三个按键
    btns[2].gpiox = GPIOB;
    btns[2].pin = GPIO_PIN_2;
    btns[2].level =1;
    btns[2].id = 2;

    //初始化第四个按键
    btns[3].gpiox = GPIOA;
    btns[3].pin = GPIO_PIN_0;
    btns[3].level =1;
    btns[3].id = 4;
    
}

void key_task(button *btn)
{
    uint8_t gpio_level = HAL_GPIO_ReadPin(btn->gpiox, btn->pin);

    if(btn-> state > 0)
    {
        btn->ticks++;
    }

    if(btn->level != gpio_level)
    {
        if(++(btn->debouce_cnt) >= 3)
        {
            btn->level = gpio_level;
            btn->debouce_cnt = 0;
        }
    }
    else
    {
        btn->debouce_cnt = 0;
    }

    switch (btn->state)
    {
    case 0:                 //按键初始状态
        if(btn->level == 0)
        {
            btn->ticks = 0;
            btn->repeat = 1;
            btn -> state = 1;
        }
        break;
        
    case 1:                 //按键按下状态  
        if(btn->level != 0)//等待按键松开
        {
            if(btn->ticks >=30)//按键长按
            {
						
				ucled[btn->id] ^= 1;//长按执行的操作
                btn -> repeat = 0;//防止释放后再次进入触发单击事件

            }

            btn->ticks = 0;//重置计时器
            btn->state = 2;//进入按键释放状态
        }
		//此处else if（btn->ticks >= 30） 可进行按键持续长按的操作  如变量持续增加            
        break;
    
    case 2:                 //按键释放状态
        if(btn->ticks >= 15)
        {
            btn->state = 0;//返回初始状态

            //单击按键事件
            if(btn->repeat == 1)
            {
                //单击按键事件
				ucled[btn->id+4] ^= 1;
            }
            else if(btn->repeat == 2)
            {
				ucled[btn->id] ^= 1;
                //双击按键事件
            }
        }
        else
        {
            if(btn->level == 0)
            {
                btn->repeat++;//递增重复次数
                btn->ticks = 0;//重置计时器
                btn->state = 1;//返回按键按下状态
            }
        }

        break;
    }
}

void key_state(void)
{
    for(uint8_t i =0 ;i< 4;i++)
    {
        key_task(&btns[i]);
    }
}

