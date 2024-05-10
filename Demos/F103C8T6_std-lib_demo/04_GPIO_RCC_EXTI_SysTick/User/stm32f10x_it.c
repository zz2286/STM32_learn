/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.6.0
  * @date    20-September-2021
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2011 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp_led.h"
#include "bsp_exti.h"
#include "bsp_systick.h"


/* 使用 定时器中断处理函数 和 定时器滴答变量 实现延时 */
#ifdef SYSTICK_INT_DELAY_FLAG
  extern volatile unsigned int systick_counter;
#else
/* 否则 通过读取控制状态寄存器的标志位次数实现延时 */
#endif

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  /* 使用 定时器中断处理函数 和 定时器滴答变量 实现延时 */
  #ifdef SYSTICK_INT_DELAY_FLAG
    if(systick_counter != 0)
    {
      systick_counter--;
    }
  #else
  /* 否则 通过读取控制状态寄存器的标志位次数实现延时 */
  #endif
}

/* 外部中断按键 中断处理函数 */
#ifdef __BSP_EXTI_H

// 外部中断按键 EXTI_KEY
// PA0 上升沿
void EXTI_KEY_PA0_IRQHandler(void)
{
  //确保是否产生了EXTI Line中断
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
    Block_Delay_ms(20);
    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 1)
    {
      LED_TOGGLE();
    }
  }
  //清除中断标志位
  EXTI_ClearITPendingBit(EXTI_Line0);
}
// PA1 下降沿
void EXTI_KEY_PA1_IRQHandler(void)
{
  //确保是否产生了EXTI Line中断
  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
  {
    Block_Delay_ms(20);
    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == 0)
    {
      LED_TOGGLE();
    }
  }
  //清除中断标志位
  EXTI_ClearITPendingBit(EXTI_Line1);
}

#endif /* __BSP_EXTI_H */

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


