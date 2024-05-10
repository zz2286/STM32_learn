#include "bsp_exti.h"

/** 外部中断按键 配置函数 Key_GPIO_Config
  * 将 PA0|PA1 内部上拉输入, 上升沿/下降沿中断
  */
void EXTI_Key_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 配置 GPIO & AFIO 时钟 */
    RCC_APB2PeriphClockCmd((RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO),ENABLE);

    /* 配置 GPIO 输入 */
    // 选择按键用到的GPIO,配置为内部上拉输入.
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // 选择按键用到的GPIO,配置为内部上拉输入.
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* 配置 外部中断控制寄存器AFIO_EXTICRx */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);

    /* 配置 EXTI 中断 */
    // 选择EXTI的信号源,EXTI为中断模式,上升沿中断,使能中断.
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    // 选择EXTI的信号源,EXTI为中断模式,下降沿中断,使能中断.
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* 配置 NVIC 中断 */
    // 选择NVIC优先级分组为组1, 这在整个工程只需要调用一次.
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    // 选择中断源为按键PA0,选择抢占优先级,选择子优先级,使能中断通道.
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    // 选择中断源为按键PA1,其它配置同上.
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_Init(&NVIC_InitStructure);
}
