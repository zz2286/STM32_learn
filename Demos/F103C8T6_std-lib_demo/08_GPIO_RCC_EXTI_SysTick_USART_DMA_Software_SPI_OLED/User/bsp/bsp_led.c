#include "bsp_led.h"

/**
 * @brief  初始化控制LED的IO
 */
void LED_GPIO_Config(void)
{
    /* 开启LED相关的GPIO外设时钟 */
    RCC_APB2PeriphClockCmd(LED_GPIO_CLK, ENABLE);

    /* 定义一个GPIO_InitTypeDef类型的结构体，调用库函数，初始化GPIO */
    /* 选择要控制的GPIO引脚，设置引脚模式为通用推挽输出，引脚速率为50MHz */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = LED_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);

    /* 关闭led灯 */
    GPIO_SetBits(LED_GPIO_PORT, LED_GPIO_PIN);
}

/* 阻塞式延时函数, 需要volatile关键字 */
void Block_Delay_ms(volatile uint32_t count)
{
    /* STM32 => 1.25 MIPS/MHz => 90 MIPS */
    /* SYSCLK = 72MHz */
    count *= 5400;
    for( ; count !=0; count-- );
}

/** 
 * 阻塞式延时闪烁LED，时间不准确;
 * 如需准确延时闪烁LED，调用 LED_Blink_SysTick();
 */
void LED_Blink_Block(uint8_t times)
{
    while(times--)
    {
        GPIO_ResetBits(GPIOC,GPIO_Pin_13);
        Block_Delay_ms(500);
        GPIO_SetBits(GPIOC,GPIO_Pin_13);
        Block_Delay_ms(500);
    }
}