#include "bsp_key.h"

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
  * 按键扫描函数 Key_Scan
  * 检测是否有按键按下
  * @param  GPIOx       x 可以是 A，B，C，D或者 E
  * @param  GPIO_Pin    待读取的端口位
  * @retval KEY_State = KEY_OFF 或 KEY_ON
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
/*********************************************END OF FILE**********************/
