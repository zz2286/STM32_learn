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

  #ifdef USART1_INTERRUPT_ENABLE
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
  #endif

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

/* ==================== General situations. ==================== */

/* re-direct C library function to USART1_Tx. */
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}

/* re-direct C library function to USART1_Rx. */
int fgetc(FILE *f)
{
  uint8_t ch = 0;
  HAL_UART_Receive(&huart1, &ch, 1, HAL_MAX_DELAY);
  return ch;
}

/**
 * @brief USART1 print log in blocking mode.
 * @param level "DEBUG","INFO".
 * @note To be called by marco AAA_LOG_DEBUG() or AAA_LOG_INFO().
 * @note #define F(...) f(__VA_ARGS__)
 * @note (const char *log_format, ...) = __VA_ARGS__
 */
void AAA_USART1_Log(const char *level, const char *file, const int line, const char *func,
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

/**
 * @brief USART1 read command and control peripheral.
 * @param RxBuffer data to be read as <perfix> + <command>.
 * @note CMD+LED+_ON/OFF/TOGGLE/BLINK=ms
 * @note CMD+BT+_<AT command of Bluetooth>
 */
void AAA_USART1_Ctrl_CMD(uint8_t *RxBuffer)
{
  /* Control LED. */
  if (strncmp((char*)RxBuffer, "CMD+LED+", strlen("CMD+LED+")) == 0)
  {
    /* Pointer command to remove RxBuffer's "CMD+LED+" prefix. */
    const char *command = (char*)RxBuffer + strlen("CMD+LED+");
    /* Deal with command. */
    if (strncmp(command, "ON",2) == 0)
    {
      AAA_LOG_INFO("Get CMD+LED+ON and set led on.\n");
      AAA_LED_ON();
      // SysTick_Timer_Stop(TIMER_ID_LED_BLINK);
    }
    else if (strncmp(command, "OFF",3) == 0)
    {
      AAA_LOG_INFO("Get CMD+LED+OFF and set led off.\n");
      AAA_LED_OFF();
      // SysTick_Timer_Stop(TIMER_ID_LED_BLINK);
    }
    else if (strncmp(command, "TOGGLE",6) == 0)
    {
      AAA_LOG_INFO("Get CMD+LED+TOGGLE and set led toggle.\n");
      AAA_LED_TOGGLE();
      // SysTick_Timer_Stop(TIMER_ID_LED_BLINK);
    }
    else if (strncmp(command, "BLINK",5) == 0)
    {
      #if 0
      uint32_t blink_value = 0;
      sscanf(command, "BLINK=%d", &blink_value);
      AAA_LOG_INFO("Get CMD+LED+BLINK and set led blink in %d ms.\n",blink_value);
      SysTick_Timer_Start(TIMER_ID_LED_BLINK,-1,blink_value);
      #else
      AAA_LOG_INFO("CMD+LED+BLINK=ms function is not yet implemented.\n");
      #endif
    }
    else
    {
      AAA_LOG_INFO("Failed to get valid CMD+LED+<ON/OFF/TOGGLE/BLINK=ms*> .\n");
    }
  }
  /* Control Bluetooth. */
  else if (strncmp((char*)RxBuffer, "CMD+BT+", strlen("CMD+BT+")) == 0)
  {
    AAA_LOG_INFO("CMD+BT+ function is not yet implemented.\n");
  }
  /* Control nothing. Not read as command. */
  else
  {
    /* Do nothing. */
    AAA_LOG_INFO("Failed to read command.");
  }
}

/* ==================== Different situations. ==================== */

/* Disable  USART1 intertupt. */
#ifdef USART1_INTERRUPT_DISABLE

uint8_t Flag_USART1_Rx = 0;
uint8_t Char_USART1_Rx = 0;
uint8_t Cntr_USART1_Rx = 0;
uint8_t Buff_USART1_Tx[65] = "Data in Buff_USART1_Tx[65].";
uint8_t Buff_USART1_Rx[65] = "Data in Buff_USART1_Rx[65].";
const char *info = "\n\
Mode: USART1 interrput disable.\n\
Help:\n\
1. Get Buff_USART1_Rx[65] end with '.' or max len 64 or idle event;\n\
2. Get <CMD+LED+?.> or <CMD+BT+?.>.\n\
\n";

void AAA_USART1_Demo_Main(void)
{
  AAA_LOG_INFO(info);
  AAA_LOG_DEBUG("Print USART1_Tx: [%s]\n", Buff_USART1_Tx);
  AAA_LOG_INFO("Getting USART1_Rx...\n");
  memset(Buff_USART1_Rx, 0, sizeof(Buff_USART1_Rx));
}

void AAA_USART1_Demo_Loop(void)
{
  #if 0 /* Only echo buff. */
  if (HAL_UART_Receive(&huart1, Buff_USART1_Rx, sizeof(Buff_USART1_Rx), 0xFFFF) == HAL_OK)
  {
    AAA_LOG_DEBUG("Get USART1_Rx.");
    HAL_UART_Transmit(&huart1, Buff_USART1_Rx, sizeof(Buff_USART1_Rx), HAL_MAX_DELAY);
  }

  #elif 0 /* Log message and do commmand, end with charactor or buff-overflow. */
  if (HAL_UART_Receive(&huart1, &Char_USART1_Rx, 1, 0) == HAL_OK)
  {
    /* Get max len = sizeof(buff), max inedx = sizeof(buff)-1. */
    if ((Flag_USART1_Rx == 0) && (Cntr_USART1_Rx <= sizeof(Buff_USART1_Rx)-1))
    {
      Buff_USART1_Rx[Cntr_USART1_Rx] = Char_USART1_Rx;
      Cntr_USART1_Rx ++;
      /* Get end-of-message character '.' or Buffer overflow. */
      if ((Char_USART1_Rx == '.') || (Cntr_USART1_Rx == sizeof(Buff_USART1_Rx)))
      {
        /* Replace final character to '\0'. */
        Buff_USART1_Rx[Cntr_USART1_Rx-1] = '\0';
        Flag_USART1_Rx = 1;
      }
    }
  }
  AAA_USART1_Demo_Process(50);

  #else /* Log message and do commmand, end with charactor or buff-overflow or idle event. */
  uint16_t RxLen = 0;
  if (HAL_UARTEx_ReceiveToIdle(&huart1, Buff_USART1_Rx, sizeof(Buff_USART1_Rx), &RxLen, HAL_MAX_DELAY) == HAL_OK)
  {
    /* Get end-of-message character '.' */
    if (Buff_USART1_Rx[RxLen-1] == '.')
    {
      Buff_USART1_Rx[RxLen-1] = '\0';
    }
    Flag_USART1_Rx = 1;
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
    AAA_LOG_DEBUG("Get USART1_Rx: [%s]\n",Buff_USART1_Rx);
    /* Deal with data as commmand. */
    AAA_USART1_Ctrl_CMD(Buff_USART1_Rx);
    /* Clear params. */
    Flag_USART1_Rx = 0;
    Char_USART1_Rx = 0;
    Cntr_USART1_Rx = 0;
    memset(Buff_USART1_Rx, 0, sizeof(Buff_USART1_Rx));
  }
}

#endif  /* Disable  USART1 intertupt. */

/* Enable  USART1 intertupt. */
#ifdef USART1_INTERRUPT_ENABLE 

uint8_t Flag_USART1_Rx_IT = 0;
uint8_t Char_USART1_Rx_IT = 0;
uint8_t Cntr_USART1_Rx_IT = 0;
uint8_t Buff_USART1_Tx_IT[65] = "Data in Buff_USART1_Tx_IT[65].";
uint8_t Buff_USART1_Rx_IT[65] = "Data in Buff_USART1_Rx_IT[65].";
const char *info = "\n\
Mode: USART1 interrput enable.\n\
Help:\n\
1. Get Buff_USART1_Rx_IT[65] end with '.' or max len 64 or IDLE;\n\
2. Get with interrupt, but print still in blocking mode.\n\
\n";

void AAA_USART1_Demo_Main(void)
{
  AAA_LOG_INFO(info);
  AAA_LOG_DEBUG("Blocking Print USART1_Tx_IT: [%s]\n", Buff_USART1_Tx_IT);
  AAA_LOG_INFO("Getting USART1_Rx_IT...\n");
  #if 0 /* HAL_UART_Transmit_IT */
  HAL_UART_Transmit_IT(&huart1, Buff_USART1_Tx_IT, sizeof(Buff_USART1_Tx_IT));
  HAL_UART_Transmit_IT(&huart1, (uint8_t *)"\n", 1);
  #endif
  /* Clear Buff_USART1_Rx_IT and Start Receive_IT. */
  memset(Buff_USART1_Rx_IT, 0, sizeof(Buff_USART1_Rx_IT));
  HAL_UART_Receive_IT(&huart1, &Char_USART1_Rx_IT, 1);
}

void AAA_USART1_Demo_Loop(void)
{
  AAA_USART1_Demo_Process(50);
}

void AAA_USART1_Demo_Process(uint32_t tick_interval)
{
  static uint32_t tick_counter = 0;
  if ((HAL_GetTick() - tick_counter) < tick_interval) return;
  tick_counter = HAL_GetTick();

  if (Flag_USART1_Rx_IT == 1)
  {
    /* Print log. */
    AAA_LOG_DEBUG("Get USART1_Rx: %s\n",Buff_USART1_Rx_IT);
    /* Deal with data as commmand. */
    AAA_USART1_Ctrl_CMD(Buff_USART1_Rx_IT);
    /* Clear params. */
    Flag_USART1_Rx_IT = 0;
    Char_USART1_Rx_IT = 0;
    Cntr_USART1_Rx_IT = 0;
    memset(Buff_USART1_Rx_IT, 0, sizeof(Buff_USART1_Rx_IT));
  }
}



/**
  * @brief  Tx Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  * @note copy from and replace __weak function in driver file: stm32f1xx_hal_uart.c
  * @note USARTx_IRQHandler -> HAL_UART_IRQHandler -> UART_EndTransmit_IT -> HAL_UART_TxCpltCallback
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Do nothing. */
}

/**
  * @brief  Rx Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  * @note copy from and replace __weak function in driver file: stm32f1xx_hal_uart.c
  * @note USARTx_IRQHandler -> HAL_UART_IRQHandler -> UART_Receive_IT -> HAL_UART_RxCpltCallback
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    /* Get max len = sizeof(buff), max index = sizeof(buff)-1. */
    if (Flag_USART1_Rx_IT == 0) // && (Counter_USART1_Rx_IT <= sizeof(Buff_USART1_Rx_IT)-1))
    {
      Buff_USART1_Rx_IT[Cntr_USART1_Rx_IT++] = Char_USART1_Rx_IT;
      /* Get end-of-message character '.' or Buffer overflow. */
      if ((Char_USART1_Rx_IT == '.') || (Cntr_USART1_Rx_IT == sizeof(Buff_USART1_Rx_IT)))
      {
        /* Replace final character to '\0'. */
        Buff_USART1_Rx_IT[Cntr_USART1_Rx_IT-1] = '\0';
        Flag_USART1_Rx_IT = 1;
      }
    }
    else /* Flag_USART1_Rx_IT == 1 */
    {
      /* Do nothing to exclude redundant received data. */
    }

    // /* Idle line detection interrupt. */
    // if (__HAL_UART_GET_IT_SOURCE(&huart1, UART_IT_IDLE) != RESET)
    // {
    //   Flag_USART1_Rx_IT = 1;
    //   __HAL_UART_CLEAR_IDLEFLAG(&huart1);
    // }

    /* Continue Receive_IT. */
    HAL_UART_Receive_IT(&huart1, &Char_USART1_Rx_IT, 1);
  }
  if (huart->Instance ==USART3)
  {
    /* Do nothing. */
  }
}


#endif  /* Enable  USART1 intertupt. */



/* USER CODE END 1 */