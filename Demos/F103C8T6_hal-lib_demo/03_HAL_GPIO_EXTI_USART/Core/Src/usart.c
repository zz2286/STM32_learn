/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}
/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* re-direct C library function <printf> to USART1_Tx. */
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}

/* re-direct C library function <getchar > to USART1_Rx. */
int fgetc(FILE *f)
{
  uint8_t ch = 0;
  HAL_UART_Receive(&huart1, &ch, 1, HAL_MAX_DELAY);
  return ch;
}


void AAA_USART1_Log(const char *level,  /* log level: DEBUG/INFO/... */
                    const char *file,
                    const int   line,
                    const char *func,
                    const char *log_format, ...)  /* __VA_ARGS__ = const char *log_format, ... */
{
  /* Define arguments' list object. */
  va_list args;
  /* Get time_ms since booting. */
  uint32_t time_ms = HAL_GetTick();

  /* Deal with different Log level. */
  if (strncmp(level, "DEBUG",5) == 0)
  {
    /* Print Log debug title. */
    printf("[%d.%03d][%s]: %s, %d, %s:\n", (time_ms/1000), (time_ms%1000), level, file, line, func);
  }
  else if (strncmp(level, "INFO",4) == 0)
  {
    /* Only print info. */
    printf("[%d.%03d][%s]: ", (time_ms/1000), (time_ms%1000), level);
  }

  /* __VA_ARGS__: (log_format, ...) */
  va_start(args, log_format);
  vprintf(log_format, args);
  va_end(args);
}


uint8_t Flag_USART1_Rx = 0;
uint8_t Char_USART1_Rx = 0;
uint8_t Counter_USART1_Rx = 0;

uint8_t Buff_USART1_Tx[64] = "{Data in Buff_USART1_Tx[64].}";
uint8_t Buff_USART1_Rx[64] = "{Data in Buff_USART1_Rx[64].}";
uint8_t Buff_USART1_Tx_IT[64] = "{Data in Buff_USART1_Tx_IT[64].}";
uint8_t Buff_USART1_Rx_IT[64] = "{Data in Buff_USART1_Rx_IT[64].}";


void AAA_USART1_Demo_Main(void)
{
  #if 0
  AAA_LOG_INFO("Put USART1_Tx: ");
  HAL_UART_Transmit(&huart1, Buff_USART1_Tx, sizeof(Buff_USART1_Tx), HAL_MAX_DELAY);
  HAL_UART_Transmit(&huart1, (uint8_t *)"\n", 1, HAL_MAX_DELAY);
  #else
  AAA_LOG_DEBUG("Put USART1_Tx: %s\n", Buff_USART1_Tx);
  #endif
  memset(Buff_USART1_Rx, 0, sizeof(Buff_USART1_Rx));
}


void AAA_USART1_Demo_Loop(void)
{
  #if 0 /* Loopback: echo buff. */
  if (HAL_UART_Receive(&huart1, Buff_USART1_Rx, sizeof(Buff_USART1_Rx), 0xFFFF) == HAL_OK)
  {
    AAA_LOG_DEBUG("Get USART1_Rx.");
    HAL_UART_Transmit(&huart1, Buff_USART1_Rx, sizeof(Buff_USART1_Rx), HAL_MAX_DELAY);
  }
  #else /* Loopback: log message. */
  
  if (HAL_UART_Receive(&huart1, &Char_USART1_Rx, 1, 0) == HAL_OK)
  {
    /* Get max len = sizeof(buff), max inedx = sizeof(buff)-1. */
    if ((Flag_USART1_Rx == 0) && (Counter_USART1_Rx <= sizeof(Buff_USART1_Rx)-1))
    {
      Buff_USART1_Rx[Counter_USART1_Rx] = Char_USART1_Rx;
      Counter_USART1_Rx ++;
      /* Get end-of-message character '.' or Buffer overflow. */
      if ((Char_USART1_Rx == '.') || (Counter_USART1_Rx == sizeof(Buff_USART1_Rx)))
      {
        /* Replace final character to '\0'. */
        Buff_USART1_Rx[Counter_USART1_Rx-1] = '\0';
        Flag_USART1_Rx = 1;
      }
    }
  }

  AAA_USART1_Demo_Process(50);

  #endif
}

void AAA_USART1_Demo_Process(uint32_t tick_interval)
{
  static uint32_t tick_counter = 0;
  if ((HAL_GetTick() - tick_counter) < tick_interval) return;
  tick_counter = HAL_GetTick();
  
  if (Flag_USART1_Rx == 1)
  {
    /* Print Log. */
    #if 0
    AAA_LOG_INFO("Get USART1_Rx: ");
    HAL_UART_Transmit(&huart1, Buff_USART1_Rx, sizeof(Buff_USART1_Rx), HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1, (uint8_t *)"\n", 1, HAL_MAX_DELAY);
    #else
    AAA_LOG_DEBUG("Get USART1_Rx: %s\n",Buff_USART1_Rx);
    #endif
    /* Clear. */
    Flag_USART1_Rx = 0;
    Char_USART1_Rx = 0;
    Counter_USART1_Rx = 0;
    memset(Buff_USART1_Rx, 0, sizeof(Buff_USART1_Rx));
  }
}


/* USER CODE END 1 */
