/* 寄存器版工程演示，点亮LED灯 */

#include "stm32f103c8t6.h"
#include "stm32f10x_gpio.h"

/* 延时函数 */
void delay_ms(uint32_t count)
{
	count *= 666;
	for( ; count !=0; count-- );
}

/* 主函数 */
int main(void)
{
#define MODE -1	// 控制GPIO寄存器的不同方法：(-1)地址指针/(0)地址指针宏/(1)结构体指针/(2)位操作函数及引脚宏/(3)自定义库函数

#if(MODE == -1)	// 通过 直接操作内存 地址指针 来控制寄存器
	// 开启GPIOC 端口时钟
	*(unsigned int*)0x40021018 |= (1<<4);

	// PC13 清空控制该引脚的的端口配置位
	*(unsigned int*)0x40011004 &= ~( 0x0F << (4*(13-8)));	
	// PC13 配置为通用推挽输出，速度为10M
	*(unsigned int*)0x40011004 |= (1<<(4*(13-8)));

	// PC13 输出低电平点亮
	*(unsigned int*)0x4001100C &= ~(1<<13);
	
	while(1)
	{
		// PC13 输出高电平熄灭
		*(unsigned int*)0x4001100C |= (1<<13);
		delay_ms(500);
		// PC13 输出低电平点亮
		*(unsigned int*)0x4001100C &= ~(1<<13);
		delay_ms(500);
	}
#elif(MODE == 0)	// 通过 直接操作内存 地址指针宏 来控制寄存器
	// 开启GPIOC 端口时钟
	RCC_APB2ENR |= (1<<4);

	// PC13 清空控制该引脚的的端口配置位
	GPIOC_CRH &= ~( 0x0F << (4*(13-8)));	
	// PC13 配置为通用推挽输出，速度为10M
	GPIOC_CRH |= (1<<(4*(13-8)));

	// PC13 输出低电平点亮
	GPIOC_ODR &= ~(1<<13);
	
	while(1)
	{
		// PC13 输出高电平熄灭
		GPIOC_ODR |= (1<<13);
		delay_ms(500);
		// PC13 输出低电平点亮
		GPIOC_ODR &= ~(1<<13);
		delay_ms(500);
	}

#elif(MODE == 1)	// 通过 寄存器结构体指针 来控制寄存器

	// 开启GPIOC 端口时钟
	RCC->APB2ENR |= (1<<4);

	// PC13 清空控制该引脚的的端口配置位
	GPIOC->CRH &= ~( 0x0F << (4*(13-8)));
	// PC13 配置为通用推挽输出，速度为10M
	GPIOC->CRH |= (1<<(4*(13-8)));

	// PC13 输出低电平点亮
	GPIOC->ODR &= ~(1<<13);
	
	while(1)
	{
		// PC13 输出高电平熄灭
		GPIOC->ODR |= (1<<13);
		delay_ms(500);
		// PC13 输出低电平点亮
		GPIOC->ODR &= ~(1<<13);
		delay_ms(500);
	}

#elif(MODE == 2)	// 通过 位操作函数及引脚宏 来控制寄存器

	// 开启GPIOC 端口时钟
	RCC->APB2ENR |= (1<<4);

	// PC13 清空控制该引脚的的端口配置位
	GPIOC->CRH &= ~( 0x0F << (4*(13-8)));
	// PC13 配置为通用推挽输出，速度为10M
	GPIOC->CRH |= (1<<(4*(13-8)));

	// PC13 输出低电平点亮
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	
	while(1)
	{
		// PC13 输出高电平熄灭
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
		delay_ms(500);
		// PC13 输出低电平点亮
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		delay_ms(500);
	}

#elif(MODE == 3)	// 通过 自定义库函数 来控制寄存器

#define   LED_GPIO_PORT                   GPIOC
#define   LED_GPIO_PIN                    GPIO_Pin_13
#define   LED_GPIO_CLK_ENABLE            (RCC->APB2ENR |= (1<<4))

	// 打开 GPIOB 端口的时钟
	LED_GPIO_CLK_ENABLE;

	// PC13 
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// PC13 输出低电平点亮
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	
	while(1)
	{
		// PC13 输出高电平熄灭
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
		delay_ms(500);
		// PC13 输出低电平点亮
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		delay_ms(500);
	}

#endif
}

/* 在启动文件中,Reset_Halder调用了系统时钟初始化函数SystemInit,
此处配置为空,是为了骗过编译器不报错,系统默认使用HSI=8M。
或者去启动文件startup_stm32f10x_xx.s中注释掉这两行:
LDR     R0, =SystemInit
BLX     R0
*/
void SystemInit(void)
{
}
