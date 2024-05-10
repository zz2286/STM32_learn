#ifndef __BSP_SYSTICK_H
#define __BSP_SYSTICK_H

#include "stm32f10x.h"

/* 宏定义 SYSTICK_INT_DELAY_FLAG 用于引导 main.c 和 stm32f10x_it.c 文件执行不同代码 */
/* 使用 定时器中断处理函数 和 定时器滴答变量 实现延时 */
/* 否则 通过读取控制状态寄存器的标志位次数实现延时 */
#define SYSTICK_INT_DELAY_FLAG

typedef struct
{
    volatile uint32_t systick_counter;
    uint32_t systick_delay_ms;
    uint8_t systick_tigger_enable;
    uint8_t systick_tigger_flag;
}SysTick_Timer_Struct_Typedef;



void SysTick_Delay_ms(volatile uint32_t count);
void SysTick_LED_Blink_Block(uint8_t times);


#ifdef SYSTICK_INT_DELAY_FLAG

void SysTick_Timer_Init(uint32_t delay_ms);
void SysTick_Timer_Process_Loop(void);

void SysTick_Timer_Enable();
void SysTick_Timer_Disable();

#endif /* SYSTICK_INT_DELAY_FLAG */

#endif /* __BSP_SYSTICK_H */
