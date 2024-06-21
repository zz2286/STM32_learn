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

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

}

/* USER CODE BEGIN 2 */

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


static void AAA_Key_Do_PA2(void)
{
  /* Do nothing. */
}

static void AAA_Key_Do_PA3(void)
{
  /* Do nothing. */
}

/* USER CODE END 2 */
