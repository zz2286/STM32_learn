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
#include "bsp_dma.h"


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

#if 0
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
#endif /* 0 */

void SysTick_Handler(void)
{
  SysTick_Timer_IRQHandler();
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
/**
 * 串口中断处理函数 选择串口接收方式: USART_IRQHANDLER_MODE
 * 0_ 接收中断 & 回显,    1_ 接收中断 & 指令,    2_ 空闲中断 & DMA(标志/中断) & 回显,    3_ 空闲中断 & DMA(标志/中断) & 指令
 */
#define USART_IRQHANDLER_MODE 3

#if (USART_IRQHANDLER_MODE==0)/* ---------- 串口中断接收后发送实现回显 ---------- */
  uint8_t ucTemp;
  if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
  {
    ucTemp = USART_ReceiveData(DEBUG_USARTx);
    USART_SendData(DEBUG_USARTx,ucTemp);
  }
#elif (USART_IRQHANDLER_MODE==1) /* ---------- 串口中断接收指令处理控制led ---------- */
  if (USART_GetITStatus(DEBUG_USARTx, USART_IT_RXNE)!=RESET)
  {
    // 无论是否识别指令，进中断都先读取接收到的字符，以此自动清除 USART_IT_RXNE 中断标志位.
    // 或者在合适位置使用 USART_ClearITPendingBit 清除对应标志位。
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
    // 判定接收完成后，不再继续接收
    if(CmdReceived==0)
    {
      // 判断是否接收到"."，表示指令结束
      if (receivedChar == '.' )
      {
        // 标记指令接收完成
        CmdReceived = 1;
      }
      else
      {
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
      }
    }
  }
#elif (USART_IRQHANDLER_MODE>=2) /* ---------- DMA_USART1_Rx 串口中断接收 回显 ---------- */

  extern volatile uint8_t Dst_SRAM_Rx_Buffer[];

  // 串口空闲中断
  if(USART_GetITStatus(DEBUG_USARTx,USART_IT_IDLE) == SET)
  {
  #if(DMA_IT_DISABLE_OR_ENABLE==0) /* 不使用DMA中断, 在轮询中使用DMA标志, 缓冲区溢出覆盖 且 输出终止位之前 */

    // 关闭 DMA 使能
    DMA_Cmd(DMA_CHANNEL_USART1_RX,DISABLE);
    // 获取剩余的数据数量
    uint16_t DMA_Data_Counter = DMA_GetCurrDataCounter(DMA_CHANNEL_USART1_RX);
    // 通过串口返回数据（接收数据数量 = SENDBUFF_SIZE - 剩余未传输的数据数量）
    Usart_SendArray(DEBUG_USARTx,(uint8_t *)Dst_SRAM_Rx_Buffer,DMA_BUFFER_SIZE_USART1_RX-DMA_Data_Counter);
    // 重新设置传输的数据数量
    DMA_SetCurrDataCounter(DMA_CHANNEL_USART1_RX,DMA_BUFFER_SIZE_USART1_RX);
    // 打开 DMA 使能
    DMA_Cmd(DMA_CHANNEL_USART1_RX,ENABLE);

    // 读取一次数据，不然会一直进中断，因为是串口接收的 空闲中断，使用后续无输入不影响
    USART_ReceiveData(DEBUG_USARTx);
    // 清除串口空闲中断标志位
    USART_ClearFlag(DEBUG_USARTx,USART_FLAG_IDLE);

  #elif(DMA_IT_DISABLE_OR_ENABLE==1) /* 使用DMA中断, 可接收 不定长数据 且 回显*/

    // 获取剩余的数据数量
    uint16_t DMA_Data_Counter = DMA_GetCurrDataCounter(DMA_CHANNEL_USART1_RX);
    // 通过串口返回数据
    // Usart_SendArray(DEBUG_USARTx,(uint8_t *)Dst_SRAM_Rx_Buffer,DMA_BUFFER_SIZE_USART1_RX-DMA_Data_Counter);

    // 这次 恰好半满或溢满，DMA 中断处理函数 会输出，USART 中断就不再重复输出
    if((DMA_Data_Counter==DMA_BUFFER_SIZE_USART1_RX/2)||(DMA_Data_Counter==DMA_BUFFER_SIZE_USART1_RX)||(DMA_Data_Counter==0))
    {
      // N.A.
      printf("\nDMA_USART_2_SRAM: Receive Complete.\n");
    }
    // 这次 过半满尚未溢满, 上一次只可能是 恰好半满, 输出从 半满位 到 这次结束位
    else if (DMA_Data_Counter < DMA_BUFFER_SIZE_USART1_RX/2)
    {
      for(int i=(DMA_BUFFER_SIZE_USART1_RX/2); i<(DMA_BUFFER_SIZE_USART1_RX-DMA_Data_Counter); i++)
      {
        printf("%c", Dst_SRAM_Rx_Buffer[i]);  // 回显.
      }
      printf("\nDMA_USART_2_SRAM: Receive Complete.\n");
    }
    // 这次尚未半满,上一次只可能是 全满, 输出从 零位 到 这次结束位
    else if(DMA_Data_Counter > DMA_BUFFER_SIZE_USART1_RX/2)
    {
      for(int i=0; i<(DMA_BUFFER_SIZE_USART1_RX-DMA_Data_Counter); i++)
      {
        printf("%c", Dst_SRAM_Rx_Buffer[i]);  // 回显.
      }
      printf("\nDMA_USART_2_SRAM: Receive Complete.\n");

      // 如果需要接收指令(这里假设约定指令数据量不超过半满), 这里可以添加指令读取功能......
      #if (USART_IRQHANDLER_MODE>=3) /* ---------- DMA_USART1_Rx 串口中断接收 指令控制LED ---------- */
        // 指令被处理标记后, 再进串口接收中断时, 才继续接收新指令.
        if(CmdReceived==0)
        {
          // 再次接收指令前, 先清空缓冲区.
          memset((void*)RxBuffer, 0, RX_BUFFER_SIZE);
          RxIndex = 0;
          // 接收指令.
          for(int i=0; i<(DMA_BUFFER_SIZE_USART1_RX-DMA_Data_Counter); i++)
          {
            // 判断是否接收到".", 表示指令结束.
            if (Dst_SRAM_Rx_Buffer[i] == '.' ) { break; }
            // 未接收到".", 继续读取直到 读完 或者 溢出.
            else
            {
              // 可以存 RX_BUFFER_SIZE-1 个字符串字符，留最后一位给'\0'.
              if (RxIndex < RX_BUFFER_SIZE-1) { RxBuffer[RxIndex++] = Dst_SRAM_Rx_Buffer[i]; }
              // 防止缓冲区溢出.
              else if (RxIndex == RX_BUFFER_SIZE-1)
              {
                RxBuffer[RxIndex] = '\0';
                // 发送字符溢出报错.
                char strings[80];
                sprintf(strings, "USART_IRQHander Cmd Read Warning: RxIndex exceeded RX_BUFFER_SIZE: %d.\n", RX_BUFFER_SIZE);
                Usart_SendString(DEBUG_USARTx, strings);
                break;
              }
            }
          }
          // 标记指令接收完成, 下次轮询中的USART_Process_Loop()会执行指令
          CmdReceived = 1;
        }
      #endif  /* USART_IRQHANDLER_MODE >=3 */
    }

    /* 重置 DMA 剩余数据量 */
    // 关闭 DMA 使能
    DMA_Cmd(DMA_CHANNEL_USART1_RX,DISABLE);
    // 重新设置传输的数据数量
    DMA_SetCurrDataCounter(DMA_CHANNEL_USART1_RX,DMA_BUFFER_SIZE_USART1_RX);
    // 打开 DMA 使能
    DMA_Cmd(DMA_CHANNEL_USART1_RX,ENABLE);

    // 读取一次数据，不然会一直进中断，因为是串口接收的 空闲中断，使用后续无输入不影响
    USART_ReceiveData(DEBUG_USARTx);
    // 清除串口空闲中断标志位
    USART_ClearFlag(DEBUG_USARTx,USART_FLAG_IDLE);
  #endif /* DMA_IT_DISABLE_OR_ENABLE == 0,1 */

  }
#endif  /* USART_IRQHANDLER_MODE ==0,1; >=2 */
}
#endif  /* __BSP_USART_H */


/* ============================ DMA 中断处理函数 ================================ */
#if defined(__BSP_DMA_H) && (DMA_IT_DISABLE_OR_ENABLE==1)

extern volatile uint8_t Dst_SRAM_Rx_Buffer[];
// DMA 通道5 中断处理函数
void DMA_USART1_RX_IRQHandler(void)
{
  // DMA 半满中断, 输出从 零位 到 半满位
  if(DMA_GetITStatus(DMA_IT_HT_USART_RX) == SET)
  {
    for(int i=0; i<(DMA_BUFFER_SIZE_USART1_RX/2); i++)
    {
      printf("%c", Dst_SRAM_Rx_Buffer[i]);
    }
    DMA_ClearITPendingBit(DMA_IT_HT_USART_RX);
  }
  // DMA 半满中断, 输出从 半满位 到 溢满位
  else if (DMA_GetITStatus(DMA_IT_TC_USART_RX) == SET)
  {
    for(int i=(DMA_BUFFER_SIZE_USART1_RX/2); i<DMA_BUFFER_SIZE_USART1_RX; i++)
    {
      printf("%c", Dst_SRAM_Rx_Buffer[i]);
    }
    DMA_ClearITPendingBit(DMA_IT_TC_USART_RX);
  }
  // 还需要配合 USART_IDLE 串口空闲中断, 对接收末尾进行判定、输出和重置
}

#endif /* defined(__BSP_DMA_H) && (DMA_IT_DISABLE_OR_ENABLE==1) */



