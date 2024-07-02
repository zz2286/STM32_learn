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

#include "usart.h"

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

// #define AAA_LED_ON()      {HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);}
// #define AAA_LED_OFF()     {HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);}
// #define AAA_LED_TOGGLE()  {HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);}

// /* PA4 - SPI_SCN */
// #define SPI_SCN_SET() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET)
// #define SPI_SCN_CLR() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET)
// /* PA5 - SPI_SCK */
// #define SPI_SCK_SET() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET)
// #define SPI_SCK_CLR() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET)
// /* PA7 - SPI_SDA */
// #define SPI_SDA_SET() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET)
// #define SPI_SDA_CLR() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET)
// #define SPI_SDA_READ() HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)

#define REG_37 0x1D00



/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */

/* ============================== GPIO - LED ============================== */
void AAA_LED_ON(void);
void AAA_LED_OFF(void);
void AAA_LED_TOGGLE(void);
void AAA_LED_Blink_Block(uint8_t times, uint16_t ms);

/* ============================== GPIO - Key ============================== */
void AAA_Key_Scan_Loop(void);
static void AAA_Key_Do_PA0(void);
static void AAA_Key_Do_PA1(void);
static void AAA_Key_Do_PA2(void);
static void AAA_Key_Do_PA3(void);

/* ============================== GPIO - SPI ============================== */
void AAA_SPI_SDA_INPUT(void);
void AAA_SPI_SDA_OUTPUT(void);
void AAA_Delay_us(uint16_t us);
/* GPIO simulate SPI interface. */
void SPI_SCN_SET(void);
void SPI_SCN_CLR(void);
void SPI_SCK_SET(void);
void SPI_SCK_CLR(void);
void SPI_SDA_SET(void);
void SPI_SDA_CLR(void);
GPIO_PinState SPI_SDA_READ(void);

void AAA_SPI_Send_Byte(uint8_t byte);
uint16_t AAA_SPI_Get_Word(void);
void AAA_RF_Write(uint8_t addr, uint16_t data);
uint16_t AAA_RF_Read(uint8_t addr);
void AAA_RF_SetGpioOut(uint8_t num, uint8_t type, uint8_t val);
uint8_t AAA_RF_GetGpioIn(uint8_t num);

void AAA_RF_SPI_Main(void);
void AAA_RF_SPI_Loop(void);
void AAA_RF_SPI_Process(uint32_t tick_interval);



/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

