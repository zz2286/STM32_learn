/**
 * @file    pass
*/

#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_key.h"
// #include "bsp_rcc_mco.h"
#include "bsp_exti.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_dma.h"
#include "bsp_oled.h"

/**
 * NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1),
 * NVIC_IRQChannel(PreemptionPriority,SubPriority):
 * EXTI0/1_IRQn(1,1);
 * DMA_USART1_RX_IRQ(1,0);
 * USART1_IRQn(1,1);
*/


int main(void)
{
  /* Initialization */

  /* 启动文件 statup_stm32f10x_md.s, 在程序来到main主函数之前，已经调用 SystemInit */
  /* 标准库文件 system_stm32f10x.c 定义了 SystemInit 系统初始化函数 */
  /* PLLCLK = SYSCLK = HCLK = PCLK2 = 72MHz, PCLK1 = HCLK/2 = 36MHz. */

  /* 如需 RCC 重设系统时钟，或 MCO 时钟输出 */
  // RCC_MCO_Init();

  // LED,Key 引脚初始化
  LED_GPIO_Config();
  Key_GPIO_Config();  // PA2,3内部上拉输入
  EXTI_Key_Config();  // PA0,1内部上拉输入,上升沿/下降沿中断,会使LED取反.
  
  // SysTick.
  SysTick_Timer_Demo();

  /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
  USART_Config();

  /* 发送一个字符串 */
  // Usart_SendString( DEBUG_USARTx,"这是一个串口中断接收回显实验\n");
  // printf("来自于重定向c库函数printf到串口\n\n");
  printf("Test1: DMA+USART实现串口接收不定长数据并回显.\n");
  printf("Test2: 通过串口读取CMD+LED+<ON/OFF/TOGGLE/BLINK=ms*>指令控制LED.\n");
  printf("Test3: 滴答定时器SysTick控制LED按串口BLINK指令设定周期闪烁.\n");
  printf("Test4: 滴答定时器SysTick计算main函数的每秒轮询次数.\n");
  
  // DMA_USART1 串口测试
  DMA_Demo();
    

  // OLED
  OLED_Demo();

  /* loop */
  while(1)
  {
    SysTick_Timer_Main_Loop();
    Key_Process_Loop();
    USART_Process_Loop();
    DMA_Process_loop();
    OLED_Process_Loop();

  }
  return 0;
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
