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

#define USART1_INTERRUPT_DISABLE  /* Disable USART1 intertupt. */
// #define USART1_INTERRUPT_ENABLE   /* Enable  USART1 intertupt. */
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

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

