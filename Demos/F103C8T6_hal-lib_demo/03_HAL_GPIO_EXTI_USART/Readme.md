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

串口功能未完成, 目前实现:

1. USART1 的非中断收发不定长(小于64)字符串数据。  
    ```
    #define USART1_INTERRUPT_DISABLE
    ```
2. 