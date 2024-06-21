#include "usb.h"

unsigned char CDC_Tx_buff[50] = {"[STM32F103C8T6,USB_DEVICE,CDC_Tx_buff.]\r\n"}; // '\r'=0x0D,'\n'=0x0A.
unsigned char CDC_Rx_buff[50] = {""};
unsigned char CDC_Rx_flag = 0;

// USB 卸载操作,取消按复位键后无法识别的问题.
void USB_Device_DeInit(void)
{
    __HAL_RCC_USB_FORCE_RESET();
    HAL_Delay(200);
    __HAL_RCC_USB_RELEASE_RESET();

    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_Initure;
    GPIO_Initure.Pin = GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull = GPIO_PULLDOWN;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_Delay(300);
}

void usb_cdc_rx_cmd_process(void)
{
    if (CDC_Rx_flag)
    {
        // 处理指令，这里简单打印指令内容，你可以根据实际需求执行相应的操作
        // printf("USART_CMD_Received: [%s].\n", RxBuffer);

        if (strncmp((char*)CDC_Rx_buff, "CMD+LED+", strlen("CMD+LED+")) == 0)
        {
            // 利用指针的算术运算，将 command 指针指向了 RxBuffer 后面的位置，即去掉了前缀的部分。
            const char *command = (char*)CDC_Rx_buff + strlen("CMD+LED+");
            if (strncmp(command, "ON",2) == 0)
            {
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
            }
            else if (strncmp(command, "OFF",3) == 0)
            {
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
            }
            else if (strncmp(command, "TOGGLE",6) == 0)
            {
                HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
            }
        }
        CDC_Rx_flag = 0;
    }
}

void USB_Main_Loop(void)
{
    // HAL_Delay(1000);
    // CDC_Transmit_FS(CDC_Tx_buff,sizeof(CDC_Tx_buff));
    // CDC_Transmit_FS(CDC_Rx_buff,sizeof(CDC_Rx_buff));
    usb_cdc_rx_cmd_process();
}
