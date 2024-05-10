#include "bsp_systick.h"
#include <stdio.h>

volatile uint16_t systick_delay_counter = 0;

SysTick_Timer_Struct_Typedef SysTick_Timer_List[MAX_TIMER_COUNT];

uint8_t main_loop_counter = 0;
uint8_t main_loop_memory = 0;

void SysTick_Timer_Start(SysTick_Timer_ID id, int repeat, uint32_t target)
{
    SysTick_Timer_List[id].id = id;
    SysTick_Timer_List[id].repeat = repeat;
    SysTick_Timer_List[id].counter = 0;
    SysTick_Timer_List[id].target = target;
}

void SysTick_Timer_Stop(SysTick_Timer_ID id)
{
    SysTick_Timer_List[id].repeat = 0;
}


void SysTick_Timer_Init(void)
{
    /* 设定系统滴答定时器 1ms 定时周期, 其中已使能 SysTick定时器 */
    SysTick_Config(SystemCoreClock/1000);
    uint8_t i = 0;
    for (i=0;i<MAX_TIMER_COUNT;i++)
    {
        SysTick_Timer_List[i].id = i;
        SysTick_Timer_List[i].repeat = 0;
        SysTick_Timer_List[i].counter = 0;
        SysTick_Timer_List[i].target = 0;
        SysTick_Timer_List[i].tigger_int = 0;
        SysTick_Timer_List[i].tigger_flag = 0;
    }
}

void SysTick_Timer_Demo(void)
{
    SysTick_Timer_Init();
    /* SysTick_Timer_Start */
    SysTick_Timer_Start(TIMER_ID_LED_BLINK, 6, 500);
    SysTick_Timer_Start(TIMER_ID_MAIN_LOOP_PS, -1, 1000);
    SysTick_Timer_List[TIMER_ID_MAIN_LOOP_PS].tigger_int = 1;
}

void SysTick_Timer_Main_Loop(void)
{
    main_loop_counter ++ ;
    // Deal with SysTick_Timer Tigger.
    uint8_t i = 0;
    for (i=0;i<MAX_TIMER_COUNT;i++)
    {
        /* Timer available. */
        if (SysTick_Timer_List[i].repeat != 0) 
        {
            /* Tigger by main loop. */
            if ((SysTick_Timer_List[i].tigger_int == 0)
            && (SysTick_Timer_List[i].counter >= SysTick_Timer_List[i].target))
            {
                /* Callback Function. */
                SysTick_Timer_Loop_Callback(SysTick_Timer_List[i].id);
                /* Do After Tigger. */
                SysTick_Timer_List[i].counter = 0;
                if (SysTick_Timer_List[i].repeat >= 1)
                {
                    SysTick_Timer_List[i].repeat -- ;
                }
                else if (SysTick_Timer_List[i].repeat == -1)
                {
                    /* Always Tigger, No Times Limit. */
                }
            }
            /* Tigger by systick interrupt. */
            else if ((SysTick_Timer_List[i].tigger_int == 1)
            && (SysTick_Timer_List[i].tigger_flag == 1))
            {
                /* Callback Function. */
                SysTick_Timer_Loop_Callback(SysTick_Timer_List[i].id);
                /* Reser tigger_flag to ensure tigger callback only once. */
                SysTick_Timer_List[i].tigger_flag = 0;
            }
        }
    }
}

void SysTick_Timer_Loop_Callback(SysTick_Timer_ID id)
{
    switch (id)
    {
        case TIMER_ID_EMPTY: break;
        case TIMER_ID_LED_BLINK:
        {
            /* Toggle LED PC13. */
            GPIOC->ODR ^= GPIO_Pin_13;
            break;
        }
        case TIMER_ID_MAIN_LOOP_PS:
        {
            /* Print MAIN_LOOP_PS. */
            if (SysTick_Timer_List[TIMER_ID_MAIN_LOOP_PS].tigger_int == 0)
            {
                main_loop_memory = main_loop_counter;
                main_loop_counter = 0;
            }
            printf("Main_Loop_Per_Second: %d.\n", main_loop_memory);
            break;
        }
        case TIMER_ID_ELSE: 
        {
            /* Do Nothing. */
            break;
        }
        default: break;
    }
}

void SysTick_Block_Delay_ms(uint16_t ms)
{
    systick_delay_counter = ms;
    while(systick_delay_counter);
}


void SysTick_Timer_IRQHandler(void)
{
    if (systick_delay_counter > 0)
    {
        systick_delay_counter--;
    }
    uint8_t i = 0;
    for (i=0;i<MAX_TIMER_COUNT;i++)
    {
        /* counter adder for all available Timer. */
        if (SysTick_Timer_List[i].repeat != 0)
        {
            SysTick_Timer_List[i].counter += 1;
        }
    }
    /* IRQ if timer tiggered by systick interrupt. */
    if ((SysTick_Timer_List[TIMER_ID_MAIN_LOOP_PS].tigger_int == 1)
    && (SysTick_Timer_List[TIMER_ID_MAIN_LOOP_PS].counter >= SysTick_Timer_List[TIMER_ID_MAIN_LOOP_PS].target))
    {
        SysTick_Timer_List[TIMER_ID_MAIN_LOOP_PS].counter = 0;
        main_loop_memory = main_loop_counter;
        main_loop_counter = 0;
        // printf("in it.c: %d.\n", main_loop_memory);
        SysTick_Timer_List[TIMER_ID_MAIN_LOOP_PS].tigger_flag = 1;
    }
}


#if 0
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

#endif /* 0 */