/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */

extern uint8_t flag_RF_Loop_Process;  // from bk4819.c

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA2 PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA5 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

}

/* USER CODE BEGIN 2 */

/* ============================== GPIO - LED - begin ============================== */

void AAA_LED_ON(void)       {HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);}
void AAA_LED_OFF(void)      {HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);}
void AAA_LED_TOGGLE(void)   {HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);}

/**
 * @brief LED blink block.
 * @param times LED turns on and then off once.
 * @param ms interval for blink.
*/
void AAA_LED_Blink_Block(uint8_t times, uint16_t ms)
{
  while (times--)
  {
    AAA_LED_ON();
    HAL_Delay(ms/2);
    AAA_LED_OFF();
    HAL_Delay(ms/2);
  }
}

/* ============================== GPIO - LED - end ============================== */


/* ============================== GPIO - Key - begin ============================== */

/**
 * @brief Key scan for main loop, block until release key.
 * @note LED_PC13 toggle if PA2 fall (key down), on when PA3 low (key preess).
*/
void AAA_Key_Scan_Loop(void)
{
  // Key - PA2, pull-up, falling edge active, key down to toggle LED.
  if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)==GPIO_PIN_RESET){HAL_Delay(10);
  if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)==GPIO_PIN_RESET)
  {
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    AAA_Key_Do_PA2();
    while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)==GPIO_PIN_RESET);
  }}
  // Key - PA3, pull-up, key press to turn on LED, release to off.
  if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3)==GPIO_PIN_RESET){HAL_Delay(10);
  if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3)==GPIO_PIN_RESET)
  {
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
    AAA_Key_Do_PA3();
    while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3)==GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
  }}
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  * @note copy from and replace __weak function in driver file: stm32f1xx_hal_gpio.c
  * @note EXTIx_IRQHandler -> HAL_GPIO_EXTI_IRQHandler(GPIO_Pin) -> HAL_GPIO_EXTI_Callback(GPIO_Pin)
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* NOTE: __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin) outside and before this function, 
           EXTI line interrupt request state has been cleared. */

  /* Called by EXTI0_IRQHandler, triggered by PA0 (PULLUP & IT_RISING). */
  if (GPIO_Pin == GPIO_PIN_0)
  {
    AAA_LED_ON();
    AAA_Key_Do_PA0();
  }
  /* Called by EXTI1_IRQHandler, triggered by PA1 (PULLUP & IT_FALLING). */
  else if (GPIO_Pin == GPIO_PIN_1)
  {
    AAA_LED_OFF();
    AAA_Key_Do_PA1();
  }
}


static void AAA_Key_Do_PA0(void)
{
  /* Do nothing. */
  printf("Key PA0: Enable RF_Loop_Process.\n");
  flag_RF_Loop_Process = 1;
}

static void AAA_Key_Do_PA1(void)
{
  /* Do nothing. */
  printf("Key PA1: Disable RF_Loop_Process.\n");
  flag_RF_Loop_Process = 0;
}

static void AAA_Key_Do_PA2(void)
{
  /* Do nothing. */
  static uint16_t data_w = 0xABCD;
  AAA_RF_Write(0x38,data_w);
  printf("Key PA2: AAA_RF_Write(0x38, 0x%x).\n",data_w++);
}

static void AAA_Key_Do_PA3(void)
{
  /* Do nothing. */
  uint16_t data_r = AAA_RF_Read(0x38);
  printf("Key PA3: AAA_RF_Read(0x38) = 0x%x.\n",data_r);
}


/* ============================== GPIO - Key - end ============================== */

/* USER CODE END 2 */
