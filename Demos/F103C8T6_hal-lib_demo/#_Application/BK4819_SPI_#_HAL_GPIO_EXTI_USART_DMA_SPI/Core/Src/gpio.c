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

uint8_t flag_RF_SPI_Loop = 0;

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
  printf("Key PA0: Enable RF_SPI_Loop.\n");
  flag_RF_SPI_Loop = 1;
}

static void AAA_Key_Do_PA1(void)
{
  /* Do nothing. */
  printf("Key PA1: Disable RF_SPI_Loop.\n");
  flag_RF_SPI_Loop = 0;
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

/* ============================== GPIO - SPI - begin ============================== */
/* PA7 - SPI_SDA */
void AAA_SPI_SDA_INPUT(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void AAA_SPI_SDA_OUTPUT(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
}

void AAA_Delay_us(uint16_t us)
{
  uint32_t cnt = us * 4200;
  while (cnt--)
  {
    __NOP();
  }
  
}

/* PA4 - SPI_SCN */
void SPI_SCN_SET(void) {HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);AAA_Delay_us(2);}
void SPI_SCN_CLR(void) {HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);AAA_Delay_us(2);}
/* PA5 - SPI_SCK */
void SPI_SCK_SET(void) {HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);AAA_Delay_us(2);}
void SPI_SCK_CLR(void) {HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);AAA_Delay_us(2);}
/* PA7 - SPI_SDA */
void SPI_SDA_SET(void) {HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);AAA_Delay_us(2);}
void SPI_SDA_CLR(void) {HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET);AAA_Delay_us(2);}
GPIO_PinState SPI_SDA_READ(void) {return HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7);}


void AAA_SPI_Send_Byte(uint8_t byte)
{
  uint8_t temp = byte;

  for (uint8_t i=0;i<8;i++)
  {
    SPI_SCK_CLR();

    if (temp & 0x80)
    {
      SPI_SDA_SET();
    }
    else
    {
      SPI_SDA_CLR();
    }

    SPI_SCK_SET();

    temp <<= 1;
  }
}

uint16_t AAA_SPI_Get_Word(void)
{
  uint16_t data = 0;

  AAA_SPI_SDA_INPUT();
  SPI_SCK_CLR();

  for (uint8_t i=0;i<16;i++)
  {
    data <<= 1;

    SPI_SCK_SET();
    
    if (SPI_SDA_READ())
    {
      data |= 1;
    }

    SPI_SCK_CLR();
  }

  return data;
}

void AAA_RF_Write(uint8_t addr, uint16_t data)
{
  uint16_t temp = data >> 8;

  AAA_SPI_SDA_OUTPUT();
  SPI_SCK_CLR();
  SPI_SCN_CLR();

  AAA_SPI_Send_Byte(addr);
  AAA_SPI_Send_Byte(temp&0xff);
  AAA_SPI_Send_Byte(data&0xff);

  SPI_SCN_SET();
}

uint16_t AAA_RF_Read(uint8_t addr)
{
  uint16_t data = 0;

  AAA_SPI_SDA_OUTPUT();
  SPI_SCK_CLR();
  SPI_SCN_CLR();

  AAA_SPI_Send_Byte(addr | 0x80);

  AAA_Delay_us(2);

  data = AAA_SPI_Get_Word();

  SPI_SCN_SET();

  return data;
}

void AAA_RF_SetGpioOut(uint8_t num, uint8_t type, uint8_t val)
/** GPIOx Output Type Selection: 
 * type=0 -> High/Low
 * type=1 -> Interrupt
 * type=2 -> Squelch
 * type=3 -> VoX
 * type=4 -> Subau Result
 * type=5 -> CTCSS Result
 * type=6 -> CDCSS Result
 * type=7 -> Tail
 * type=8 -> DTMF/5Tone
 * type=9 -> CTCSS/CDCSS Wave
 */
// Set GPIO6 = High:             RF_SetGpioOut(6,0,1);
// Set GPIO2 = CTCSS/CDCSS Wave: RF_SetGpioOut(2,9,0);
// Set GPIO5 = Squelch:          RF_SetGpioOut(5,2,0);
{
  uint16_t rdata;

  /* Enable GPIOx Output. */
  rdata = AAA_RF_Read(0x33);
  rdata &= ~(0x4040>>num);  /* GPIOx Output Enable & Value Low. */
  rdata |= (val<<6)>>num;   /* GPIOx Output Value High/Low. */
  AAA_RF_Write(0x33,rdata);

  /* GPIOx Output Type Selection. */
  switch(num)
  {
 		case 0: 
 			rdata = AAA_RF_Read(0x35);
			rdata &= 0xf0ff;  /* REG_35<11:8> GPIO0. */
			rdata |= type<<8;
			AAA_RF_Write(0x35,rdata);
 			break;
			
		case 1: 
			rdata = AAA_RF_Read(0x35);
			rdata &= 0xff0f;  /* REG_35<7:4> GPIO1. */
			rdata |= type<<4;
			AAA_RF_Write(0x35,rdata);
			break;

		case 2: 
			rdata = AAA_RF_Read(0x35);
			rdata &= 0xfff0;
			rdata |= type;
			AAA_RF_Write(0x35,rdata);
			break;

		case 3: 
 			rdata = AAA_RF_Read(0x34);
			rdata &= 0x0fff;
			rdata |= type<<12;
			AAA_RF_Write(0x34,rdata);
			break;

		case 4: 
			rdata = AAA_RF_Read(0x34);
			rdata &= 0xf0ff;
			rdata |= type<<8;
			AAA_RF_Write(0x34,rdata);
			break;
			
		case 5: 
			rdata = AAA_RF_Read(0x34);
			rdata &= 0xff0f;
			rdata |= type<<4;
			AAA_RF_Write(0x34,rdata);
			break;
			
		case 6: 
			rdata = AAA_RF_Read(0x34);
			rdata &= 0xfff0;
			rdata |= type;
			AAA_RF_Write(0x34,rdata);
			break;
  }
}

/**
 * @param num int range 0 ~ 6.
 */
uint8_t AAA_RF_GetGpioIn(uint8_t num)
{
  uint16_t rdata;

  rdata = AAA_RF_Read(0x33);
  /* Disable GPIOx Output. */
  rdata |= (0x4000>>num);
  AAA_RF_Write(0x33,rdata);
  
  /* Read GPIO Input Indicator, REG_0A read only. */
  rdata = AAA_RF_Read(0x0A);

  if (rdata & (0x40>>num))
    return 1;
  else
    return 0;
}


void AAA_RF_SPI_Main(void)
{
  /* Soft Reset. REG_00<15>: 1=Reset, 0=Normal. */
  AAA_RF_Write(0x00, 0x8000);
  AAA_RF_Write(0x00, 0x0000);

  /* Power Up. REG_37<7:0>: ANA/VCO/RF/PLL LDO Bypass, DSP/XTAL/Band-Gap Enable. */
  AAA_RF_Write(0x37, REG_37 | 0x0F);

  #if 0
  // Set AGC Table
  RF_SetAgc(0);

  // Set MIC Sensitivity
  RF_Write(0x7D,MIC_GAIN | REG_7D); //bit[4:0]=MicSens灵敏�???

  // Set Volume 
  RF_Write(0x48,REG_48 | VOL_GAIN<<4 | DAC_GAIN); //bit[9:4]=音量数字增益；bit[3:0]音量模拟增益

  // Others Setting
  RF_Write(0x1F,0x5454); 	// 2020.9.5 set rfpll_regvco_vbit=0101
  RF_Write(0x1F,0x1454);	// 2020.8.24 RF_Write(0x1F,0x1858);

  RF_Write(0x25,0xC1BA);
  RF_Write(0x77,0x88EF);

  RF_Write(0x3E,0xA037); //v3版本芯片必须修改！！�???

  //以下两组设置可以提高1M Block 1~2dB，但接收灵敏度会变差1~2dB
  RF_Write(0x13,0x037E);
  RF_Write(0x7D,MIC_GAIN | 0xE340);

  RF_SetTxAu(0);
  #endif

}

void AAA_RF_SPI_Loop(void)
{
  if (flag_RF_SPI_Loop)
  {
    AAA_RF_SPI_Process(2000);
  }
}

void AAA_RF_SPI_Process(uint32_t tick_interval)
{
  if (tick_interval != 0)
  {
    static uint32_t tick_counter = 0;
    if ((HAL_GetTick() - tick_counter) < tick_interval) return;
    tick_counter = HAL_GetTick();
  }

  if (AAA_RF_GetGpioIn(5))
  {
    printf("Get GPIO_5 High.\n");
    AAA_RF_SetGpioOut(6,0,1);
    printf("Set GPIO_6 High.\n");
  }
  else
  {
    printf("Get GPIO_5 Low.\n");
    AAA_RF_SetGpioOut(6,0,0);
    printf("Set GPIO_6 low.\n");
  }
}

/* ============================== GPIO - SPI - end ============================== */


/* USER CODE END 2 */
