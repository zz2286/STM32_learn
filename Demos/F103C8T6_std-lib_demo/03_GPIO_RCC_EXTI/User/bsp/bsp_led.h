#ifndef __BSP_LED_H
#define	__BSP_LED_H


#include "stm32f10x.h"


/* 定义LED连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的LED引脚 */
#define LED_GPIO_PORT   GPIOC		              /* GPIO端口 */
#define LED_GPIO_CLK    RCC_APB2Periph_GPIOC		/* GPIO端口时钟 */
#define LED_GPIO_PIN    GPIO_Pin_13			        


/** the macro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */
#define ON  0
#define OFF 1

/* 使用标准的固件库控制IO*/
#define LED(a)	if (a)	\
                    GPIO_SetBits(LED_GPIO_PORT,LED_GPIO_PIN);\
                else	\
                    GPIO_ResetBits(LED_GPIO_PORT,LED_GPIO_PIN)


/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //输出为高电平		
#define digitalLo(p,i)		 {p->BRR=i;}	 //输出低电平
#define digitalToggle(p,i)	{p->ODR ^=i;} //输出反转状态


/* 定义控制IO的宏 */
#define LED_TOGGLE  digitalToggle(LED_GPIO_PORT,LED_GPIO_PIN)
#define LED_OFF     digitalHi(LED_GPIO_PORT,LED_GPIO_PIN)
#define LED_ON      digitalLo(LED_GPIO_PORT,LED_GPIO_PIN)



void LED_GPIO_Config(void);

#endif /* __BSP_LED_H */
