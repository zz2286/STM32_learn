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
#include "bsp_usart.h"
#include "bsp_tim.h"



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

/* 使用 定时器中断处理函数 和 定时器滴答变量 实现延时 */
#ifdef SYSTICK_INT_DELAY_FLAG
  extern SysTick_Timer_Struct_Typedef SysTick_Timer_Struct;
#else
/* 否则 通过读取控制状态寄存器的标志位次数实现延时 */
#endif

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  /* 使用 定时器中断处理函数 和 定时器滴答变量 实现延时 */
  #ifdef SYSTICK_INT_DELAY_FLAG
    if (SysTick_Timer_Struct.systick_counter != 0)
    {
      SysTick_Timer_Struct.systick_counter--;
    }
    if ((SysTick_Timer_Struct.systick_counter == 0) 
    && (SysTick_Timer_Struct.systick_tigger_flag == 0) 
    && (SysTick_Timer_Struct.systick_tigger_enable == 1))
    {
      SysTick_Timer_Struct.systick_tigger_flag = 1;
    }
  #else
  /* 否则 通过读取控制状态寄存器的标志位次数实现延时 */
  #endif
}


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


/* ========================== 外部中断按键 中断处理函数 ============================== */
#ifdef __BSP_EXTI_H

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


/* ============================ USART 中断处理函数 ================================ */
#ifdef __BSP_USART_H

/* 使用USART串口收发 */
extern volatile char RxBuffer[];
extern volatile uint8_t RxIndex;
extern volatile uint8_t CmdReceived;
extern volatile uint8_t CmdReset;


void DEBUG_USART_IRQHandler(void)
{
#if 0/* ------------------ 串口接收后发送实现回显 -------------------- */
  uint8_t ucTemp;
  if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
  {
    ucTemp = USART_ReceiveData(DEBUG_USARTx);
    USART_SendData(DEBUG_USARTx,ucTemp);
  }
#else /* ----------------- 串口指令处理控制led ----------------------- */
  if (USART_GetITStatus(DEBUG_USARTx, USART_IT_RXNE)!=RESET)
  {
    // 无论是否识别指令，进中断都先读取接收到的字符，以此自动清除 USART_IT_RXNE 中断标志位.
    char receivedChar = USART_ReceiveData(DEBUG_USARTx);
    // 指令处理后，再进串口接收中断时，先清空缓冲区
    if (CmdReset)
    {
      CmdReset = 0;
      // 调用 memset 执行期间, 其它中断对 RxBuffer 的再次操作存在风险。
      // 这个(void*)只是针对memset的参数进行转换以忽略编译器的警告。
      memset((void*)RxBuffer, 0, RX_BUFFER_SIZE);
      RxIndex = 0;
    }

    // // 判断是否接收到"."，表示指令结束
    // if (receivedChar == '.' )
    // {
    //   // 标记指令接收完成
    //   CmdReceived = 1;
    // }
    // else
    // {
      // 可以存RX_BUFFER_SIZE-1个字符串字符，留最后一位给'\0'
      if (RxIndex < RX_BUFFER_SIZE-1)
      {
        // 将字符存入接收缓冲区
        RxBuffer[RxIndex++] = receivedChar;
      }
      // 防止缓冲区溢出
      else if (RxIndex == RX_BUFFER_SIZE-1)
      {
        RxBuffer[RxIndex] = '\0';
        // 发送字符溢出报错
        char strings[50];
        sprintf(strings, "Error: RxIndex exceeded RX_BUFFER_SIZE: %d.\n", RX_BUFFER_SIZE);
        Usart_SendString(DEBUG_USARTx, strings);
        // 标记指令接收完成
        CmdReceived = 1;
      }
    // }
  }
  else if (USART_GetITStatus(DEBUG_USARTx, USART_IT_IDLE) != RESET)
  {
    // 标记指令接收完成
    CmdReceived = 1;
    // USART_ClearITPendingBit(DEBUG_USARTx, USART_IT_IDLE); // 无效
    // USART_ClearFlag(DEBUG_USARTx, USART_IT_IDLE); // 无效
    USART_ReceiveData(DEBUG_USARTx);
  }
#endif /* 0 */
}
#endif  /* __BSP_USART_H */


#ifdef __BSP_TIM_H

/* ==============================　Advance Timer 1 ==============================　*/
#ifdef ADVANCE_TIM1_CNT
extern unsigned int timer_1_counter;
void ADVANCE_TIM_IRQHandler(void)
{
  if (TIM_GetITStatus(ADVANCE_TIM,TIM_IT_Update) == SET)
  {
    timer_1_counter += 1;
    TIM_ClearITPendingBit(ADVANCE_TIM,TIM_IT_Update);
  }
}
#endif /* ADVANCE_TIM1_CNT */


/* ==============================　General Timer 2 ==============================　*/
#ifdef GENERAL_TIM2_IC
// extern unsigned int timer_2_counter;
extern TIM2_IC_Info_Struct_Typedef TIM2_IC_Info_Struct;

void GENERAL_TIM2_IRQHandler(void)
{
  if (TIM_GetITStatus(GENERAL_TIM2,TIM_IT_Update) == SET)
  {
    // timer_2_counter += 1;
    TIM2_IC_Info_Struct.IC_Period += 1;
    TIM_ClearITPendingBit(GENERAL_TIM2,TIM_IT_Update);
  }
  // ---- \_/-\ ___ to detect falling and rising.
  if (TIM_GetITStatus(GENERAL_TIM2,GENERAL_TIM2_IT_CCx) == SET)
  {
    // key pressed and triggered start_polarity (falling).
    if (TIM2_IC_Info_Struct.IC_Start_Flag == 0)
    {
      TIM_SetCounter(GENERAL_TIM2, 0);
      TIM2_IC_Info_Struct.IC_Period = 0;
      TIM2_IC_Info_Struct.IC_CCR_Value = 0;
      // set to detect next end_polarity (rising).
      GENERAL_TIM2_OCxPolarityConfig_CMD(GENERAL_TIM2,GENERAL_TIM2_END_ICPolarity);
      TIM2_IC_Info_Struct.IC_Start_Flag = 1;
      printf("TIM2_CH3 get start_polarity falling.\n");
    }
    // key released and triggered end_polarity (raising).
    else if (TIM2_IC_Info_Struct.IC_Start_Flag == 1)
    {
      TIM2_IC_Info_Struct.IC_CCR_Value = GENERAL_TIM2_GetCapturex_CMD(GENERAL_TIM2);
      // set to detect next start_polarity (falling).
      GENERAL_TIM2_OCxPolarityConfig_CMD(GENERAL_TIM2, GENERAL_TIM2_STRAT_ICPolarity);
      TIM2_IC_Info_Struct.IC_Start_Flag = 0;
      TIM2_IC_Info_Struct.IC_End_Flag = 1;
      printf("TIM2_CH3 get end_polarity rising.\n");
    }
    TIM_ClearITPendingBit(GENERAL_TIM2,GENERAL_TIM2_IT_CCx);
  }
}
#endif /* GENERAL_TIM2_IC */

/* ==============================　General Timer 4 ==============================　*/
#ifdef GENERAL_TIM4_IC

__IO uint16_t IC1Value = 0;
__IO uint16_t IC2Value = 0;
__IO float DutyCycle = 0;
__IO float Frequency = 0;

void GENERAL_TIM4_IRQHandler(void)
{
// Method-1: In one PWM_Period to count TIM_CCR:
//// PWM_Fre = TIM_Fre_cnt / TIM_CCR = (1M/CCR) Hz;
//// TIM_CCR(2-1000) => PWM_Fre(1KHz-500KHz). TIM_CCR(2-10)

  if (TIM_GetITStatus(GENERAL_TIM4,GENERAL_TIM4_IT_CCx) == SET)
  {
    /* Get the Input Capture value */
    IC1Value = TIM_GetCapture1(GENERAL_TIM4);
    IC2Value = TIM_GetCapture2(GENERAL_TIM4);
    // printf("IC1Value = %d,  C2Value = %d.\n",IC1Value,IC2Value);
    if (IC1Value == 0)
    {
      DutyCycle = 0;
      Frequency = 0;
    }
    else
    {
      /* Frequency computation */
      Frequency = (72000000/(GENERAL_TIM4_Prescaler+1))/(float)(IC1Value+1);
      /* Duty cycle computation */
      DutyCycle = (float)((IC2Value+1) * 100) / (IC1Value+1);
      printf("Frequency = %0.2fHz,  DutyCycle = %0.2f%%.\n",Frequency,DutyCycle);
    }
    /* Clear TIM4 Capture compare interrupt pending bit */
    TIM_ClearITPendingBit(GENERAL_TIM4,GENERAL_TIM4_IT_CCx);
  }
}

#endif /* GENERAL_TIM4_IC */

#endif /* __BSP_TIM_H */

