#ifndef __BSP_SPI_H
#define __BSP_SPI_H

#include "stm32f10x.h"
#include <stdio.h>

#define SPI_GPIO_PORT       GPIOA
// OLED_CS, SPI_NSS, PA4
#define SPI_NSS_GPIO_PIN    GPIO_Pin_4
// OLED_D0_SCL, SPI_SCK, PA5
#define SPI_SCK_GPIO_PIN    GPIO_Pin_5
// OLED_D1_SDA, SPI_MOSI, PA7
#define SPI_MOSI_GPIO_PIN   GPIO_Pin_7


void SPI_Config(void);
void SPI_SendByte(uint8_t byte);


// #define SPI_DMA_ENABLE

#ifdef SPI_DMA_ENABLE

    #define DMA_CLOCK_SPI1_TX         RCC_AHBPeriph_DMA1  // DMA 通道时钟
    #define DMA_CHANNEL_SPI1_TX       DMA1_Channel3       // SPI1_Tx对应的DMA请求通道
    #define DMA_FLAG_TC_SPI1_TX       DMA1_FLAG_TC3       // DMA 通道 溢满 标志 transfer complete flag
    #define DMA_PERIPH_ADDR_SPI1_TX   (uint32_t)&SPI1->DR // 外设寄存器地址
    #define DMA_BUFFER_SIZE_SPI1_TX   1024                // 一次发送的数据量
    
    void SPI_DMA_Config(void);
    void SPI_DMA_Tx(void);

#endif /* ifdef SPI_DMA_ENABLE */
#endif /* __BSP_SPI_H */
