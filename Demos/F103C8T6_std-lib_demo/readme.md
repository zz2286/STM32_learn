# STM32_learn/Demos/F103C8T6_std-lib_demo

本文件夹下存放一些标准库学习过程中实现的demo

**以下文件还有待修改**

1. 08_GPIO_RCC_EXTI_SysTick_USART_DMA_Hardware_SPI_OLED
    + 检查USART的DMA功能是否打开，否则收发错误。
    - 将OLED定帧输出，设置刷新周期定时器，实现并非阻塞延时。

2. 04_GPIO_RCC_EXTI_SysTick
    - 修改软件定时器的逻辑，将由中断触发的定时器直接在定时器中断回调中嵌套功能执行回调，否则按现代码仍然在轮询中判定没实际意义。

