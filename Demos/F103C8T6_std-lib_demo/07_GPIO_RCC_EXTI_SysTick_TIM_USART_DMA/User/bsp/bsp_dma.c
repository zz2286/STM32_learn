/**
 * @file    User\bsp\bsp_mda.c
 * @brief   本文件用于单独存放 DMA 直接存储器访问 的demo代码
 * @note    包括三个demo：存储器FLASH到存储器SRAM传输、存储器SRAM到外设USART传输、外设USART到存储器SRAM传输.
*/

#include "bsp_dma.h"
#include "bsp_led.h"
#include "bsp_usart.h"


#if(DMA_DIRECTION>=1)
//存储器FLASH到存储器SRAM传输

/* const 定义常量 Src_FLASH_Buffer 数组作为DMA传输数据源, 数据存储在内部的FLASH中 */
const uint32_t Src_FLASH_Buffer[DMA_BUFFER_SIZE_M2M]= {
    0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
    0x11121314,0x15161718,0x191A1B1C,0x1D1E1F20,
    0x21222324,0x25262728,0x292A2B2C,0x2D2E2F30,
    0x31323334,0x35363738,0x393A3B3C,0x3D3E3F40,
    0x41424344,0x45464748,0x494A4B4C,0x4D4E4F50,
    0x51525354,0x55565758,0x595A5B5C,0x5D5E5F60,
    0x61626364,0x65666768,0x696A6B6C,0x6D6E6F70,
    0x71727374,0x75767778,0x797A7B7C,0x7D7E7F80
};
/* 定义DMA传输目标存储器, 存储在内部的SRAM中 */
uint32_t Dst_SRAM_Buffer[DMA_BUFFER_SIZE_M2M];

// 存储器FLASH到存储器SRAM传输
void DMA_Demo_FLASH_2_SRAM(void)
{
    // 定义存放比较结果变量
    uint8_t TransferStatus;
    // DMA 配置并使能开始传输
    DMA_Config_FLASH_2_SRAM();
    // 等待DMA传输完成
    while(DMA_GetFlagStatus(DMA_FLAG_TC_M2M)==RESET)
    ;
    // 比较源数据与传输后数据
    TransferStatus = Buffer_Compare(Src_FLASH_Buffer, Dst_SRAM_Buffer, DMA_BUFFER_SIZE_M2M);
    if (TransferStatus==1)
    {
        printf("DMA_Demo_FLASH_2_SRAM: SUCCESS.\n");
        LED_ON();
    }
    else
    {
        printf("DMA_Demo_FLASH_2_SRAM: FAIL.\n");
        LED_OFF();
    }
}

/* DMA 配置并使能 存储器FLASH到存储器SRAM传输 */
void DMA_Config_FLASH_2_SRAM(void)
{
    RCC_AHBPeriphClockCmd(DMA_CLOCK_M2M, ENABLE);   // 开启DMA时钟

    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)Src_FLASH_Buffer;  // 外设地址，源数据（这里把内部FLASH当作是外设）
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Dst_SRAM_Buffer;       // 存储器地址，目标
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                      // 传输方向：外设到存储器
    DMA_InitStructure.DMA_BufferSize = DMA_BUFFER_SIZE_M2M;                 // 传输大小：32
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;         // 外设地址递增使能
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 // 内存地址递增使能
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; // 外设数据单位，字长
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;         // 内存数据单位，字长
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;      // DMA模式,一次或者循环模式,DMA_Mode_Normal/DMA_Mode_Circular.
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; // 优先级：高
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;         // 使能内存到内存的传输
    DMA_Init(DMA_CHANNEL_M2M, &DMA_InitStructure);          // 配置DMA通道

    DMA_ClearFlag(DMA_FLAG_TC_M2M);     // 清除DMA数据流传输完成标志位
    DMA_Cmd(DMA_CHANNEL_M2M,ENABLE);    // 使能DMA
}

/* 判断指定长度的两个数据源是否完全相等，如果完全相等返回1，只要其中一对数据不相等返回0 */
uint8_t Buffer_Compare(const uint32_t* pBuffer, uint32_t* pBuffer1, uint16_t BufferLength)
{
    /* 数据长度递减 */
    while(BufferLength--)
    {
    /* 判断两个数据源是否对应相等 */
    if(*pBuffer != *pBuffer1)
    {
        /* 对应数据源不相等马上退出函数，并返回0 */
        return 0;
    }
    /* 递增两个数据源的地址指针 */
    pBuffer++;
    pBuffer1++;
    }
    /* 完成判断并且对应数据相等 */
    return 1;
}

#endif /* DMA_DIRECTION >= 1 */



#if(DMA_DIRECTION>=2)
//存储器SRAM到外设USART传输
uint8_t Src_SRAM_Tx_Buffer[DMA_BUFFER_SIZE_USART1_TX];

/* DMA 配置并使能 存储器SRAM到外设USART传输 */
void DMA_Config_SRAM_2_USART(void)
{
    RCC_AHBPeriphClockCmd(DMA_CLOCK_USART1_TX, ENABLE);   // 开启DMA时钟

    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = DMA_PERIPH_ADDR_USART1_TX;   // 外设地址，目标，这里是 USART1_Tx
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Src_SRAM_Tx_Buffer;    // 存储器地址，源地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                      // 传输方向：存储器到外设
    DMA_InitStructure.DMA_BufferSize = DMA_BUFFER_SIZE_USART1_TX;           // 传输大小：宏定义
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        // 外设地址递增，不使能
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 // 内存地址递增使能
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 外设数据单位，字节
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;     // 内存数据单位，字节
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;           // DMA模式,一次或者循环模式,DMA_Mode_Normal/DMA_Mode_Circular.
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;   // 优先级：中
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;            // 禁止内存到内存的传输
    DMA_Init(DMA_CHANNEL_USART1_TX, &DMA_InitStructure);    // 配置DMA通道

    // DMA_ClearFlag(DMA_FLAG_TC_USART1_TX);     // 清除DMA数据流传输完成标志位
    DMA_Cmd(DMA_CHANNEL_USART1_TX,ENABLE);    // 使能DMA
}

void DMA_Demo_SRAM_2_USART(void)
{
    // DMA 配置并使能开始传输
    DMA_Config_SRAM_2_USART();
    printf("DMA_Demo_SRAM_2_USART: USART1 Strat Transmitting...\n");

    // 清除再填充 Src_SRAM_Tx_Buffer 缓冲区,最后一位添加回车符, #include <string.h>
    // memset(Src_SRAM_Tx_Buffer, 0, sizeof(Src_SRAM_Tx_Buffer));
    // char *myString = "[ This is the data filled in variable < Src_SRAM_Tx_Buffer >. Buffer max length: 300, end with '\\n'. ]";
    // memcpy(Src_SRAM_Tx_Buffer, myString, strlen(myString));
    // Src_SRAM_Tx_Buffer[sizeof(Src_SRAM_Tx_Buffer)-1] = '\n';

    // 使用 sprintf 将格式化的字符串填充到 Src_SRAM_Tx_Buffer 中, #include <stdio.h>
    sprintf((char *)Src_SRAM_Tx_Buffer,\
    "[ This is the data filled in variable < Src_SRAM_Tx_Buffer >. Buffer max length: %d, end with '\\n'. ]\n"\
    , sizeof(Src_SRAM_Tx_Buffer));

    // USART1 向 DMA发出TX请求
    USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Tx, ENABLE);
    // 这里让DMA保持USART发送，CPU去处理其他事情，所以不要阻塞while。
    // while(DMA_GetFlagStatus(DMA_FLAG_TC_USART1_TX)==RESET);
}

#endif /* DMA_DIRECTION >= 2 */



#if(DMA_DIRECTION>=3)
//外设USART到存储器SRAM传输
volatile uint8_t Dst_SRAM_Rx_Buffer[DMA_BUFFER_SIZE_USART1_RX];

void DMA_Config_USART_2_SRAM(void)
{
    RCC_AHBPeriphClockCmd(DMA_CLOCK_USART1_RX, ENABLE);   // 开启DMA时钟

    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = DMA_PERIPH_ADDR_USART1_RX;   // 外设地址，源地址，这里是 USART1_RX
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Dst_SRAM_Rx_Buffer;    // 存储器地址，目标
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                      // 传输方向：外设到存储器
    DMA_InitStructure.DMA_BufferSize = DMA_BUFFER_SIZE_USART1_RX;           // 传输大小：宏定义
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        // 外设地址递增，不使能
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 // 内存地址递增使能
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 外设数据单位，字节
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;     // 内存数据单位，字节
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;         // DMA模式,一次或者循环模式,DMA_Mode_Normal/DMA_Mode_Circular.
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;   // 优先级：中
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;            // 禁止内存到内存的传输
    DMA_Init(DMA_CHANNEL_USART1_RX, &DMA_InitStructure);    // 配置DMA通道

#if(DMA_IT_DISABLE_OR_ENABLE==1)
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = DMA_USART1_RX_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    DMA_ITConfig(DMA_CHANNEL_USART1_RX, DMA_IT_HT|DMA_IT_TC, ENABLE);
    DMA_ClearITPendingBit(DMA_IT_HT_USART_RX|DMA_IT_TC_USART_RX);
#endif /* DMA_IT_DISABLE_OR_ENABLE==1 */

    // DMA_ClearFlag(DMA_FLAG_TC_USART1_RX);     // 清除DMA数据流传输完成标志位
    DMA_Cmd(DMA_CHANNEL_USART1_RX,ENABLE);    // 使能DMA
}

void DMA_Demo_USART_2_SRAM(void)
{
    memset((void*)Dst_SRAM_Rx_Buffer,0,sizeof(Dst_SRAM_Rx_Buffer));

    // DMA 配置并使能开始传输
    DMA_Config_USART_2_SRAM();

    /* USART1 向 DMA发出RX请求 */
    USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Rx, ENABLE);
}

#if(DMA_IT_DISABLE_OR_ENABLE==0) // 不使用接收中断, 在DMA轮询中接收, 当DMA缓存 半满/溢满 时输出一次
    void DMA_Demo_USART_2_SRAM_Half_or_Full(void)
    {
    #define HALF_OR_FULL 0
        /**
         * 为什么需要使用 DMA 通道buf半满标志位？
         * 当DMA传输数据完成, 通道buf溢满置位，CPU介入开始拷贝DMA通道buf数据，如果此时串口继续有数据进来，DMA继续搬运数据到buf，就有可能将数据覆盖。
         * 改进的办法是"乒乓缓存”, 即 CPU 和 DMA 各自一块内存交替访问, 在 DMA 不支持双缓存的情况下, 可以借助 半满标志位 配合较大的buf空间实现类似效果。
         * 这里使用轮询，效果不如中断，如果通信数据较大或速率较快，仍存在溢出风险。
         * 如果使用了中断，在半满中断和溢满中断发生时，立刻让CPU响应读取对应一半buf的数据,效果会更好。
         * 
         * 通常情况下，DMA 传输是应用在 确定传输内容的大小 的情况下，所以 DMA 标志位的 "半满/溢满"，等同于传输任务的 "一半/完成"。
         * 但在接收不定长数据时，不管在 DMA 中使用哪种 标志位判定 或 中断判定，都只是在 半满/溢满 时CPU处理内存, 无法绝对避免溢出覆盖;
         * 可以在串口中断服务函数中, 判定 USART_IT_IDLE 串口空闲中断, 来判断串口接收数据的结束。
        */
    #if(HALF_OR_FULL==0)   // 判定 DMA 接收半满/溢满 标志位，设定较大的缓存区buf，实现双缓存效果，但轮询没有中断，如果通信数据较大或速率较快仍存在溢出风险。
        // 半满时，DMA 继续接收串口数据到 后半区，CPU 读取 前半区
        if(DMA_GetFlagStatus(DMA_FLAG_HT_USART1_RX)==SET)
        {
            DMA_ClearFlag(DMA_FLAG_HT_USART1_RX);
            for(int i = 0; i < DMA_BUFFER_SIZE_USART1_RX / 2; i++)
            {
                printf("%c", Dst_SRAM_Rx_Buffer[i]);
            }
        }
        // 溢满时，DMA 继续接收串口数据到 前半区，CPU 读取 后半区
        if(DMA_GetFlagStatus(DMA_FLAG_TC_USART1_RX)==SET)
        {
            DMA_ClearFlag(DMA_FLAG_TC_USART1_RX);
            for(int i = DMA_BUFFER_SIZE_USART1_RX / 2; i < DMA_BUFFER_SIZE_USART1_RX; i++)
            {
                printf("%c", Dst_SRAM_Rx_Buffer[i]);
            }
        }

    #elif(HALF_OR_FULL==1)   // 判定 DMA 接收满溢 标志位 进行读取，存在溢出覆盖风险。
        // 只有当设定的目标SRAM的缓存变量满溢时才会判定成功，溢出字符从头开始覆盖。
        if(DMA_GetFlagStatus(DMA_FLAG_TC_USART1_RX)==SET)
        {
            DMA_ClearFlag(DMA_FLAG_TC_USART1_RX);
            printf("DMA_Demo_USART_2_SRAM: < Src_SRAM_Tx_Buffer > Full or Exceeded.\nUSART1 Received:\n");
            printf("%s\n",Dst_SRAM_Rx_Buffer);
            // 获取DMA当前剩余的数据量，表示距离下一次满溢还能接收这么多数据，一旦超出又重新开始倒数。
            printf("DMA_GetCurrDataCounter: %d left to Exceeded.\n",DMA_GetCurrDataCounter(DMA_CHANNEL_USART1_RX));
            // memset(Dst_SRAM_Rx_Buffer,0,sizeof(Dst_SRAM_Rx_Buffer));    // 没必要清空,溢满肯定覆盖。
            DMA_Cmd(DMA_CHANNEL_USART1_RX,DISABLE);
            DMA_SetCurrDataCounter(DMA_CHANNEL_USART1_RX, DMA_BUFFER_SIZE_USART1_RX);
            DMA_Cmd(DMA_CHANNEL_USART1_RX,ENABLE);
            printf("Do DMA_SetCurrDataCounter to Rest.\n");
            printf("DMA_GetCurrDataCounter: %d left to Exceeded.\n",DMA_GetCurrDataCounter(DMA_CHANNEL_USART1_RX));
        }
    #endif /* HALF_OR_FULL */
    }
#elif(DMA_IT_DISABLE_OR_ENABLE==1)// 使用DMA中断, 当DMA缓存 半满/溢满 时触发中断，及时将信息处理转移
    /* Code in DMA_USART1_RX_IRQHandler() */
    /**
     * 没有必要去 记忆上一次结束时的 DMA 剩余数据量，每一次串口空闲中断里将 DMA 剩余数据量 重置为全满即可
     * 这种策略在 不足半满的数据 高频触发 空闲中断 的情况下可能会有数据覆盖风险
    **/

#endif /* DMA_IT_DISABLE_OR_ENABLE */

#endif /* DMA_DIRECTION >= 3 */



void DMA_Demo(void)
{
    DMA_Demo_FLASH_2_SRAM();
    DMA_Demo_SRAM_2_USART();
    DMA_Demo_USART_2_SRAM();
}


void DMA_Process_loop(void)
{
    // DMA_Demo_SRAM_2_USART_Is_Done();
    if(DMA_GetFlagStatus(DMA_FLAG_TC_USART1_TX)==SET)
    {
        DMA_ClearFlag(DMA_FLAG_TC_USART1_TX);
        printf("DMA_Demo_SRAM_2_USART: USART1 Transfer Complete.\n");
        // 关闭 USART1_TX 的 DMA 通道 使能
        DMA_Cmd(DMA_CHANNEL_USART1_TX, DISABLE);
        
        #if(DMA_IT_DISABLE_OR_ENABLE==1)
        printf("DMA_Demo_USART_2_SRAM: USART1 Enable Receive...\n");
        #endif /* DMA_IT_DISABLE_OR_ENABLE */
    }

    #if(DMA_IT_DISABLE_OR_ENABLE==0)
        DMA_Demo_USART_2_SRAM_Half_or_Full();
    #elif(DMA_IT_DISABLE_OR_ENABLE==1)
        /* Code in DMA_USART1_RX_IRQHandler() */
    #endif /* DMA_IT_DISABLE_OR_ENABLE */

}