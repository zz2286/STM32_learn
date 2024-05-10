#ifndef __BSP_DMA_H
#define __BSP_DMA_H


#include "stm32f10x.h"
#include <string.h>
#include <stdio.h>

/* DMA 传输方向定义: 1_FLASH->SRAM, 2_SRAM->USART, 3_USART->SRAM */
#define DMA_DIRECTION 3
/* DMA 传输中断使能: 0_DISABLE, 1_ENABLE */
#define DMA_IT_DISABLE_OR_ENABLE 1

#if(DMA_DIRECTION>=1)
//存储器FLASH到存储器SRAM传输

    #define DMA_CLOCK_M2M       RCC_AHBPeriph_DMA1  // DMA 通道时钟
    #define DMA_CHANNEL_M2M     DMA1_Channel6       // 当使用存储器到存储器模式时候，通道可以随便选，没有硬性的规定
    #define DMA_FLAG_TC_M2M     DMA1_FLAG_TC6       // 传输完成标志
    #define DMA_BUFFER_SIZE_M2M 32                  // 要发送的数据大小

    void DMA_Demo_FLASH_2_SRAM(void);
    void DMA_Config_FLASH_2_SRAM(void);
    uint8_t Buffer_Compare(const uint32_t* pBuffer, uint32_t* pBuffer1, uint16_t BufferLength);

#endif /* DMA_DIRECTION >= 1 */



#if(DMA_DIRECTION>=2)
//存储器SRAM到外设USART传输

    #define DMA_CLOCK_USART1_TX         RCC_AHBPeriph_DMA1  // DMA 通道时钟
    #define DMA_CHANNEL_USART1_TX       DMA1_Channel4       // 串口发送对应的DMA请求通道
    #define DMA_FLAG_TC_USART1_TX       DMA1_FLAG_TC4       // 传输完成标志 DMA1 Channel4 transfer complete flag.
    #define DMA_PERIPH_ADDR_USART1_TX   (USART1_BASE+0x04)  // 外设寄存器地址
    #define DMA_BUFFER_SIZE_USART1_TX   500                // 一次发送的数据量
    
    void DMA_Config_SRAM_2_USART(void);
    void DMA_Demo_SRAM_2_USART(void);

#endif /* DMA_DIRECTION >= 2 */



#if(DMA_DIRECTION>=3)
//外设USART到存储器SRAM传输
    #define DMA_CLOCK_USART1_RX         RCC_AHBPeriph_DMA1  // DMA 通道时钟
    #define DMA_CHANNEL_USART1_RX       DMA1_Channel5       // 串口发送对应的DMA请求通道
    #define DMA_FLAG_TC_USART1_RX       DMA1_FLAG_TC5       // DMA 通道 溢满 标志 transfer complete flag
    #define DMA_FLAG_HT_USART1_RX       DMA1_FLAG_HT5       // DMA 通道 半满 标志 half transfer flag
    #define DMA_PERIPH_ADDR_USART1_RX   (USART1_BASE+0x04)  // 外设寄存器地址
    #define DMA_BUFFER_SIZE_USART1_RX   500                // 一次发送的数据量
    
    void DMA_Config_USART_2_SRAM(void);
    void DMA_Demo_USART_2_SRAM(void);

    #if(DMA_IT_DISABLE_OR_ENABLE==0)// DMA 传输不使用 DMA_IT_ 中断，仅在轮询中查询 DMA_FLAG_ 标志位，存在缓冲区数据溢出覆盖的问题

        void DMA_Demo_USART_2_SRAM_Half_or_Full(void);

    #elif(DMA_IT_DISABLE_OR_ENABLE==1)// DMA 传输使用 DMA_IT_HT/TC 和 USART_IT_IDLE 中断，在缓冲区数据半满和溢满以及接收结束时，及时调度CPU响应读取对应数据，避免数据溢出覆盖或滞留。
        
        #define DMA_IT_HT_USART_RX          DMA1_IT_HT5
        #define DMA_IT_TC_USART_RX          DMA1_IT_TC5
        #define DMA_USART1_RX_IRQ           DMA1_Channel5_IRQn
        #define DMA_USART1_RX_IRQHandler    DMA1_Channel5_IRQHandler

    #endif /* DMA_IT_DISABLE_OR_ENABLE */

#endif /* DMA_DIRECTION >= 3 */



void DMA_Demo(void);
void DMA_Process_loop(void);



#endif  /* __BSP_DMA_H */
