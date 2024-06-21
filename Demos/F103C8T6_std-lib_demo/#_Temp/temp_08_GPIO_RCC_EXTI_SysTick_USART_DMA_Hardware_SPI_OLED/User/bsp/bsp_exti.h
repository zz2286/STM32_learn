#ifndef __BSP_EXTI_H
#define	__BSP_EXTI_H


#include "stm32f10x.h"

//引脚定义

// 外部中断 按键KEY0 - PA0
#define KEY0_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)
#define KEY0_INT_GPIO_PORT         GPIOA
#define KEY0_INT_GPIO_PIN          GPIO_Pin_0
#define KEY0_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA
#define KEY0_INT_EXTI_PINSOURCE    GPIO_PinSource0
#define KEY0_INT_EXTI_LINE         EXTI_Line0
#define KEY0_INT_EXTI_IRQ          EXTI0_IRQn

/* 外部中断处理函数 EXTIx_IRQHandler:
* 在 startup_stm32f10x_md.s 启动文件中声明,空函数初始化中断向量表;
* 在 stm32f10x_it.c 源文件中,重新编写中断处理函数功能;
* 在 bsp_exti.h 或 stm32f10x_it.h 头文件中,对中断处理函数名进行宏定义.
*/
#define EXTI_KEY_PA0_IRQHandler EXTI0_IRQHandler
#define EXTI_KEY_PA1_IRQHandler EXTI1_IRQHandler

// 按键KEY1 - PA1

void EXTI_Key_Config(void);

#endif /* __BSP_EXTI_H */