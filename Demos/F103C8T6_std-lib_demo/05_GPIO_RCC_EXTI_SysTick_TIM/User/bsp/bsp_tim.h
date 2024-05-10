#ifndef __BSP_TIM_H
#define __BSP_TIM_H

/**
 * Basic TIM6/TIM7 can only CountModeUp without IO.
 * General TIM2/3/4/5 can CountModeUp/Down, OutputCompare, InputCapture and PWM_Output with 4 IO.
 * Advance TIM1/8 can CountModeUp/Down, OutputCompare, InputCapture, PWM_Output and BDTR with 8 IO.
*/

#include "stm32f10x.h"

/* TIMx macro definition. */
#define BASIC_TIM6_TIM7 // Do not available in STM32F103C8T6.
#define ADVANCE_TIM1    // Only TIM1 available, TIM8 NA.
#define GENERAL_TIM2
#define GENERAL_TIM3
#define GENERAL_TIM4


/* Advance Timer 1 */
#ifdef ADVANCE_TIM1
#define ADVANCE_TIM                   TIM1
#define ADVANCE_TIM_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define ADVANCE_TIM_CLK               RCC_APB2Periph_TIM1
#define ADVANCE_TIM_Period            (1000-1)
#define ADVANCE_TIM_Prescaler         (72-1)
#define ADVANCE_TIM_IRQ               TIM1_UP_IRQn
#define ADVANCE_TIM_IRQHandler        TIM1_UP_IRQHandler
void ADVANCE_TIM_Init(void);
void ADVANCE_TIM_Process_Loop(void);
#endif /* ADVANCE_TIM1 */


/* General Timer 2 */
#ifdef GENERAL_TIM2
#define GENERAL_TIM                   TIM2
#define GENERAL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define GENERAL_TIM_CLK               RCC_APB1Periph_TIM2
#define GENERAL_TIM_Period            (1000-1)
#define GENERAL_TIM_Prescaler         (72-1)
#define GENERAL_TIM_IRQ               TIM2_IRQn
#define GENERAL_TIM_IRQHandler        TIM2_IRQHandler
void GENERAL_TIM_Init(void);
void GENERAL_TIM_Process_Loop(void);
#endif /* GENERAL_TIM2 */

#endif /* __BSP_TIM_H */