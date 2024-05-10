/**
 * @file    04_GPIO_RCC_EXTI_SysTick\User\main.c
*/

#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_key.h"
// #include "bsp_rcc_mco.h"
#include "bsp_exti.h"
#include "bsp_systick.h"
#include "bsp_tim.h"


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
    // ADVANCE_TIM_Init();
    GENERAL_TIM_Init();

    /* Add your application code here */
    
    // LED_Blink_Block(3);
    LED_Blink_SysTick(3);

    /* loop */
    while(1)
    {
      Key_Process_Loop();
      // ADVANCE_TIM_Process_Loop();
      GENERAL_TIM_Process_Loop();
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
