#ifndef __BSP_RCC_MCO_H
#define	__BSP_RCC_MCO_H


#include "stm32f10x.h"

void HSE_SetSysClock(uint32_t pllmul);
void HSI_SetSysClock(uint32_t pllmul);

void MCO_GPIO_Config(void);
void RCC_MCO_Init(void);

#endif /* __BSP_RCC_MCO_H */
