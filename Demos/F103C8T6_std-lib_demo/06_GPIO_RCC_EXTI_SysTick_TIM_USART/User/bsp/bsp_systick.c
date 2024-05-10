#include "bsp_systick.h"


/* 使用 定时器中断处理函数 和 定时器滴答变量 实现延时 */
#ifdef SYSTICK_INT_DELAY_FLAG
    SysTick_Timer_Struct_Typedef SysTick_Timer_Struct = {0,0,0,0};
#else
/* 否则 通过读取控制状态寄存器的标志位次数实现延时 */
#endif

void SysTick_Delay_ms(volatile uint32_t count)
{
    // 设定系统滴答定时器 1ms 定时周期, 其中已使能 SysTick定时器；
    SysTick_Config(SystemCoreClock/1000);

    #ifdef SYSTICK_INT_DELAY_FLAG   /* 使用 定时器中断处理函数 和 定时器滴答变量 实现延时 */
    
        // 对定时器滴答变量赋初始值 并等待其与参数值匹配，实现阻塞延时
        SysTick_Timer_Struct.systick_counter = count;
        while(SysTick_Timer_Struct.systick_counter != 0);
    
    #else   /* 通过读取控制状态寄存器的标志位次数实现延时 */
    
        // 等待SysTick->CTRL标志位COUNTFLAG置1次数与参数值匹配，实现阻塞延时
        while(count--)
        {
            // 当计数器的值减小到0的时候，CRTL寄存器的位16会置1	
            while( !((SysTick->CTRL)&(SysTick_CTRL_COUNTFLAG_Msk)) );
        }
    
    #endif

    // 关闭SysTick定时器
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void SysTick_LED_Blink_Block(uint8_t times)
{
    while(times--)
    {
        GPIO_ResetBits(GPIOC,GPIO_Pin_13);
        SysTick_Delay_ms(500);
        GPIO_SetBits(GPIOC,GPIO_Pin_13);
        SysTick_Delay_ms(500);
    }
}


#ifdef SYSTICK_INT_DELAY_FLAG

/* Unblocking delay */
void SysTick_Timer_Init(uint32_t delay_ms)
{
    // 设定系统滴答定时器 1ms 定时周期, 其中已使能 SysTick定时器；
    SysTick_Config(SystemCoreClock/1000);
    // 定时器中断处理函数SysTick_Handler(),systick_counter--;至0,将systick_tigger_flag置1.
    SysTick_Timer_Struct.systick_delay_ms = delay_ms;
    SysTick_Timer_Struct.systick_counter = SysTick_Timer_Struct.systick_delay_ms;
    SysTick_Timer_Struct.systick_tigger_enable = 1;
    SysTick_Timer_Struct.systick_tigger_flag = 0;
}

void SysTick_Timer_Enable()
{
    SysTick_Timer_Struct.systick_tigger_enable = 1;
    SysTick_Timer_Struct.systick_tigger_flag = 0;
}
void SysTick_Timer_Disable()
{
    SysTick_Timer_Struct.systick_tigger_enable = 0;
    SysTick_Timer_Struct.systick_tigger_flag = 0;
}

void SysTick_Timer_Process_Loop(void)
{
    if (SysTick_Timer_Struct.systick_tigger_flag == 1)
    {
        SysTick_Timer_Struct.systick_counter = SysTick_Timer_Struct.systick_delay_ms;
        SysTick_Timer_Struct.systick_tigger_flag = 0;
        GPIOC->ODR ^= GPIO_Pin_13;
    }
}

#endif /* SYSTICK_INT_DELAY_FLAG */