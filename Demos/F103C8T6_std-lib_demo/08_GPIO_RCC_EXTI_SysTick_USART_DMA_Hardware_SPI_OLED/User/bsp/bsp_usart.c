#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_systick.h"


// 接收字符串相关变量
volatile char RxBuffer[RX_BUFFER_SIZE]; // 接收缓冲区
volatile uint8_t RxIndex = 0;           // 接收字符序数
volatile uint8_t CmdReceived = 0;       // 接收到结束符 "." 或 字符数溢出缓冲区
volatile uint8_t CmdReset = 0;       // 重新开始接收字符串


 /**
  * @brief  配置嵌套向量中断控制器NVIC
  */
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 嵌套向量中断控制器组选择 */
    /* 提示 NVIC_PriorityGroupConfig() 在整个工程只需要调用一次来配置优先级分组*/
    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* 配置USART为中断源 */
    NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  USART GPIO 配置,工作参数配置
  */
void USART_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    // 打开串口GPIO的时钟
    DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);

    // 打开串口外设的时钟
    DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

    // 将USART Tx的GPIO配置为推挽复用模式
    GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

    // 将USART Rx的GPIO配置为浮空输入模式
    GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);

    // 配置串口的工作参数
    USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(DEBUG_USARTx, &USART_InitStructure);

    // 串口中断优先级配置
    NVIC_Configuration();

    // 使能串口接收中断
    // USART_ClearITPendingBit(DEBUG_USARTx, USART_IT_RXNE);
    // USART_ClearITPendingBit(DEBUG_USARTx, USART_IT_IDLE);
    // 这里使用内部软件清零, ClearITPendingBit没有参数IDLE。
    USART_ReceiveData(DEBUG_USARTx);
    // USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE | USART_IT_IDLE, ENABLE);
    // 这里一定要分开写, 合在一起会失效。
    USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);
    USART_ITConfig(DEBUG_USARTx, USART_IT_IDLE, ENABLE);

    // 使能串口
    USART_Cmd(DEBUG_USARTx, ENABLE);
}

/*****************  发送一个字节 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
    /* 发送一个字节数据到USART */
    USART_SendData(pUSARTx,ch);
    /* 等待发送数据寄存器为空 */
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/****************** 发送8位的数组 ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
    uint8_t i;

    for(i=0; i<num; i++)
    {
        /* 发送一个字节数据到USART */
        Usart_SendByte(pUSARTx,array[i]);	
    }
    /* 等待发送完成 */
    while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
    unsigned int k=0;
    do 
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(*(str + k)!='\0');

    /* 等待发送完成 */
    while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
    {}
}

/*****************  发送一个16位数 **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
    uint8_t temp_h, temp_l;

    /* 取出高八位 */
    temp_h = (ch&0XFF00)>>8;
    /* 取出低八位 */
    temp_l = ch&0XFF;

    /* 发送高八位 */
    USART_SendData(pUSARTx,temp_h);	
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);

    /* 发送低八位 */
    USART_SendData(pUSARTx,temp_l);	
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
    /* 发送一个字节数据到串口 */
    USART_SendData(DEBUG_USARTx, (uint8_t) ch);
    
    /* 等待发送完毕 */
    while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		

    return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
    /* 等待串口输入数据 */
    while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

    return (int)USART_ReceiveData(DEBUG_USARTx);
}

void USART_Process_Loop(void)
{
    // 指令接收完成
    if(CmdReceived==1)
    {
        // 处理指令，这里简单打印指令内容，你可以根据实际需求执行相应的操作
        printf("USART_CMD_Received: [%s].\n", RxBuffer);

        if(strncmp((char*)RxBuffer, "CMD+LED+", strlen("CMD+LED+")) == 0)
        {
            // 利用指针的算术运算，将 command 指针指向了 RxBuffer 后面的位置，即去掉了前缀的部分。
            const char *command = (char*)RxBuffer + strlen("CMD+LED+");
            if(strncmp(command, "ON",2) == 0)
            {
                printf("Get CMD+LED+ON and set led on.\n");
                LED_ON();
                SysTick_Timer_Stop(TIMER_ID_LED_BLINK);
            }
            else if(strncmp(command, "OFF",3) == 0)
            {
                printf("Get CMD+LED+OFF and set led off.\n");
                LED_OFF();
                SysTick_Timer_Stop(TIMER_ID_LED_BLINK);
            }
            else if(strncmp(command, "TOGGLE",6) == 0)
            {
                printf("Get CMD+LED+TOGGLE and set led toggle.\n");
                LED_TOGGLE();
                SysTick_Timer_Stop(TIMER_ID_LED_BLINK);
            }
            else if(strncmp(command, "BLINK",5) == 0)
            {
                uint32_t blink_value = 0;
                sscanf(command, "BLINK=%d", &blink_value);
                printf("Get CMD+LED+BLINK and set led blink in %d ms.\n",blink_value);
                SysTick_Timer_Start(TIMER_ID_LED_BLINK,-1,blink_value);
            }
        }
        else
        {
            printf("Failed to get valid CMD+LED+<ON/OFF/TOGGLE/BLINK=ms*> .\n");
        }
        // 处理结束后，再次进入串口接收中断时，先清除标志和缓冲区
        CmdReset = 1;
        CmdReceived = 0;
    }// endif CmdReceived==1
}
