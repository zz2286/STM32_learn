#include "bsp_tim.h"
#include "bsp_led.h"

/**
 * NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1),
 * NVIC_IRQChannel(PreemptionPriority,SubPriority):
 * EXTI0/1_IRQn(1,1);
 * USART1_IRQn(1,0)
 * ADVANCE_TIM1(0,1);
 * GENERAL_TIM2(0,3);
 * GENERAL_TIM4(0,2);
*/
// printf("Test1: 高级定时器TIM1控制带断路和死区功能的PWM互补输出模式,引脚PA8/PB13.\n");
// printf("Test2: 通用定时器TIM2测量按键GPIO电平信号的脉宽时间,引脚PA2.\n");
// printf("Test3: 通用定时器TIM3调制输出PWM,引脚PA6.\n");
// printf("Test4: 通用定时器TIM4输入捕获PWM,引脚PB6.\n");

/* ============================== Advance Timer 1 ============================== */
#ifdef ADVANCE_TIM1_CNT

unsigned int timer_1_counter = 0;

static void ADVANCE_TIM1_NVIC_Config(void)
{
    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  // Have been setup in "exti.h" as NVIC_PriorityGroup_1.
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = ADVANCE_TIM1_IRQ ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/* Fre = 72MHz / 72 / 1000 = 1000 Hz, Period = 1ms. */
static void ADVANCE_TIM1_Mode_Config(void)
{
    ADVANCE_TIM1_APBxClock_FUN(ADVANCE_TIM1_CLK, ENABLE);

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Prescaler= ADVANCE_TIM1_Prescaler; // all timer.(basic/advance/general)
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;   // available in advance/general timer.
    TIM_TimeBaseStructure.TIM_Period=ADVANCE_TIM1_Period;        // all timer.(basic/advance/general)
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;       // available in advance/general timer.
    TIM_TimeBaseStructure.TIM_RepetitionCounter=0;              // available only in advance timer.
    TIM_TimeBaseInit(ADVANCE_TIM1, &TIM_TimeBaseStructure);

    TIM_ClearFlag(ADVANCE_TIM1, TIM_FLAG_Update);
    TIM_ClearITPendingBit(ADVANCE_TIM1, TIM_IT_Update);
    TIM_ITConfig(ADVANCE_TIM1,TIM_IT_Update,ENABLE);
    TIM_Cmd(ADVANCE_TIM1, ENABLE);
}

void ADVANCE_TIM1_Init(void)
{
    ADVANCE_TIM1_NVIC_Config();
    ADVANCE_TIM1_Mode_Config();
}

/**
 * @brief Advance Timer 1 control LED blink each 2s.
*/
void ADVANCE_TIM1_Process_Loop(void)
{
    if (timer_1_counter >= 1000)
    {
        timer_1_counter = 0;
        LED_TOGGLE();
    }
}

#endif /* ADVANCE_TIM1_CNT */



#ifdef ADVANCE_TIM1_OC

unsigned int timer_1_counter = 0;

static void ADVANCE_TIM1_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // TIM1_CH1 Compare Output Channel GPIO (PA8).
    ADVANCE_TIM1_CH1_GPIO_CLK_CMD(ADVANCE_TIM1_CH1_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM1_CH1_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ADVANCE_TIM1_CH1_GPIO_PORT, &GPIO_InitStructure);
    // TIM1_CH1N Compare Output Complement Channel GPIO (PB13).
    ADVANCE_TIM1_CH1N_GPIO_CLK_CMD(ADVANCE_TIM1_CH1N_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM1_CH1N_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ADVANCE_TIM1_CH1N_GPIO_PORT, &GPIO_InitStructure);
    // TIM1 Compare Output Channel BKIN (Break Input) GPIO (PB12).
    ADVANCE_TIM1_BKIN_GPIO_CLK_CMD(ADVANCE_TIM1_BKIN_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM1_BKIN_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ADVANCE_TIM1_BKIN_GPIO_PORT, &GPIO_InitStructure);
    // Reset BKIN as default.
    GPIO_ResetBits(ADVANCE_TIM1_BKIN_GPIO_PORT,ADVANCE_TIM1_BKIN_GPIO_PIN);
}

/* Fre = 72MHz / 72 / 1000 = 1000 Hz, Period = 1ms. */
static void ADVANCE_TIM1_Mode_Config(void)
{
    ADVANCE_TIM1_APBxClock_FUN(ADVANCE_TIM1_CLK, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Prescaler= ADVANCE_TIM1_Prescaler; // all timer.(basic/advance/general)
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;   // available in advance/general timer.
    TIM_TimeBaseStructure.TIM_Period=ADVANCE_TIM1_Period;        // all timer.(basic/advance/general)
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;       // available in advance/general timer.
    TIM_TimeBaseStructure.TIM_RepetitionCounter=0;              // available only in advance timer.
    TIM_TimeBaseInit(ADVANCE_TIM1, &TIM_TimeBaseStructure);
    
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = ADVANCE_TIM1_Pulse;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    TIM_OC1Init(ADVANCE_TIM1, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(ADVANCE_TIM1, TIM_OCPreload_Enable);

    TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
    TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;
    TIM_BDTRInitStructure.TIM_DeadTime = 11; // 11=0b 000 01011, T = 1/72M*1*11 = 1.39ns*11 = 152 ns.
    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
    TIM_BDTRConfig(ADVANCE_TIM1, &TIM_BDTRInitStructure);

    TIM_Cmd(ADVANCE_TIM1, ENABLE);
    // Enable Output. This step is not necessary for general timers.
    TIM_CtrlPWMOutputs(ADVANCE_TIM1, ENABLE);
}

void ADVANCE_TIM1_Init(void)
{
    ADVANCE_TIM1_GPIO_Config();
    ADVANCE_TIM1_Mode_Config();
}

/**
 * @brief Advance Timer 1 complementary output with break and dead-time.
*/
void ADVANCE_TIM1_Process_Loop(void)
{
    // Do Nothing.
}

#endif /* ADVANCE_TIM1_OC */



/* ============================== General Timer 2 ============================== */
#ifdef GENERAL_TIM2

// unsigned int timer_2_counter = 0;
TIM2_IC_Info_Struct_Typedef TIM2_IC_Info_Struct = {0,0,0,0};

static void GENERAL_TIM2_NVIC_Config(void)
{
    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  // Have been setup in "exti.h" as NVIC_PriorityGroup_1.
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM2_IRQ ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

#if 0
/* Same code in Key_GPIO_Config(). */
static void GENERAL_TIM2_GPIO_Config(void) 
{
    GENERAL_TIM2_CH3_GPIO_CLK_CMD(GENERAL_TIM2_CH3_GPIO_CLK, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM2_CH3_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GENERAL_TIM2_CH3_GPIO_PORT, &GPIO_InitStructure);
}
#endif

/* Fre = 72MHz / 72 / 1000 = 1000 Hz, Period = 1ms. */
static void GENERAL_TIM2_Mode_Config(void)
{
    GENERAL_TIM2_APBxClock_FUN(GENERAL_TIM2_CLK, ENABLE);

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM2_Prescaler; // all timer.(basic/advance/general)
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;   // available in advance/general timer.
    TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM2_Period;        // all timer.(basic/advance/general)
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;       // available in advance/general timer.
    TIM_TimeBaseStructure.TIM_RepetitionCounter=0;              // available only in advance timer.
    TIM_TimeBaseInit(GENERAL_TIM2, &TIM_TimeBaseStructure);

    // TIM Input Capture.
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICInitStructure.TIM_Channel = GENERAL_TIM2_CHx;                  // 配置输入捕获的通道，需要根据具体的GPIO来配置
    TIM_ICInitStructure.TIM_ICPolarity = GENERAL_TIM2_STRAT_ICPolarity;  // 输入捕获信号的极性配置
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;     // 输入通道和捕获通道的映射关系，有直连和非直连两种
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;               // 输入的需要被捕获的信号的分频系数
    TIM_ICInitStructure.TIM_ICFilter = 0;                               // 输入的需要被捕获的信号的滤波系数
    TIM_ICInit(GENERAL_TIM2, &TIM_ICInitStructure);

    // TIM_ClearFlag(GENERAL_TIM, TIM_FLAG_Update | TIM_FLAG_CC3);
    TIM_ClearITPendingBit(GENERAL_TIM2, TIM_IT_Update | GENERAL_TIM2_IT_CCx);
    TIM_ITConfig(GENERAL_TIM2,TIM_IT_Update | GENERAL_TIM2_IT_CCx, ENABLE);
    TIM_Cmd(GENERAL_TIM2, ENABLE);
}

void GENERAL_TIM2_Init(void)
{
    GENERAL_TIM2_NVIC_Config();
    GENERAL_TIM2_Mode_Config();
}

/**
 * @brief General Timer_2 Channel_3/4 Input Capture Key PA2/3 GPIO width.
 * @details PA0至PA3对应为TIM2_CH1至CH4
 * @note interrupt releated code in "it.c"
*/
void GENERAL_TIM2_Process_Loop(void)
{
    unsigned int counter = 0;
    unsigned int counter_per_second = 72000000 / (GENERAL_TIM2_Prescaler + 1);
    if(TIM2_IC_Info_Struct.IC_End_Flag == 1)
    {
        TIM2_IC_Info_Struct.IC_End_Flag = 0;
        // 计算高电平时间的计数器的值
        counter = TIM2_IC_Info_Struct.IC_Period * (GENERAL_TIM2_Period + 1 ) + (TIM2_IC_Info_Struct.IC_CCR_Value + 1);
        
        // 打印高电平脉宽时间
        printf("按键引脚PA2的GPIO电平信号脉宽时间: %d.%06d s.\n", counter/counter_per_second, counter%counter_per_second);
    }
}

#endif /* GENERAL_TIM2 */

/* ============================== General Timer 3 ============================== */
#ifdef GENERAL_TIM3_OC

static void GENERAL_TIM3_GPIO_Config(void) 
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(GENERAL_TIM3_CH1_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM3_CH1_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GENERAL_TIM3_CH1_GPIO_PORT, &GPIO_InitStructure);
}

static void GENERAL_TIM3_Mode_Config(void)
{
    GENERAL_TIM3_APBxClock_FUN(GENERAL_TIM3_CLK,ENABLE);
    
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM3_Prescaler;
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM3_Period;
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
    TIM_TimeBaseInit(GENERAL_TIM3, &TIM_TimeBaseStructure);

    TIM_OCInitTypeDef  TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = GENERAL_TIM3_Pulse;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OC1Init(GENERAL_TIM3, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(GENERAL_TIM3, TIM_OCPreload_Enable);
    // TIM_OC2Init(GENERAL_TIM3, &TIM_OCInitStructure);
    // TIM_OC2PreloadConfig(GENERAL_TIM3, TIM_OCPreload_Enable);
    TIM_Cmd(GENERAL_TIM3, ENABLE);
}

void GENERAL_TIM3_Init(void)
{
    GENERAL_TIM3_GPIO_Config();
    GENERAL_TIM3_Mode_Config();
}

#endif /* GENERAL_TIM3_OC */

/* ============================== General Timer 4 ============================== */
#ifdef GENERAL_TIM4

static void GENERAL_TIM4_NVIC_Config(void)
{
    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  // Have been setup in "exti.h" as NVIC_PriorityGroup_1.
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM4_IRQ ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
static void GENERAL_TIM4_GPIO_Config(void) 
{
    GENERAL_TIM2_CH3_GPIO_CLK_CMD(GENERAL_TIM4_CH1_GPIO_CLK, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM4_CH1_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GENERAL_TIM4_CH1_GPIO_PORT, &GPIO_InitStructure);
}
static void GENERAL_TIM4_Mode_Config(void)
{
    GENERAL_TIM4_APBxClock_FUN(GENERAL_TIM4_CLK, ENABLE);

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM4_Prescaler;
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM4_Period;
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
    TIM_TimeBaseInit(GENERAL_TIM4, &TIM_TimeBaseStructure);

    // TIM Input Capture.
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICInitStructure.TIM_Channel = GENERAL_TIM4_CHx;                  // 配置输入捕获的通道，需要根据具体的GPIO来配置
    TIM_ICInitStructure.TIM_ICPolarity = GENERAL_TIM4_STRAT_ICPolarity;  // 输入捕获信号的极性配置
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;     // 输入通道和捕获通道的映射关系，有直连和非直连两种
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;               // 输入的需要被捕获的信号的分频系数
    TIM_ICInitStructure.TIM_ICFilter = 0;                               // 输入的需要被捕获的信号的滤波系数
    TIM_PWMIConfig(GENERAL_TIM4, &TIM_ICInitStructure);

    /* Select the TIM4 Input Trigger: TI1FP1 . (Trigger to detect period.)*/
    TIM_SelectInputTrigger(GENERAL_TIM4, TIM_TS_TI1FP1);
    /* Select the slave Mode: Reset Mode */
    TIM_SelectSlaveMode(GENERAL_TIM4, TIM_SlaveMode_Reset);
    /* Enable the Master/Slave Mode */
    TIM_SelectMasterSlaveMode(GENERAL_TIM4, TIM_MasterSlaveMode_Enable);
    /* Enable the CC2 Interrupt Request */
    TIM_ITConfig(GENERAL_TIM4, GENERAL_TIM4_IT_CCx, ENABLE);

    TIM_ClearITPendingBit(GENERAL_TIM4, TIM_IT_Update | GENERAL_TIM4_IT_CCx);
    /* TIM enable counter */
    TIM_Cmd(GENERAL_TIM4, ENABLE);

}

void GENERAL_TIM4_Init(void)
{
    GENERAL_TIM4_GPIO_Config();
    GENERAL_TIM4_NVIC_Config();
    GENERAL_TIM4_Mode_Config();
}
#endif /* GENERAL_TIM4 */

