#ifndef __BSP_OLED_H
#define __BSP_OLED_H 

#include "stm32f10x.h"
#include <stdlib.h>
#include <string.h>

#if 0
    #define SPI_SOFTWARE
#else
    #define SPI_HARDWARE
#endif

#ifdef SPI_SOFTWARE
// OLED_CS, SPI_NSS, PA4
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define OLED_CS_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_4)
// OLED_D0_SCL, SPI_SCK, PA5
#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define OLED_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_5)
// OLED_D1_SDA, SPI_MOSI, PA7
#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_7)
#define OLED_SDIN_Set() GPIO_SetBits(GPIOA,GPIO_Pin_7)
#endif


// OLED_RES, PB0
#define OLED_RST_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_0)
#define OLED_RST_Set() GPIO_SetBits(GPIOB,GPIO_Pin_0)
// OLED_DC, PB1
#define OLED_DC_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_1)
#define OLED_DC_Set() GPIO_SetBits(GPIOB,GPIO_Pin_1)

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_ClearAll(void);
void OLED_ClearPoint(u8 x,u8 y);
void OLED_DrawPoint(u8 x,u8 y);
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2);
void OLED_DrawCircle(u8 x,u8 y,u8 r);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1);
void OLED_ShowString(u8 x,u8 y,char *chr,u8 size1);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1);
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1);
void OLED_ShowPicture(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[]);
void OLED_ScrollDisplay(u8 num,u8 space);

u32 OLED_Pow(u8 m,u8 n);
void OLED_Set_StartAddr(u8 x,u8 y);

void OLED_Init(void);
void OLED_Demo();
void OLED_Process_Loop();

#ifdef SPI_SOFTWARE
void OLED_WR_Byte(u8 dat,u8 cmd);
#elif defined SPI_HARDWARE
void OLED_WR_Byte(uint8_t data, uint8_t data_type);
#endif
void OLED_GPIO_Init(void);
void OLED_Refresh(void);



/**
 * @brief SSDi306 chip command definition
 */
#define SSD1306_CMD_LOWER_COLUMN_START_ADDRESS              0x00        /**< command lower column start address */
#define SSD1306_CMD_HIGHER_COLUMN_START_ADDRESS             0x10        /**< command higher column start address */
#define SSD1306_CMD_MEMORY_ADDRESSING_MODE                  0x20        /**< command memory addressing mode */
#define SSD1306_CMD_SET_COLUMN_ADDRESS                      0x21        /**< command set column address */
#define SSD1306_CMD_SET_PAGE_ADDRESS                        0x22        /**< command set page address */
#define SSD1306_CMD_SET_FADE_OUT_AND_BLINKING               0x23        /**< command set fade out and blinking */
#define SSD1306_CMD_RIGHT_HORIZONTAL_SCROLL                 0x26        /**< command right horizontal scroll */
#define SSD1306_CMD_LEFT_HORIZONTAL_SCROLL                  0x27        /**< command left horizontal scroll */
#define SSD1306_CMD_VERTICAL_RIGHT_HORIZONTAL_SCROLL        0x29        /**< command vertical right horizontal scroll */
#define SSD1306_CMD_VERTICAL_LEFT_HORIZONTAL_SCROLL         0x2A        /**< command vertical left horizontal scroll */
#define SSD1306_CMD_DEACTIVATE_SCROLL                       0x2E        /**< command deactivate scroll */
#define SSD1306_CMD_ACTIVATE_SCROLL                         0x2F        /**< command activate scroll */
#define SSD1306_CMD_DISPLAY_START_LINE                      0x40        /**< command display start line */
#define SSD1306_CMD_CONTRAST_CONTROL                        0x81        /**< command contrast control */
#define SSD1306_CMD_CHARGE_PUMP_SETTING                     0x8D        /**< command charge pump setting */
#define SSD1306_CMD_COLUMN_0_MAPPED_TO_SEG0                 0xA0        /**< command column 0 mapped to seg 0 */
#define SSD1306_CMD_COLUMN_127_MAPPED_TO_SEG0               0xA1        /**< command column 127 mapped to seg 0 */
#define SSD1306_CMD_VERTICAL_SCROLL_AREA                    0xA3        /**< command vertical scroll area */
#define SSD1306_CMD_ENTIRE_DISPLAY_OFF                      0xA4        /**< command entire display off */ 
#define SSD1306_CMD_ENTIRE_DISPLAY_ON                       0xA5        /**< command entire display on */ 
#define SSD1306_CMD_NORMAL_DISPLAY                          0xA6        /**< command normal display */ 
#define SSD1306_CMD_INVERSE_DISPLAY                         0xA7        /**< command inverse display */ 
#define SSD1306_CMD_MULTIPLEX_RATIO                         0xA8        /**< command multiplex ratio */ 
#define SSD1306_CMD_DISPLAY_OFF                             0xAE        /**< command display off */ 
#define SSD1306_CMD_DISPLAY_ON                              0xAF        /**< command display on */ 
#define SSD1306_CMD_PAGE_ADDR                               0xB0        /**< command page address */ 
#define SSD1306_CMD_SCAN_DIRECTION_COM0_START               0xC0        /**< command scan direction com 0 start */ 
#define SSD1306_CMD_SCAN_DIRECTION_COMN_1_START             0xC8        /**< command scan direction com n-1 start */ 
#define SSD1306_CMD_DISPLAY_OFFSET                          0xD3        /**< command display offset */ 
#define SSD1306_CMD_DISPLAY_CLOCK_DIVIDE                    0xD5        /**< command display clock divide */ 
#define SSD1306_CMD_SET_ZOOM_IN                             0xD6        /**< command set zoom in */ 
#define SSD1306_CMD_PRE_CHARGE_PERIOD                       0xD9        /**< command pre charge period */ 
#define SSD1306_CMD_COM_PINS_CONF                           0xDA        /**< command com pins conf */ 
#define SSD1306_CMD_COMH_DESLECT_LEVEL                      0xDB        /**< command comh deslect level */ 
#define SSD1306_CMD_NOP                                     0xE3        /**< command nop */ 

/* Commands:
Fundamental Command:
- Set Lower Column Start Address for Page Addressing Mode (00h~0Fh).
  - 设置页面寻址模式下显示数据RAM的8位列起始地址的低4位部分。
- Set Higher Column Start Address for Page Addressing Mode (10h~1Fh).
  - 设置页面寻址模式下显示数据RAM的8位列起始地址的高4位部分。
- Set Memory Addressing Mode (20h) -> Mode Value (00h/01h/02h).
  - 页寻址模式 Page addressing mode (02h):
    - Set the page start address of the target display location by command B0h to B7h. 
    - Set the lower start column address of pointer by command 00h~0Fh. 
    - Set the upper start column address of pointer by command 10h~1Fh. 
  - 水平寻址模式 Horizontal addressing mode (00h):
    - Set the column start and end address of the target display location by command 21h.
    - Set the page start and end address of the target display location by command 22h. 
  - 垂直寻址模式 Vertical addressing mode (01h):
    - Set the column start and end address of the target display location by command 21h.
    - Set the page start and end address of the target display location by command 22h. 
- Set Column Address (21h) -> Start Address (00h) -> End Address (7Fh).
  - 指定显示数据RAM的列起始地址和列结束地址。该命令还将列地址指针设置为列起始地址。该指针用于定义图形显示数据RAM中的当前读/写列地址。
  - 如果通过命令20h开启了水平地址增量模式，当完成一列数据的读写后，会自动增加到下一列地址。每当列地址指针完成对结束列地址的访问时，它将被重置为开始列地址，并且行地址在净行上增加。
- Set Page Address (22h) -> Start Address (00h) -> End Address (07h).
  - 指定显示数据RAM的页起始地址和结束地址。该命令还将页地址指针设置为页起始地址。该指针用于定义图形显示数据RAM中的当前读/写页地址。
  - 如果使用20h命令开启垂直地址增量模式，当完成一页数据的读写后，会自动增量到下一页地址。每当页地址指针完成对结束页地址的访问时，它就被重置回起始页地址。
- Set Display Start Line (40h~7Fh).
  - 设置显示起始行寄存器，通过选择0到63之间的值来确定显示RAM的起始地址，指令值与真实值关系为 (40h~7Fh) = 40h+(00h~3Fh)。
  - 当值为0时，将RAM第0行映射到COM0。值为1时，将RAM第1行映射到COM0，依此类推。
- Set Contrast Control for BANK0 (81h) -> Value (00h~FFh).
  - 设置显示对比度控制，范围在0-255。段输出电流随着对比度阶跃值的增加而增加。
- Set Segment Re-map (A0h/A1h).
  - 改变显示数据列地址和段驱动程序之间的映射关系。它允许OLED模块设计的灵活性。该命令只影响后续的数据输入。已经存储在GDDRAM中的数据不会被更改。
- Entire Display ON (A4h/A5h).
  - A4 命令使能根据GDDRAM内容显示输出。
  - A5 命令强制整个显示处于“ON”状态，无论显示数据RAM的内容如何。
  - A5 指令后可以通过 A4 指令恢复输出GDDRAM内容。
- Set Normal/Inverse Display (A6h/A7h).
  - 将显示设置为正常或反向。在正常显示中，RAM数据为1表示“开”像素，而在反向显示中，RAM数据为0表示“开”像素。
- Set Multiplex Ratio (A8h) -> Value (10h~3Fh).
  - 设置复用比: 将默认的63复用模式切换为任意复用比，范围为16 ~ 63。从前往后选用输出端口COM0-COM63用于驱动显示屏。
- Set Display ON/OFF (AEh/AFh).
- Set Page Start Address for Page Addressing Mode (B0h~B7h).
  - 在页面寻址模式下定位GDDRAM中的页起始地址，从 Page 0到 Page 7。
- Set COM Output Scan Direction (C0h/C8h).
  - 设置COM输出的扫描方向。一旦发出此命令，图形显示将立即垂直翻转。
- Set Display Offset (D3h) -> Value (00h~3Fh).
  - 指定显示起始行到COM0~COM63之一的映射(假设COMO是显示起始行，则显示起始行寄存器等于0)。
  - 例如，为了将COM16向COM0方向移动16行，第二个字节中的6位数据应该被给定为10h。要向相反方向移动16行，6位数据应该由64-16给出，因此第二个字节将是20h。
- Set Display Clock Divide Ratio / Oscillator Frequency (D5h) -> Value (80h).
  - 设置 时钟分频比A[3:0] 和 振荡器频率A[7:4]，默认值 (80h)。
  - 时钟分频比 Display Clock Divide Ratio，从CLK生成DCLK (Display Clock)。分频比(0h~Fh)对应1~16，重置值(0h)对应为1。
  - 振荡器频率 Fosc，如果CLS引脚被拉高，它就是CLK的来源。4位值产生16种不同的频率设置，如下所示。默认值为8h。
- Set Pre-charge Period (D9h).
  - 设置预充电周期。该间隔以 DCLK 数计算，其中 RESET 等于 2 个 DCLK。
- Set COM Pins Hardware Configuration (DAh).
  - 设置COM信号引脚配置以匹配OLED面板硬件布局。下表显示了不同条件下(MUX复用比=64)的COM引脚配置:
- Set V_COMH Deselect Level (DBh).
- NOP, No Operation Command (E3h).

Graphic Acceleration Command:
- Horizontal Scroll Setup (26h/27h).
  - 水平滚动: 该命令由连续字节组成，用于设置水平滚动参数，并确定滚动的起始页、结束页和滚动速度。
  - 在发出此命令之前，必须禁用水平滚动(2Eh)。否则，RAM内容可能会损坏。
- Continuous Vertical and Horizontal Scroll Setup (29h/2Ah).
  - 连续垂直滚动: 该命令由6个连续字节组成，用于设置连续垂直滚动参数，并确定滚动的起始页、结束页、滚动速度和垂直滚动偏移量。
  - 命令29h/2Ah的字节B[2:0]，C[2:0]和D[2:0]用于设置连续水平滚动。字节E[5:0]用于设置连续垂直滚动偏移量。所有这些字节一起用于设置连续对角线(水平+垂直)滚动。如果垂直滚动偏移字节E[5:0]被设置为零，则只执行水平滚动(如命令26/27h)。
  - 在发出此命令之前，必须停用滚动(2Eh)。否则，RAM内容可能会损坏。
- Deactivate Scroll (2Eh).
  - 停止滚动。发送2Eh命令去激活滚动动作后，ram数据需要重写。
- Activate Scroll (2Fh).
  - 启动滚动。只有在滚动设置命令26h/27h/29h/2Ah定义了滚动设置参数后才应该发出。
  - 最后一个滚动设置命令中的设置将覆盖前一个滚动设置命令中的设置。
  - 激活滚动后，禁止以下操作1.RAM访问(数据写入或读取)2.更改水平滚动设置参数。
- Set Vertical Scroll Area(A3h).
*/

#endif /* __BSP_OLED_H */