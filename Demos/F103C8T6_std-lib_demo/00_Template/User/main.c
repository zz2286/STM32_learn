/**
  ******************************************************************************
  * @project Template/Project/MDK-ARM/Peoject_Template.uvprojx 
  * @file    Template/User/main.c 
  ******************************************************************************
  */

#include "stm32f10x.h"
#include <stdio.h>


int main(void)
{

	/* Initialization */
  /* 标准库文件system_stm32f10x.c 定义了 SystemInit系统初始化函数 */
  /* PLLCLK = SYSCLK = HCLK = PCLK2 = 72MHz, PCLK1 = HCLK/2 = 36MHz. */
  
	/* Add your application code here */

	/* loop */
	while (1)
	{
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
