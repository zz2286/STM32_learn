/**
 * @brief pass
*/

#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_key.h"
// #include "bsp_rcc_mco.h"
#include "bsp_exti.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_tim.h"

/**
 * NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1),
 * NVIC_IRQChannel(PreemptionPriority,SubPriority):
 * EXTI0/1_IRQn(1,1);
 * USART1_IRQn(1,0)
 * ADVANCE_TIM1(0,1);
 * GENERAL_TIM2(0,2);
*/



int main(void)
{
    /* Initialization */
    /**
     * 启动文件 statup_stm32f10x_md.s, 在程序来到main主函数之前，已经调用 SystemInit,
     * 标准库文件 system_stm32f10x.c 定义了 SystemInit 系统初始化函数,
     * PLLCLK = SYSCLK = HCLK = PCLK2 = 72MHz, PCLK1 = HCLK/2 = 36MHz. 
    */

    /* 如需 RCC 重设系统时钟，或 MCO 时钟输出 */
    // RCC_MCO_Init();

    // LED,Key 引脚初始化
    LED_GPIO_Config();
    Key_GPIO_Config();  // PA2,3内部上拉输入
    EXTI_Key_Config();  // PA0,1内部上拉输入,上升沿/下降沿中断,会使LED取反.
    
    // LED_Blink_Block(3);
    SysTick_LED_Blink_Block(3);
    // unblocking delay in 1000ms.
    // SysTick_Timer_Init(1000);

    /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
    USART_Config();
    /* 发送一个字符串 */
    // Usart_SendString( DEBUG_USARTx,"这是一个串口中断接收回显实验\n");
    // printf("来自于重定向c库函数printf到串口\n\n");
    printf("Test1: 高级定时器TIM1控制带断路和死区功能的PWM互补输出模式,引脚PA8/PB13.\n");
    printf("Test2: 通用定时器TIM2测量按键GPIO电平信号的脉宽时间,引脚PA2.\n");
    printf("Test3: 通用定时器TIM3调制输出PWM,引脚PA6.\n");
    printf("Test4: 通用定时器TIM4输入捕获PWM,引脚PB6.\n");
    printf("Test5: 滴答定时器SysTick控制LED按串口指令设定周期闪烁.\n");

    /* Timer */
    ADVANCE_TIM1_Init();
    GENERAL_TIM2_Init();
    GENERAL_TIM3_Init();
    GENERAL_TIM4_Init();

    /* loop */
    while(1)
    {
      SysTick_Timer_Process_Loop();
      Key_Process_Loop();
      USART_Process_Loop();
      // ADVANCE_TIM1_Process_Loop();
      GENERAL_TIM2_Process_Loop();
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
