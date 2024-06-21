#ifndef __BSP_LED_H
#define	__BSP_LED_H


#include "stm32f10x.h"
#include <stdio.h>


/* 定义LED连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的LED引脚 */
#define LED_GPIO_PORT   GPIOC
#define LED_GPIO_PIN    GPIO_Pin_13
#define LED_GPIO_CLK    RCC_APB2Periph_GPIOC


/** Enumerated value representing  the states of the LED
  * LED_ON = 0 ,
  * LED_OFF = 1
  */
// typedef enum LED_STATE {LED_ON = 0, LED_OFF = 1} LED_State;

/* 使用标准的固件库控制IO*/
#define LED_GPIO_Set(a) {\
    if(a)   GPIO_SetBits(LED_GPIO_PORT,LED_GPIO_PIN);\
    else    GPIO_ResetBits(LED_GPIO_PORT,LED_GPIO_PIN);}

/* 直接操作寄存器的方法控制IO */
// BSRR,BRR,ODR,三个寄存器在设置或清除某一位时，不会影响寄存器中的其他位。
#define GPIO_Register_SetBits(port,pin)     {port->BSRR = pin;} // 输出为高电平
#define GPIO_Register_ResetBits(port,pin)   {port->BRR = pin;}  // 输出为低电平
#define GPIO_Register_ToggleBits(port,pin)  {port->ODR ^= pin;}  // 输出反转状态

/* 使用宏定义模拟函数,无参数,调用时()可有可无 */
/* 如果在串口模块中重定向了fputc输出, 则会通过串口发送 LED 执行情况 */
#define LED_ON()        {printf("Run LED_ON().\n");GPIO_Register_ResetBits(LED_GPIO_PORT,LED_GPIO_PIN);}
#define LED_OFF()       {printf("Run LED_OFF().\n");GPIO_Register_SetBits(LED_GPIO_PORT,LED_GPIO_PIN);}
#define LED_TOGGLE()    {printf("Run LED_TOGGLE().\n");GPIO_Register_ToggleBits(LED_GPIO_PORT,LED_GPIO_PIN);}


void LED_GPIO_Config(void);
void LED_Blink_Block(uint8_t times);

void Block_Delay_ms(volatile uint32_t count);

#endif /* __BSP_LED_H */
