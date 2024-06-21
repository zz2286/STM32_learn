#ifndef __BSP_SYSTICK_H
#define __BSP_SYSTICK_H

#include "stm32f10x.h"

#define MAX_TIMER_COUNT TIMER_ID_MAX

typedef enum _systick_timer_id
{
    TIMER_ID_EMPTY = 0,
    TIMER_ID_LED_BLINK = 1,
    TIMER_ID_MAIN_LOOP_PS,
    TIMER_ID_OLED_DELAY,
    TIMER_ID_ELSE,
    TIMER_ID_MAX
}SysTick_Timer_ID;


typedef struct
{
    SysTick_Timer_ID id;
    int repeat;         // Callback Times, @param -1 no times limit, @param 0 disable, @param 1-2147483647 repeat times.
    uint32_t counter;
    uint32_t target;
    uint8_t tigger_int;
    uint8_t tigger_flag;
}SysTick_Timer_Struct_Typedef;


void SysTick_Timer_Start(SysTick_Timer_ID id, int repeat, uint32_t target);
void SysTick_Timer_Stop(SysTick_Timer_ID id);
void SysTick_Timer_Init(void);
void SysTick_Timer_Demo(void);
void SysTick_Timer_Main_Loop(void);

void SysTick_Timer_Loop_Callback(SysTick_Timer_ID id);

void SysTick_Block_Delay_ms(uint16_t ms);
void SysTick_Timer_IRQHandler(void);














#if 0
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
#endif /* 0 */

#endif /* __BSP_SYSTICK_H */
