#include "bsp_spi.h"


void SPI_Config(void)
{
    // Enable SPI & GPIO Clock in APB2.
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA, ENABLE);

    // SPI_GPIO_Init for SCK & MOSI.
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = SPI_SCK_GPIO_PIN | SPI_MOSI_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_GPIO_PORT, &GPIO_InitStructure);

    // SPI_Init.
    SPI_InitTypeDef SPI_InitStructure;
    SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    // SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);

    // SPI_GPIO_Init for NSS.
    // Not necessary if is SPI_NSS_Soft.
    if (SPI_InitStructure.SPI_NSS == SPI_NSS_Soft)
    {
        GPIO_InitStructure.GPIO_Pin = SPI_NSS_GPIO_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(SPI_GPIO_PORT, &GPIO_InitStructure);
        // Default ResetBit NSS_Pin, to keep OLED_CS low as Enable.
        // Or can connect OLED_CS to GND in the circuit.
    }
    // Necessary if is SPI_NSS_Hard.
    else if (SPI_InitStructure.SPI_NSS == SPI_NSS_Hard)
    {
        GPIO_InitStructure.GPIO_Pin = SPI_NSS_GPIO_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(SPI_GPIO_PORT, &GPIO_InitStructure);
        // Enable SPI_NSS output for master mode.
        SPI_SSOutputCmd(SPI1,ENABLE);
        // OLED_CS can pull-up in the circuit when SPI shutdown.
    }
    // Enable SPI.
    SPI_Cmd(SPI1, ENABLE);
}


void SPI_SendByte(uint8_t byte)
{
    // Wait for SPI Tx buffer empty.
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    // Send date.
    SPI_I2S_SendData(SPI1, byte);
    // Wait for SPI transmission completion.
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
}


#ifdef SPI_DMA_ENABLE

extern u8 OLED_GRAM[128][8];

void SPI_DMA_Config(void)
{
    RCC_AHBPeriphClockCmd(DMA_CLOCK_SPI1_TX, ENABLE);   // 开启DMA时钟

    DMA_InitTypeDef DMA_InitStructure;
    // DMA_InitStructure.DMA_PeripheralBaseAddr = DMA_PERIPH_ADDR_SPI1_TX; // 外设地址
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR; // 外设地址：(SPI1_BASE+0x04*3)=(uint32_t)&SPI1->DR.
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&OLED_GRAM;        // 存储器地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                  // 传输方向：存储器到外设
    DMA_InitStructure.DMA_BufferSize = DMA_BUFFER_SIZE_SPI1_TX;         // 传输大小: sizeof(OLED_GRAM)=128*8=1024.
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;    // 外设地址递增，不使能
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;             // 内存地址递增使能
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 外设数据单位，字节
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;     // 内存数据单位，字节
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;           // DMA模式,一次或者循环模式,DMA_Mode_Normal/DMA_Mode_Circular.
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;     // 优先级
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;            // 禁止内存到内存的传输
    DMA_Init(DMA_CHANNEL_SPI1_TX, &DMA_InitStructure);      // 配置DMA通道

    /* Enable DMA channels */
    DMA_Cmd(DMA_CHANNEL_SPI1_TX, ENABLE);
    /* Enable SPI1 DMA Tx request */
    // SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
}

void SPI_DMA_Tx(void)
{
    printf("SPI_DMA_Tx: into.\n");
    
    DMA_ClearFlag(DMA_FLAG_TC_SPI1_TX);
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);


    // while(DMA_GetFlagStatus(DMA_FLAG_TC_SPI1_TX)==RESET);
    
    printf("SPI_DMA_Tx: exit.\n");
}
#endif