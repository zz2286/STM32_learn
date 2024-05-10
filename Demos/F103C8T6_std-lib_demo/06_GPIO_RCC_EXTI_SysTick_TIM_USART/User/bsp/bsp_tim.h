#ifndef __BSP_TIM_H
#define __BSP_TIM_H

/**
 * Basic TIM6/TIM7 can only CountModeUp without IO.
 * General TIM2/3/4/5 can CountModeUp/Down, OutputCompare, InputCapture and PWM_Output with 4 IO.
 * Advance TIM1/8 can CountModeUp/Down, OutputCompare, InputCapture, PWM_Output and BDTR with 8 IO.
*/

#include "stm32f10x.h"

/* TIMx macro definition. */
#define BASIC_TIM6_TIM7     // Do not available in STM32F103C8T6.
// #define ADVANCE_TIM1_CNT    // Timer1 Counter mode.
#define ADVANCE_TIM1_OC     // Timer1 Compare Output mode.
// #define ADVANCE_TIM1_IC     // Timer1 Input Capture mode.
#define GENERAL_TIM2_IC    // detect key press time.
#define GENERAL_TIM3_OC    // output pwm.
#define GENERAL_TIM4_IC    // input pwm.


/* ============================== Advance Timer 1 ============================== */
#ifdef ADVANCE_TIM1_CNT
/* ADVANCE_TIM1_CNT */
#define ADVANCE_TIM1                   TIM1
#define ADVANCE_TIM1_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define ADVANCE_TIM1_CLK               RCC_APB2Periph_TIM1
#define ADVANCE_TIM1_Period            (1000-1)
#define ADVANCE_TIM1_Prescaler         (72-1)
#define ADVANCE_TIM1_IRQ               TIM1_UP_IRQn
#define ADVANCE_TIM1_IRQHandler        TIM1_UP_IRQHandler
void ADVANCE_TIM1_Init(void);
void ADVANCE_TIM1_Process_Loop(void);
#endif /* ADVANCE_TIM1_CNT */

#ifdef ADVANCE_TIM1_OC
/* ADVANCE_TIM1_OC */
#define ADVANCE_TIM1                 TIM1
#define ADVANCE_TIM1_APBxClock_FUN   RCC_APB2PeriphClockCmd
#define ADVANCE_TIM1_CLK             RCC_APB2Periph_TIM1
// #define ADVANCE_TIM1_IRQ             TIM1_UP_IRQn
// #define ADVANCE_TIM1_IRQHandler      TIM1_UP_IRQHandler
// PWM: Fre=72/8/9=1MHz, T=1us.
#define ADVANCE_TIM1_Period          (8-1)
#define ADVANCE_TIM1_Prescaler       (9-1)
#define ADVANCE_TIM1_Pulse           4
// TIM1_CH1 Compare Output Channel GPIO (PA8).
#define ADVANCE_TIM1_CH1_GPIO_CLK_CMD   RCC_APB2PeriphClockCmd
#define ADVANCE_TIM1_CH1_GPIO_CLK       RCC_APB2Periph_GPIOA
#define ADVANCE_TIM1_CH1_GPIO_PORT      GPIOA
#define ADVANCE_TIM1_CH1_GPIO_PIN       GPIO_Pin_8
// TIM1_CH1N Compare Output Complement Channel GPIO (PB13).
#define ADVANCE_TIM1_CH1N_GPIO_CLK_CMD  RCC_APB2PeriphClockCmd
#define ADVANCE_TIM1_CH1N_GPIO_CLK      RCC_APB2Periph_GPIOB
#define ADVANCE_TIM1_CH1N_GPIO_PORT     GPIOB
#define ADVANCE_TIM1_CH1N_GPIO_PIN      GPIO_Pin_13
// TIM1 Compare Output Channel BKIN (Break Input) GPIO (PB12).
#define ADVANCE_TIM1_BKIN_GPIO_CLK_CMD  RCC_APB2PeriphClockCmd
#define ADVANCE_TIM1_BKIN_GPIO_CLK      RCC_APB2Periph_GPIOB
#define ADVANCE_TIM1_BKIN_GPIO_PORT     GPIOB
#define ADVANCE_TIM1_BKIN_GPIO_PIN      GPIO_Pin_12

void ADVANCE_TIM1_Init(void);
void ADVANCE_TIM1_Process_Loop(void);   // Do Nothing.
#endif /* ADVANCE_TIM1_OC */



/* ============================== General Timer 2 ============================== */
#ifdef GENERAL_TIM2_IC
#define GENERAL_TIM2                   TIM2
#define GENERAL_TIM2_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define GENERAL_TIM2_CLK               RCC_APB1Periph_TIM2
#define GENERAL_TIM2_Period            (1000-1)
#define GENERAL_TIM2_Prescaler         (72-1)
// TIM2 Interrupt
#define GENERAL_TIM2_IRQ                 TIM2_IRQn
#define GENERAL_TIM2_IRQHandler          TIM2_IRQHandler
#define GENERAL_TIM2_CHx                 TIM_Channel_3
#define GENERAL_TIM2_IT_CCx              TIM_IT_CC3
#define GENERAL_TIM2_STRAT_ICPolarity    TIM_ICPolarity_Falling
#define GENERAL_TIM2_END_ICPolarity      TIM_ICPolarity_Rising
#define GENERAL_TIM2_OCxPolarityConfig_CMD   TIM_OC3PolarityConfig
#define GENERAL_TIM2_GetCapturex_CMD         TIM_GetCapture3
// TIM2_CH3 Input Capture Channel GPIO (PA2)
#define GENERAL_TIM2_CH3_GPIO_CLK_CMD   RCC_APB2PeriphClockCmd
#define GENERAL_TIM2_CH3_GPIO_CLK       RCC_APB2Periph_GPIOA
#define GENERAL_TIM2_CH3_GPIO_PORT      GPIOA
#define GENERAL_TIM2_CH3_GPIO_PIN       GPIO_Pin_2

typedef struct
{
    unsigned char IC_Start_Flag;
    unsigned char IC_End_Flag;
    unsigned short IC_CCR_Value;
    unsigned short IC_Period;
}TIM2_IC_Info_Struct_Typedef;

void GENERAL_TIM2_Init(void);
void GENERAL_TIM2_Process_Loop(void);
#endif /* GENERAL_TIM2_IC */



/* ============================== General Timer 3 ============================== */
#ifdef GENERAL_TIM3_OC
/* GENERAL_TIM3 */
#define GENERAL_TIM3                 TIM3
#define GENERAL_TIM3_APBxClock_FUN   RCC_APB1PeriphClockCmd
#define GENERAL_TIM3_CLK             RCC_APB1Periph_TIM3
// #define GENERAL_TIM3_IRQ             TIM3_IRQn
// #define GENERAL_TIM3_IRQHandler      TIM3_IRQHandler
// PWM: Fre=2KHz, DutyCycle=40%.
#define GENERAL_TIM3_Period          (500-1)
#define GENERAL_TIM3_Prescaler       (72-1)
#define GENERAL_TIM3_Pulse           200
// TIM3_CH1 Output Compare Channel GPIO (PA6).
#define GENERAL_TIM3_CH1_GPIO_CLK_CMD   RCC_APB2PeriphClockCmd
#define GENERAL_TIM3_CH1_GPIO_CLK       RCC_APB2Periph_GPIOA
#define GENERAL_TIM3_CH1_GPIO_PORT      GPIOA
#define GENERAL_TIM3_CH1_GPIO_PIN       GPIO_Pin_6

void GENERAL_TIM3_Init(void);
#endif /* GENERAL_TIM3_OC */




/* ============================== General Timer 4 ============================== */
#ifdef GENERAL_TIM4_IC
/* GENERAL_TIM4 */
#define GENERAL_TIM4                 TIM4
#define GENERAL_TIM4_APBxClock_FUN   RCC_APB1PeriphClockCmd
#define GENERAL_TIM4_CLK             RCC_APB1Periph_TIM4
// PWM Input Capture

// TIM_Fre_cnt = TIM_CLK / TIM_Psc = 1 MHz.
// TIM_Fre = TIM_Fre_cnt / TIM_Period = 72MHz/72/1000 = 1 KHz.

// Method-1: In one PWM_Period to count TIM_CCR:
//// PWM_Fre = TIM_Fre_cnt / TIM_CCR = (1M/CCR) Hz;
//// TIM_CCR(2-1000) => PWM_Fre(1KHz-500KHz). TIM_CCR(2-10).

// Method-2: Set a GateTime and count PWM_Num:
//// PWM_Fre = PWM_Num / GateTime. N.A.

#define GENERAL_TIM4_Period          (1000-1)   // TIM_Fre=72MHz/72/1000=1KHz.
#define GENERAL_TIM4_Prescaler       (72-1)     // TIM_Fre_cnt=72MHz/72=1MHz.

// TIM4 Interrupt
#define GENERAL_TIM4_IRQ             TIM4_IRQn
#define GENERAL_TIM4_IRQHandler      TIM4_IRQHandler
#define GENERAL_TIM4_CHx                 TIM_Channel_1
#define GENERAL_TIM4_IT_CCx              TIM_IT_CC1
#define GENERAL_TIM4_STRAT_ICPolarity    TIM_ICPolarity_Rising
#define GENERAL_TIM4_END_ICPolarity      TIM_ICPolarity_Falling
#define GENERAL_TIM4_GetCapturex_CMD         TIM_GetCapture1
// TIM3_CH1 Input Capture Channel GPIO (PA6).
#define GENERAL_TIM4_CH1_GPIO_CLK_CMD   RCC_APB2PeriphClockCmd
#define GENERAL_TIM4_CH1_GPIO_CLK       RCC_APB2Periph_GPIOB
#define GENERAL_TIM4_CH1_GPIO_PORT      GPIOB
#define GENERAL_TIM4_CH1_GPIO_PIN       GPIO_Pin_6

void GENERAL_TIM4_Init(void);

#endif /* GENERAL_TIM4_IC */

#endif /* __BSP_TIM_H */