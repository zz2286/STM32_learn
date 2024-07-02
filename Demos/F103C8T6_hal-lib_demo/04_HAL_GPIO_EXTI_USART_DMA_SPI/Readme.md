## 04_HAL_GPIO_EXTI_USART_DMA_SPI
STM32F103C8T6 基于 CubeMX 生成代码，并由 VSCode 编辑，再由 Keil 编译的项目。

# 本demo新实现功能

针对 BK4819 芯片和开发板，通过GPIO模拟SPI时序进行通信，实现读写对应地址的寄存器值以实现相关功能。

本demo只是早期验证SPI接口的代码版本，有关 BK4819 的更多后续控制代码，请参考 Demos\F103C8T6_hal-lib_demo\\#_Application\BK4819_SPI\_#_HAL_GPIO_EXTI_USART_DMA_SPI。

本demo的所谓SPI只是 针对 BK4819 数据手册所要求的 SPI 时序图进行的 软件GPIO模拟SPI通信，并未添加STM32的SPI模块配置。


# 之前已实现的基础功能

**按键轮询功能**
按键 PA2, 下降沿有效, 按下时阻塞且反置LED, 松开后退出阻塞.  
按键 PA3, 按下点亮LED, 松开熄灭LED。  

**按键外部中断**
按键 PA0, 上升沿有效, 松开点亮LED.  
按键 PA1, 下降沿有效, 按下熄灭LED.  

**串口收发功能**
使用的是 通用同步收发器`USART` 的异步模式 `UART`  
串口连接:  
- USART1_Rx/Tx - PA10/PA9, 接 CH340 的USB串口模块, 波特率 115200;  
- (未实现)USART2_Rx/Tx - PB11/PB10, 接 HC-05 的蓝牙串口模块, 波特率 9600;  

一、非中断 收发数据  
1. 非中断 串口收发  
2. 输入输出接口重定向  
3. 非中断 调试输出日志  

二、串口指令读取与外设控制  
- 自定义函数: 读取数据-识别指令-控制外设。  

三、串口接收不定长字符串数据(多种方案)  
1. 非中断 接收不定长字符串  
2. 中断 接收不定长字符串  
- 方案一: 单次收取一字节, 收到结束符 或 缓冲区溢出 时处理数据。  
    - 使用 `UART_Receive_IT` 函数, 单次接收一个字符。  
    - 使用 `UART_RxCpltCallback` 函数, 处理字符并开启下一次接收。  
- 方案二: 单次收取整个缓冲区, 收到指定长度 或 空闲中断 时处理数据。  
    - 使用 `HAL_UART_Receive_IT` 函数收取指定长度字符。  
    - 在 `USART1_IRQHandler` 中断处理函数中判定串口空闲标志位, 并调用自定义的 `AAA_UART_IDLE_Callback` 空闲中断回调函数。  
    - 在自定义的 `AAA_UART_IDLE_Callback` 空闲中断回调函数中, 使用 `UART_Start_Receive_IT` 函数开启下一次接收。  
- 方案三: 方案二的简化版。  
    - 使用 `HAL_UARTEx_ReceiveToIdle_IT` 和 `HAL_UARTEx_RxEventCallback` 函数。  
3. 中断+DMA 接收不定长字符串
- 两种方案: HAL_UART_Receive_DMA 和 HAL_UARTEx_ReceiveToIdle_DMA





