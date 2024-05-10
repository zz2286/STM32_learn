/**
  ******************************************************************************
  * @project GPIO/Project/MDK-ARM/Peoject_GPIO.uvprojx 
  * @file    GPIO/User/main.c 
  ******************************************************************************
  */

#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_key.h"


/* 简单的延时函数, 需要volatile关键字 */
void Delay_ms(volatile uint32_t count)
{
    /* STM32 => 1.25 MIPS/MHz */
	count *= 5400;
	for( ; count !=0; count-- );
}

void LED_Blink_Block(uint8_t times)
{
	while(times--)
	{
        GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		Delay_ms(500);
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
		Delay_ms(500);
	}
}
void Key_Scan_Task(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==0){Delay_ms(20);
    if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==0)
    {
        LED_ON;
        while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0);
        LED_OFF;
    }}
}
void Key_Process_Loop(void)
{
    // Key-PA0,单击松开后反置
    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0){Delay_ms(20);
    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0)
    {
        LED_ON;
        while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 0);
        LED_OFF;
        // LED_TOGGLE;
    }}
    // Key-PA1,单击松开前反置
    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0){Delay_ms(20);
    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0)
    {
        LED_ON;
        while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == 0);
        LED_OFF;
    }}
    // Key-PA23,按下亮,松开灭
    Key_Scan_Task(GPIOA, GPIO_Pin_2);
    Key_Scan_Task(GPIOA, GPIO_Pin_3);
}

int main(void)
{

    /* Initialization */
    /* 标准库文件system_stm32f10x.c 定义了 SystemInit系统初始化函数 */
    /* PLLCLK = SYSCLK = HCLK = PCLK2 = 72MHz, PCLK1 = HCLK/2 = 36MHz. */

    /* LED 端口初始化 */
    LED_GPIO_Config();
    Key_GPIO_Config();

    /* Add your application code here */
    
    LED_Blink_Block(3);

    /* loop */
    while(1)
    {
        Key_Process_Loop();
    }
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
