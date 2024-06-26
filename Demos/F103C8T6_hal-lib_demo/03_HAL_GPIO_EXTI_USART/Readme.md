## 03_HAL_GPIO_EXTI_USART
STM32F103C8T6 基于 CubeMX 生成代码，并由 VSCode 编辑，再由 Keil 编译的项目。

## 之前已实现的基础功能

**按键轮询功能**
按键 PA2, 下降沿有效, 按下时阻塞且反置LED, 松开后退出阻塞.  
按键 PA3, 按下点亮LED, 松开熄灭LED。  

**按键外部中断**
按键 PA0, 上升沿有效, 松开点亮LED.  
按键 PA1, 下降沿有效, 按下熄灭LED.  


# 串口收发功能

通用同步异步收发器 `USART`  
在本案例中, 使用的是 通用同步收发器 的异步模式 `UART`  

#### 串口连接:

- USART1_Rx/Tx - PA10/PA9, 接 CH340 的USB串口模块, 波特率 115200;  
- USART2_Rx/Tx - PB11/PB10, 接 HC-05 的蓝牙串口模块, 波特率 9600;  


## 一、非中断 收发数据

### 1. 非中断 串口收发 {ch1.1}
```C
/* Disable  USART1 intertupt. */
#ifdef USART1_INTERRUPT_DISABLE

uint8_t Flag_USART1_Rx = 0;
uint8_t Char_USART1_Rx = 0;
uint8_t Cntr_USART1_Rx = 0;
uint8_t Buff_USART1_Tx[65] = "Data in Buff_USART1_Tx[65].";
uint8_t Buff_USART1_Rx[65] = "Data in Buff_USART1_Rx[65].";
const char *info = "\n\
Mode: USART1 interrput disable.\n\
Help:\n\
1. Get Buff_USART1_Rx[65] end with '.' or max len 64 or idle event;\n\
2. Get <CMD+LED+?.> or <CMD+BT+?.>.\n\
\n";


void AAA_USART1_Demo_Main(void)
{
  AAA_LOG_INFO(info);
  AAA_LOG_DEBUG("Print USART1_Tx: [%s]\n", Buff_USART1_Tx);
  AAA_LOG_INFO("Getting USART1_Rx...\n");
  memset(Buff_USART1_Rx, 0, sizeof(Buff_USART1_Rx));
}

void AAA_USART1_Demo_Loop(void)
{
  #if 0 /* Only echo buff. */
  if (HAL_UART_Receive(&huart1, Buff_USART1_Rx, sizeof(Buff_USART1_Rx), 0xFFFF) == HAL_OK)
  {
    AAA_LOG_DEBUG("Get USART1_Rx.");
    HAL_UART_Transmit(&huart1, Buff_USART1_Rx, sizeof(Buff_USART1_Rx), HAL_MAX_DELAY);
  }

  #elif 0 /* Log message and do commmand, end with charactor or buff-overflow. */
  if (HAL_UART_Receive(&huart1, &Char_USART1_Rx, 1, 0) == HAL_OK)
  {
    /* Get max len = sizeof(buff), max inedx = sizeof(buff)-1. */
    if ((Flag_USART1_Rx == 0) && (Cntr_USART1_Rx <= sizeof(Buff_USART1_Rx)-1))
    {
      Buff_USART1_Rx[Cntr_USART1_Rx] = Char_USART1_Rx;
      Cntr_USART1_Rx ++;
      /* Get end-of-message character '.' or Buffer overflow. */
      if ((Char_USART1_Rx == '.') || (Cntr_USART1_Rx == sizeof(Buff_USART1_Rx)))
      {
        /* Replace final character to '\0'. */
        Buff_USART1_Rx[Cntr_USART1_Rx-1] = '\0';
        Flag_USART1_Rx = 1;
      }
    }
  }
  AAA_USART1_Demo_Process(50);

  #else /* Log message and do commmand, end with charactor or buff-overflow or idle event. */
  uint16_t RxLen = 0;
  if (HAL_UARTEx_ReceiveToIdle(&huart1, Buff_USART1_Rx, sizeof(Buff_USART1_Rx), &RxLen, HAL_MAX_DELAY) == HAL_OK)
  {
    /* Get end-of-message character '.' */
    if (Buff_USART1_Rx[RxLen-1] == '.')
    {
      Buff_USART1_Rx[RxLen-1] = '\0';
    }
    Flag_USART1_Rx = 1;
  }
  AAA_USART1_Demo_Process(50);

  #endif
}

void AAA_USART1_Demo_Process(uint32_t tick_interval)
{
  static uint32_t tick_counter = 0;
  if ((HAL_GetTick() - tick_counter) < tick_interval) return;
  tick_counter = HAL_GetTick();
  
  if (Flag_USART1_Rx == 1)
  {
    /* Print Log. */
    AAA_LOG_DEBUG("Get USART1_Rx: [%s]\n",Buff_USART1_Rx);
    /* Deal with data as commmand. */
    AAA_USART1_Ctrl_CMD(Buff_USART1_Rx);
    /* Clear params. */
    Flag_USART1_Rx = 0;
    Char_USART1_Rx = 0;
    Cntr_USART1_Rx = 0;
    memset(Buff_USART1_Rx, 0, sizeof(Buff_USART1_Rx));
  }
}

#endif  /* Disable  USART1 intertupt. */
```

### 2. 输入输出接口重定向
```C
/* re-direct C library function to USART1_Tx. */
int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}
/* re-direct C library function to USART1_Rx. */
int fgetc(FILE *f)
{
    uint8_t ch = 0;
    HAL_UART_Receive(&huart1, &ch, 1, HAL_MAX_DELAY);
    return ch;
}
```

### 3. 非中断 调试输出日志
```C
/**
* @brief USART1 print log in blocking mode.
* @param level "DEBUG","INFO".
* @note To be called by marco AAA_LOG_DEBUG() or AAA_LOG_INFO().
* @note #define F(...) f(__VA_ARGS__)
* @note (const char *log_format, ...) = __VA_ARGS__
*/
void AAA_USART1_Log(const char *level, const char *file, const int line, const char *func,
                    const char *log_format, ...)  /* __VA_ARGS__ = const char *log_format, ... */
{
    /* Define arguments' list object. */
    va_list args;
    /* Get time_ms since booting. */
    uint32_t time_ms = HAL_GetTick();

    /* Deal with different Log level. */
    if (strncmp(level, "DEBUG",5) == 0)
    {
        /* Print Log debug title. */
        printf("[%d.%03d][%s]: %s, %d, %s:\n", (time_ms/1000), (time_ms%1000), level, file, line, func);
    }
    else if (strncmp(level, "INFO",4) == 0)
    {
        /* Only print info. */
        printf("[%d.%03d][%s]: ", (time_ms/1000), (time_ms%1000), level);
    }

    /* __VA_ARGS__: (log_format, ...) */
    va_start(args, log_format);
    vprintf(log_format, args);
    va_end(args);
}

#define AAA_LOG_DEBUG(...)  AAA_USART1_Log("DEBUG", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define AAA_LOG_INFO(...)   AAA_USART1_Log("INFO", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
```



## 二、串口指令读取与外设控制

### 1. 自定义函数: 读取数据-识别指令-控制外设。
```C
/**
* @brief USART1 read command and control peripheral.
* @param RxBuffer data to be read as <perfix> + <command>.
* @note CMD+LED+_ON/OFF/TOGGLE/BLINK=ms
* @note CMD+BT+_<AT command of Bluetooth>
*/
void AAA_USART1_Ctrl_CMD(uint8_t *RxBuffer)
{
    /* Control LED. */
    if (strncmp((char*)RxBuffer, "CMD+LED+", strlen("CMD+LED+")) == 0)
    {
        /* Pointer command to remove RxBuffer's "CMD+LED+" prefix. */
        const char *command = (char*)RxBuffer + strlen("CMD+LED+");
        /* Deal with command. */
        if (strncmp(command, "ON",2) == 0)
        {
            AAA_LOG_INFO("Get CMD+LED+ON and set led on.\n");
            AAA_LED_ON();
            // SysTick_Timer_Stop(TIMER_ID_LED_BLINK);
        }
        else if (strncmp(command, "OFF",3) == 0)
        {
            AAA_LOG_INFO("Get CMD+LED+OFF and set led off.\n");
            AAA_LED_OFF();
            // SysTick_Timer_Stop(TIMER_ID_LED_BLINK);
        }
        else if (strncmp(command, "TOGGLE",6) == 0)
        {
            AAA_LOG_INFO("Get CMD+LED+TOGGLE and set led toggle.\n");
            AAA_LED_TOGGLE();
            // SysTick_Timer_Stop(TIMER_ID_LED_BLINK);
        }
        else if (strncmp(command, "BLINK",5) == 0)
        {
            #if 0
            uint32_t blink_value = 0;
            sscanf(command, "BLINK=%d", &blink_value);
            AAA_LOG_INFO("Get CMD+LED+BLINK and set led blink in %d ms.\n",blink_value);
            SysTick_Timer_Start(TIMER_ID_LED_BLINK,-1,blink_value);
            #else
            AAA_LOG_INFO("CMD+LED+BLINK=ms function is not yet implemented.\n");
            #endif
        }
        else
        {
            AAA_LOG_INFO("Failed to get valid CMD+LED+<ON/OFF/TOGGLE/BLINK=ms*> .\n");
        }
    }
    /* Control Bluetooth. */
    else if (strncmp((char*)RxBuffer, "CMD+BT+", strlen("CMD+BT+")) == 0)
    {
        AAA_LOG_INFO("CMD+BT+ function is not yet implemented.\n");
    }
    /* Control nothing. Not read as command. */
    else
    {
        /* Do nothing. */
        AAA_LOG_INFO("Failed to read command.\n");
    }
}
```



## 三、串口接收不定长字符串数据(多种方案)

### 1. 非中断 接收不定长字符串
具体代码见前文 [非中断 串口收发](ch1.1) 部分。
```C
#define USART1_INTERRUPT_DISABLE
```

### 2. 中断 接收不定长字符串 
```C
#define USART1_INTERRUPT_ENABLE
```

**方案一**: 单次收取一字节, 收到结束符 或 缓冲区溢出 时处理数据。  
- 使用 `UART_Receive_IT` 函数, 单次接收一个字符。  
```C
#if defined USART1_INT_SOLUTION_1
/* HAL_UART_Receive_IT & HAL_UART_RxCpltCallback. */
/* Receive only one char once IDLE, so cannot get uncertian length data by IDLE event. */
/* Unless get the appointed end-charactor or buffer full. */
HAL_UART_Receive_IT(&huart1, &Char_USART1_Rx_IT, 1);
```
- 使用 `UART_RxCpltCallback` 函数, 处理字符并开启下一次接收。  
```C
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        /* Get max len = sizeof(buff), max index = sizeof(buff)-1. */
        if (Flag_USART1_Rx_IT == 0) // && (Counter_USART1_Rx_IT <= sizeof(Buff_USART1_Rx_IT)-1))
        {
            Buff_USART1_Rx_IT[Cntr_USART1_Rx_IT++] = Char_USART1_Rx_IT;
            /* Get end-of-message character '.' or Buffer overflow. */
            if ((Char_USART1_Rx_IT == '.') || (Cntr_USART1_Rx_IT == sizeof(Buff_USART1_Rx_IT)))
            {
            /* Replace final character to '\0'. */
            Buff_USART1_Rx_IT[Cntr_USART1_Rx_IT-1] = '\0';
            Flag_USART1_Rx_IT = 1;
            }
        }
        /* Continue Receive_IT. */
        HAL_UART_Receive_IT(&huart1, &Char_USART1_Rx_IT, 1);
    }
}
```

**方案二**: 单次收取整个缓冲区, 收到指定长度 或 空闲中断 时处理数据。  
- 使用 `HAL_UART_Receive_IT` 函数收取指定长度字符。  
```C
#elif defined USART1_INT_SOLUTION_2
/* HAL_UART_Receive_IT & HAL_UART_RxCpltCallback & AAA_UART_IDLE_Callback. */
/* Receive full buffer or get IDLE event, so can get uncertian length data. */
__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
HAL_UART_Receive_IT(&huart1, Buff_USART1_Rx_IT, sizeof(Buff_USART1_Rx_IT)-1);
```
- 缓冲区满溢时, 调用 `HAL_UART_RxCpltCallback` 函数, 但暂时不重启接收。  
```C
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        AAA_LOG_INFO("RxCpltCallback.\n");
        if (Flag_USART1_Rx_IT == 0)
        {
        Flag_USART1_Rx_IT = 1;
        }
    }
}
```
- 在 `USART1_IRQHandler` 中断处理函数中判定串口空闲标志位, 并调用自定义的 `AAA_UART_IDLE_Callback` 空闲中断回调函数。  
```c
void USART1_IRQHandler(void)
{
    /* USER CODE BEGIN USART1_IRQn 0 */
    #if defined(USART1_INTERRUPT_ENABLE) && defined(USART1_INT_SOLUTION_2)
    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE) != RESET)
    {
        AAA_UART_IDLE_Callback(&huart1);
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);
    }
    #endif
    /* USER CODE END USART1_IRQn 0 */
    HAL_UART_IRQHandler(&huart1);
}
```
- 在自定义的 `AAA_UART_IDLE_Callback` 空闲中断回调函数中, 使用 `UART_Start_Receive_IT` 函数开启下一次接收。  
```C
void AAA_UART_IDLE_Callback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        AAA_LOG_INFO("IDLE_Callback.\n");
        if (Flag_USART1_Rx_IT == 0)
        {
        Flag_USART1_Rx_IT = 1;
        }
        else /* Flag_USART1_Rx_IT == 1 */
        {
        /* RxCpltCallback has been called. */
        /* Or AAA_USART1_Demo_Process has not finished yet. */
        /* Do nothing. */
        }
        /* Restart with UART_Start_Receive_IT() instead of HAL_UART_Receive_IT(). */
        UART_Start_Receive_IT(&huart1, Buff_USART1_Rx_IT, sizeof(Buff_USART1_Rx_IT)-1);
    }
}
```

**方案三**: 方案二的简化版。  
- 使用 `HAL_UARTEx_ReceiveToIdle_IT` 和 `HAL_UARTEx_RxEventCallback` 函数。  
```C
#elif defined USART1_INT_SOLUTION_3
/* HAL_UARTEx_ReceiveToIdle_IT & HAL_UARTEx_RxEventCallback. */
/* Same as Solution_2. Receive till either the expected number of data is received or an IDLE event occurs. */
HAL_UARTEx_ReceiveToIdle_IT(&huart1, Buff_USART1_Rx_IT, sizeof(Buff_USART1_Rx_IT)-1);
```
- 使用 `HAL_UARTEx_ReceiveToIdle_IT` 函数开启下一次接收。  
```C
HAL_UARTEx_ReceiveToIdle_IT(&huart1, Buff_USART1_Rx_IT, sizeof(Buff_USART1_Rx_IT)-1);
```

### 3. 中断+DMA 接收不定长字符串






# Template & Format


- [ ] 未完成的任务  
- [x] 已完成的任务  


[Baidu](https://www.baidu.com "Click to https://www.baidu.com.")  
[baidu][url_web_1]  
[bilibili][url_web_2]  
[Bilibili]  
[搜图神器][url_web_3]  


![url_img_0]( https://hbimg.huaban.com/39c128c413cbaa1a4dc4a1f185041b3b8940ff41eed3e0-68ZWjR "星空")  
![url_img_1]  
![url_img_2]  
![url_img_3]  

Here is a footnote reference[^abcd].  
Here is a footnote reference[^1234].  

<!-- 水平线 -->
---
***
___

# END

<!-- 这是一个HTML注释，它在Markdown渲染后不会显示 -->

<!-- 以下是不直接显示的内容 -->

[url_web_1]: 
    https://www.baidu.com 
    "Click to [url_web_1]."
[url_web_2]: 
    https://www.bilibili.com 
    "Click to [url_web_2]."
[Bilibili]: 
    https://www.bilibili.com 
    "Click to [Bilibili]."
[url_web_3]:
    https://www.soutushenqi.com/home
    "Click to [url_web_3]: 搜图神器"
<!--

-->
[url_img_1]: 
    https://hbimg.huaban.com/39c128c413cbaa1a4dc4a1f185041b3b8940ff41eed3e0-68ZWjR
    "星空"
[url_img_2]:
    https://i0.hdslb.com/bfs/article/5a5f98de16ff68694ca06ec9dcc9d30908c70f83.jpg
    "天空云彩"
[url_img_3]:
    https://up.enterdesk.com/edpic_source/02/17/ca/0217ca947097907cf3662745b928b9cd.jpg
    "山川湖海"


[^abcd]: This is the footnote text.  
[^1234]: This is the footnote text.  
