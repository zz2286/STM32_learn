#ifndef __BSP_USART_H
#define	__BSP_USART_H


#include "stm32f10x.h"
#include <stdio.h>  // printf 相关
#include <string.h> // memset 相关

/** 
 * 串口宏定义，不同的串口挂载的总线和IO不一样，移植时需要修改这几个宏
 * 1-修改总线时钟的宏，uart1挂载到apb2总线，其他uart挂载到apb1总线
 * 2-修改GPIO的宏
 */
// 串口1-USART1 - (TX:PA9, RX:PA10) - (重定向TX:PB6, RX:PB7)
#define  DEBUG_USARTx                   USART1
#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1
#define  DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  DEBUG_USART_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART_TX_GPIO_PORT       GPIOA
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9
#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10

#define  DEBUG_USART_IRQ                USART1_IRQn
#define  DEBUG_USART_IRQHandler         USART1_IRQHandler


// 串口2-USART2 - (TX:PA2, RX:PA3)
//#define  DEBUG_USARTx                   USART2
//#define  DEBUG_USART_CLK                RCC_APB1Periph_USART2
//#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
//#define  DEBUG_USART_BAUDRATE           115200
//// USART GPIO 引脚宏定义
//#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
//#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//#define  DEBUG_USART_TX_GPIO_PORT       GPIOA
//#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_2
//#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
//#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_3
//#define  DEBUG_USART_IRQ                USART2_IRQn
//#define  DEBUG_USART_IRQHandler         USART2_IRQHandler

// 串口3-USART3 - (TX:PB10, RX:PB11)
//#define  DEBUG_USARTx                   USART3
//#define  DEBUG_USART_CLK                RCC_APB1Periph_USART3
//#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
//#define  DEBUG_USART_BAUDRATE           115200
//// USART GPIO 引脚宏定义
//#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOB)
//#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//#define  DEBUG_USART_TX_GPIO_PORT       GPIOB
//#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_10
//#define  DEBUG_USART_RX_GPIO_PORT       GPIOB
//#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_11
//#define  DEBUG_USART_IRQ                USART3_IRQn
//#define  DEBUG_USART_IRQHandler         USART3_IRQHandler


// 定义接收缓冲区大小
#define RX_BUFFER_SIZE           50

void USART_Config(void);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);

void USART_Process_Loop(void);

#endif /* __BSP_USART_H */
