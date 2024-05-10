#include "bsp_key.h"
#include "bsp_led.h"

/** 按键端口配置函数 Key_GPIO_Config
  * 将 PA2|PA3 设为内部上拉输入
  */
void Key_GPIO_Config(void)
{
    /*开启按键端口的时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    //选择按键的引脚 PA0|PA1, 内部上拉输入
    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //选择按键的引脚 , 内部上拉输入
    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
}

/**
  * @brief  按键扫描函数 Key_Scan
  * @param  GPIOx       x 可以是 A，B，C，D或者 E
  * @param  GPIO_Pin    待读取的端口位
  * @retval KEY_State = KEY_OFF 或 KEY_ON
  * @note   检测是否有按键按下
  */
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{
    /*检测是否有按键按下 */
    if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0 )
    {
        /*等待按键释放 */
        while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0);
        return KEY_ON;
    }
    else
        return KEY_OFF;
}

/**
  * @brief  按键扫描检测 Key_Scan_Hold
  * @note   检测到按键按下期间LED随亮随灭
  */
void Key_Scan_Hold(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==0){Block_Delay_ms(20);
    if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==0)
    {
        LED_ON();
        while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0)
            ;
        LED_OFF();
    }}
}
/**
  * @brief  按键扫描检测 Key_Scan_Down
  * @note   检测到按键下降沿LED反转
  */
void Key_Scan_Down(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==0){Block_Delay_ms(20);
    if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==0)
    {
        LED_TOGGLE();
        while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0)
            ;
    }}
}
/**
  * @brief  按键扫描检测 Key_Scan_Up
  * @note   检测到按键上升沿LED反转
  */
void Key_Scan_Up(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==0){Block_Delay_ms(20);
    if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==0)
    {
        while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0)
            ;
        LED_TOGGLE();
    }}
}


/**
 * @brief 按键轮询处理函数
 * @note  设置按键处理功能
*/
void Key_Process_Loop(void)
{
    // Key-PA01,由外部中断控制
    // Key-PA23,按下亮,松开灭
    Key_Scan_Hold(GPIOA, GPIO_Pin_2);
    Key_Scan_Hold(GPIOA, GPIO_Pin_3);
}
