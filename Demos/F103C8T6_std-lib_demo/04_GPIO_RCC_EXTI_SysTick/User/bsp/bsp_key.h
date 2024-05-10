#ifndef __BSP_KEY_H
#define	__BSP_KEY_H


#include "stm32f10x.h"

//  引脚定义
// #define    KEY1_GPIO_CLK    RCC_APB2Periph_GPIOA
// #define    KEY1_GPIO_PORT   GPIOA
// #define    KEY1_GPIO_PING   GPIO_Pin_0

/*  PA0, PA1, PA2, PA3  */
typedef enum KEY_STATE {KEY_OFF = 0, KEY_ON = !KEY_OFF} KEY_State;


void Key_GPIO_Config(void);
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);

void Key_Process_Loop(void);


#endif /* __BSP_KEY_H */
