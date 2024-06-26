# 03_HAL_GPIO_EXTI_USART


【按键外部中断】  
按键 PA0, 上升沿有效, 松开点亮LED.  
按键 PA1, 下降沿有效, 按下熄灭LED.  

【按键轮询功能】  
按键 PA2, 下降沿有效, 按下时阻塞且反置LED, 松开后退出阻塞.  
按键 PA3, 按下点亮LED, 松开熄灭LED。  

【通用同步异步收发器 - USART】【使用同步收发器的异步模式 - UART】
USART1_Rx/Tx - PA10/PA9, 接 CH340 的USB串口模块, 波特率 115200;  
USART2_Rx/Tx - PB11/PB10, 接 HC-05 的蓝牙串口模块, 波特率 9600;  

【串口收发功能】未完成, 目前实现:

1. USART1 的非中断收发不定长(小于64)字符串数据。  
    ```C
    #define USART1_INTERRUPT_DISABLE
    ```
2. USART1 中断收发不定长(小于64)字符串数据。  
    ```C
    #define USART1_INTERRUPT_ENABLE
    ```
    1. 情况一: 单次收取一字节, 收到结束符 或 缓冲区溢出 时处理数据。  
        ```C
        #if defined USART1_INT_SOLUTION_1 /* UART_Receive_IT & UART_RxCpltCallback. */
        /* Receive only one char once idle, so cannot figure uncertian length data with idle. */
        HAL_UART_Receive_IT(&huart1, &Char_USART1_Rx_IT, 1);
        ```
    2. 情况二: 单次收取整个缓冲区, 收到指定长度 或 空闲中断 时处理数据。  
        ```C
        #elif defined USART1_INT_SOLUTION_2 /* UART_Receive_IT & UART_RxCpltCallback & UART_IDLE_Callback. */
        /* Receive full buff once complete, so can figure uncertian length data with idle event. */
        __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
        HAL_UART_Receive_IT(&huart1, Buff_USART1_Rx_IT, sizeof(Buff_USART1_Rx_IT)-1);
        ```
    3. 情况三: 考虑使用 HAL_UARTEx_RxEventCallback() 函数。  
        ```C
        待补充
        ```
3. 