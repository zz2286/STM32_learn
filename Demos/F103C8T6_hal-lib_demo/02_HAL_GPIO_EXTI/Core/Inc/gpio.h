/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
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
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

// #define AAA_LED_ON()      {HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);}
// #define AAA_LED_OFF()     {HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);}
// #define AAA_LED_TOGGLE()  {HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);}

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */

void AAA_LED_ON(void);
void AAA_LED_OFF(void);
void AAA_LED_TOGGLE(void);
void AAA_LED_Blink_Block(uint8_t times, uint16_t ms);

void AAA_Key_Scan_Loop(void);
static void AAA_Key_Do_PA2(void);
static void AAA_Key_Do_PA3(void);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

