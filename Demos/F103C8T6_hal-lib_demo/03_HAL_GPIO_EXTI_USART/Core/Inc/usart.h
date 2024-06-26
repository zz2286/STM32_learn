/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

#include <stdio.h>  // printf()
#include <string.h> // memset()
#include <stdarg.h> // va_start(),vsnprintf()

#include "gpio.h"

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

extern UART_HandleTypeDef huart3;

/* USER CODE BEGIN Private defines */

// #define USART1_INTERRUPT_DISABLE  /* Disable USART1 intertupt. */
#define USART1_INTERRUPT_ENABLE   /* Enable  USART1 intertupt. */
// #define USART1_INT_SOLUTION_1   /* HAL_UART_Receive_IT & HAL_UART_RxCpltCallback. */
// #define USART1_INT_SOLUTION_2   /* HAL_UART_Receive_IT & HAL_UART_RxCpltCallback & AAA_UART_IDLE_Callback. */
#define USART1_INT_SOLUTION_3   /* HAL_UARTEx_ReceiveToIdle_IT & HAL_UARTEx_RxEventCallback. */

// #define USART1_DMA_ENABLE         /* Enable  USART1 DMA. */
// #define USART1_DMA_DISABLE        /* Disable USART1 DMA. */


#define AAA_LOG_DEBUG(...)  AAA_USART1_Log("DEBUG", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define AAA_LOG_INFO(...)   AAA_USART1_Log("INFO", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);
void MX_USART3_UART_Init(void);

/* USER CODE BEGIN Prototypes */

void AAA_USART1_Log(const char *level, const char *file, const int line, const char *func,
                    const char *log_format, ...);
void AAA_USART1_Ctrl_CMD(uint8_t *RxBuffer);

void AAA_USART1_Demo_Main(void);
void AAA_USART1_Demo_Loop(void);
void AAA_USART1_Demo_Process(uint32_t tick_interval);

#if defined(USART1_INTERRUPT_ENABLE) && defined(USART1_INT_SOLUTION_1)
/* HAL_UART_Receive_IT & HAL_UART_RxCpltCallback. */
/* Receive only one char once IDLE, so cannot get uncertian length data by IDLE event. */
/* Unless get the appointed end-charactor or buffer full. */

#elif defined(USART1_INTERRUPT_ENABLE) && defined(USART1_INT_SOLUTION_2)
/* HAL_UART_Receive_IT & HAL_UART_RxCpltCallback & AAA_UART_IDLE_Callback. */
/* Receive full buffer or get IDLE event, so can get uncertain length data. */
void AAA_UART_IDLE_Callback(UART_HandleTypeDef *huart);

#elif defined(USART1_INTERRUPT_ENABLE) && defined(USART1_INT_SOLUTION_3)
/* HAL_UARTEx_ReceiveToIdle_IT & HAL_UARTEx_RxEventCallback. */
/* Same as Solution_2. Receive till either the expected number of data is received or an IDLE event occurs. */

#endif

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

