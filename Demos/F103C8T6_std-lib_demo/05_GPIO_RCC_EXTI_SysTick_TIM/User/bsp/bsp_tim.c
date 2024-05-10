#include "bsp_tim.h"
#include "bsp_led.h"

/**
 * NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1),
 * NVIC_IRQChannel(PreemptionPriority,SubPriority):
 * EXTI0_IRQn(1,1);
 * ADVANCE_TIM1(0,1);
 * GENERAL_TIM2(0,2);
*/


/* Advance Timer 1 */
#ifdef ADVANCE_TIM1

unsigned int timer_1_counter = 0;

static void ADVANCE_TIM_NVIC_Config(void)
{
    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  // Have been setup in "exti.h" as NVIC_PriorityGroup_1.
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = ADVANCE_TIM_IRQ ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/* Fre = 72MHz / 72 / 1000 = 1000 Hz, Period = 1ms. */
static void ADVANCE_TIM_Mode_Config(void)
{
    ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM_CLK, ENABLE);

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Prescaler= ADVANCE_TIM_Prescaler; // all timer.(basic/advance/general)
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;   // available in advance/general timer.
    TIM_TimeBaseStructure.TIM_Period=ADVANCE_TIM_Period;        // all timer.(basic/advance/general)
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;       // available in advance/general timer.
    TIM_TimeBaseStructure.TIM_RepetitionCounter=0;              // available only in advance timer.
    TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);

    TIM_ClearFlag(ADVANCE_TIM, TIM_FLAG_Update);
    TIM_ClearITPendingBit(ADVANCE_TIM, TIM_IT_Update);
    TIM_ITConfig(ADVANCE_TIM,TIM_IT_Update,ENABLE);
    TIM_Cmd(ADVANCE_TIM, ENABLE);
}

void ADVANCE_TIM_Init(void)
{
    ADVANCE_TIM_NVIC_Config();
    ADVANCE_TIM_Mode_Config();
}

void ADVANCE_TIM_Process_Loop(void)
{
    if (timer_1_counter >= 500)
    {
        timer_1_counter = 0;
        LED_TOGGLE();
    }
}

#endif /* ADVANCE_TIM1 */


/* General Timer 2 */
#ifdef GENERAL_TIM2

unsigned int timer_2_counter = 0;

static void GENERAL_TIM_NVIC_Config(void)
{
    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  // Have been setup in "exti.h" as NVIC_PriorityGroup_1.
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQ ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/* Fre = 72MHz / 72 / 1000 = 1000 Hz, Period = 1ms. */
static void GENERAL_TIM_Mode_Config(void)
{
    GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK, ENABLE);

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM_Prescaler; // all timer.(basic/advance/general)
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;   // available in advance/general timer.
    TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM_Period;        // all timer.(basic/advance/general)
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;       // available in advance/general timer.
    TIM_TimeBaseStructure.TIM_RepetitionCounter=0;              // available only in advance timer.
    TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);

    TIM_ClearFlag(GENERAL_TIM, TIM_FLAG_Update);
    TIM_ClearITPendingBit(GENERAL_TIM, TIM_IT_Update);
    TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,ENABLE);
    TIM_Cmd(GENERAL_TIM, ENABLE);
}

void GENERAL_TIM_Init(void)
{
    GENERAL_TIM_NVIC_Config();
    GENERAL_TIM_Mode_Config();
}

void GENERAL_TIM_Process_Loop(void)
{
    if (timer_2_counter >= 500)
    {
        timer_2_counter = 0;
        LED_TOGGLE();
    }
}

#endif /* GENERAL_TIM2 */
