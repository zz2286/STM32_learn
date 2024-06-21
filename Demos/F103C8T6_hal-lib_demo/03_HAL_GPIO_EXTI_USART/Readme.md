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

#define AAA_DEBUG_LOG(...) AAA_USART1_Debug_Log("DEBUG", __TIME__, __FILE__, __LINE__, __func__, __VA_ARGS__)

void AAA_USART1_Demo_Main(void);  
发送。

void AAA_USART1_Demo_Loop(void);  
接收，按固定buff长度接收回显  
接收，按'.'标志符接收回显  
