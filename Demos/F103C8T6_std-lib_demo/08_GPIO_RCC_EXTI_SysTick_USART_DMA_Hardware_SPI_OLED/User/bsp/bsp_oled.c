#include "bsp_systick.h"
#include "bsp_oled.h"
#include "bsp_oled_font.h"
#include "bsp_oled_bmp.h"
#include "bsp_spi.h"



// 图形显示数据随机访问存储器 (GDDRAM, Graphic Display Data RAM )
u8 OLED_GRAM[128][8];

// 反显函数
void OLED_ColorTurn(u8 i)
{
    if(i==0)
    {
        OLED_WR_Byte(0xA6,OLED_CMD);//正常显示
    }
    if(i==1)
    {
        OLED_WR_Byte(0xA7,OLED_CMD);//反色显示
    }
}

// 屏幕旋转180度
void OLED_DisplayTurn(u8 i)
{
    if(i==0)
    {
        OLED_WR_Byte(0xC8,OLED_CMD);//正常显示
        OLED_WR_Byte(0xA1,OLED_CMD);
    }
    if(i==1)
    {
        OLED_WR_Byte(0xC0,OLED_CMD);//反转显示
        OLED_WR_Byte(0xA0,OLED_CMD);
    }
}

// 开启OLED显示 
void OLED_DisPlay_On(void)
{
    OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
    OLED_WR_Byte(0x14,OLED_CMD);//开启电荷泵
    OLED_WR_Byte(0xAF,OLED_CMD);//点亮屏幕
}

// 关闭OLED显示 
void OLED_DisPlay_Off(void)
{
    OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
    OLED_WR_Byte(0x10,OLED_CMD);//关闭电荷泵
    OLED_WR_Byte(0xAF,OLED_CMD);//关闭屏幕
}

// 清除全部点
void OLED_ClearAll(void)
{
    // u8 i,n;
    // for(i=0;i<8;i++)
    // {
    //     for(n=0;n<128;n++)
    //     {
    //         OLED_GRAM[n][i]=0;//清除所有数据
    //     }
    // }
    memset(OLED_GRAM, 0, sizeof(OLED_GRAM));
    // OLED_Refresh();//更新显示
}

// 清除一个点, x:0~127, y:0~63
void OLED_ClearPoint(u8 x,u8 y)
{
    u8 i,m,n;
    i=y/8;
    m=y%8;
    n=1<<m;
    OLED_GRAM[x][i]=~OLED_GRAM[x][i];
    OLED_GRAM[x][i]|=n;
    OLED_GRAM[x][i]=~OLED_GRAM[x][i];
}

// 画点, x:0~127, y:0~63
void OLED_DrawPoint(u8 x,u8 y)
{
    u8 i,m,n;
    i=y/8;
    m=y%8;
    n=1<<m;
    OLED_GRAM[x][i]|=n;
}

// 画线, x:0~128, y:0~64
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2)
{
    u8 i,k,k1,k2,y0;
    if((x1<0)||(x2>128)||(y1<0)||(y2>64)||(x1>x2)||(y1>y2)) return;
    if(x1==x2)    //画竖线
    {
        for(i=0;i<(y2-y1);i++)
        {
            OLED_DrawPoint(x1,y1+i);
        }
    }
    else if(y1==y2)   //画横线
    {
        for(i=0;i<(x2-x1);i++)
        {
            OLED_DrawPoint(x1+i,y1);
        }
    }
    else      //画斜线
    {
        k1=y2-y1;
        k2=x2-x1;
        k=k1*10/k2;
        for(i=0;i<(x2-x1);i++)
        {
            OLED_DrawPoint(x1+i,y1+i*k/10);
        }
    }
}

// 画圆, x,y:圆心坐标, r:圆的半径
void OLED_DrawCircle(u8 x,u8 y,u8 r)
{
    int a, b,num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r)
    {
        OLED_DrawPoint(x + a, y - b);
        OLED_DrawPoint(x - a, y - b);
        OLED_DrawPoint(x - a, y + b);
        OLED_DrawPoint(x + a, y + b);

        OLED_DrawPoint(x + b, y + a);
        OLED_DrawPoint(x + b, y - a);
        OLED_DrawPoint(x - b, y - a);
        OLED_DrawPoint(x - b, y + a);
        
        a++;
        num = (a * a + b * b) - r*r;//计算画的点离圆心的距离
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}

// 在指定位置显示一个字符,包括部分字符, x:0~127, y:0~63, size:选择字体 12/16/24, 取模方式 逐列式
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1)
{
    u8 i,m,temp,size2,chr1;
    u8 y0=y;
    size2=(size1/8+((size1%8)?1:0))*(size1/2);  //得到字体一个字符对应点阵集所占的字节数
    chr1=chr-' ';  //计算偏移后的值
    for(i=0;i<size2;i++)
    {
        if(size1==12)
        {temp=asc2_1206[chr1][i];} //调用1206字体
        else if(size1==16)
        {temp=asc2_1608[chr1][i];} //调用1608字体
        else if(size1==24)
        {temp=asc2_2412[chr1][i];} //调用2412字体
        else return;
        for(m=0;m<8;m++)           //写入数据
        {
            if(temp&0x80) OLED_DrawPoint(x,y);
            else OLED_ClearPoint(x,y);
            temp<<=1;
            y++;
            if((y-y0)==size1)
            {
                y=y0;
                x++;
                break;
            }
        }
    }
}

// 显示字符串, x,y:起点坐标, size1:字体大小, *chr:字符串起始地址 
void OLED_ShowString(u8 x,u8 y,char *chr,u8 size1)
{
    while((*chr>=' ')&&(*chr<='~'))//判断是不是非法字符!
    {
        OLED_ShowChar(x,y,*chr,size1);
        x+=size1/2;
        if(x>128-size1)  //换行
        {
            x=0;
            y+=2;
        }
        chr++;
  }
}

// 显示2个数字, x,y :起点坐标, len :数字的位数, size:字体大小
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1)
{
    u8 t,temp;
    for(t=0;t<len;t++)
    {
        temp=(num/OLED_Pow(10,len-t-1))%10;
        if(temp==0)
        {
            OLED_ShowChar(x+(size1/2)*t,y,'0',size1);
    }
        else 
        {
            OLED_ShowChar(x+(size1/2)*t,y,temp+'0',size1);
        }
  }
}

// 显示汉字, x,y:起点坐标, num:汉字对应的序号, 取模方式 列行式
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1)
{
    u8 i,m,n=0,temp,chr1;
    u8 x0=x,y0=y;
    u8 size3=size1/8;
    while(size3--)
    {
        chr1=num*size1/8+n;
        n++;
        for(i=0;i<size1;i++)
        {
            if(size1==16)
                    {temp=Hzk1[chr1][i];}//调用16*16字体
            else if(size1==24)
                    {temp=Hzk2[chr1][i];}//调用24*24字体
            else if(size1==32)       
                    {temp=Hzk3[chr1][i];}//调用32*32字体
            else if(size1==64)
                    {temp=Hzk4[chr1][i];}//调用64*64字体
            else return;
            
            for(m=0;m<8;m++)
            {
                if(temp&0x01)OLED_DrawPoint(x,y);
                else OLED_ClearPoint(x,y);
                temp>>=1;
                y++;
            }
            x++;
            if((x-x0)==size1)
            {x=x0;y0=y0+8;}
            y=y0;
        }
    }
}

// 显示图片, x0,y0：起点坐标, x1,y1：终点坐标, BMP[]：要写入的图片数组
void OLED_ShowPicture(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[])
{
    u32 j=0;
    u8 x=0,y=0;
    if(y%8==0)y=0;
    else y+=1;
    for(y=y0;y<y1;y++)
    {
        OLED_Set_StartAddr(x0,y);
        for(x=x0;x<x1;x++)
        {
            OLED_WR_Byte(BMP[j],OLED_DATA);
            j++;
        }
    }
}

// 屏幕滚动显示, num 显示汉字的个数, space 每一遍显示的间隔
void OLED_ScrollDisplay(u8 num,u8 space)
{
    u8 i,n,t=0,m=0,r;
    while(1)
    {
        if(m==0)
        {
        OLED_ShowChinese(128,24,t,16); //写入一个汉字保存在OLED_GRAM[][]数组中
            t++;
        }
        if(t==num)
            {
                for(r=0;r<16*space;r++)      //显示间隔
                 {
                    for(i=0;i<144;i++)
                        {
                            for(n=0;n<8;n++)
                            {
                                OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
                            }
                        }
            OLED_Refresh();
                    }
        t=0;
        }
        m++;
        if(m==16){m=0;}
        for(i=0;i<144;i++)   //实现左移
        {
            for(n=0;n<8;n++)
            {
                OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
            }
        }
        OLED_Refresh();
    }
}

// m^n
u32 OLED_Pow(u8 m,u8 n)
{
    u32 result=1;
    while(n--)
    {
      result*=m;
    }
    return result;
}

// 配置写入数据的起始位置
void OLED_Set_StartAddr(u8 x,u8 y)
{
    OLED_WR_Byte(0xb0+y,OLED_CMD);//设置行起始地址
    OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
    OLED_WR_Byte((x&0x0f),OLED_CMD);
}


//OLED的初始化
void OLED_Init(void)
{
    OLED_GPIO_Init();
    
    OLED_RST_Clr();
    SysTick_Block_Delay_ms(200);
    OLED_RST_Set();

#if 1 // 按照手册自定义配置指令
/* -------------------- Application Notes Software Initialization Flow Chart : Start. -------------------- */
    /* 关闭屏幕显示 */
    OLED_WR_Byte(0xAE,OLED_CMD); // Set Display OFF/ON (AEh/AFh).
    /* 设置复用比选用端口 */
    OLED_WR_Byte(0xA8,OLED_CMD); // Set Multiplex Ratio (A8h).
    OLED_WR_Byte(0x3F,OLED_CMD); // -> Value (10h~3Fh). -- select use COM0-COM63 in row.
    /* 设置页寻址模式(非必要初始化) */
    OLED_WR_Byte(0x20,OLED_CMD); // Set Memory Addressing Mode (20h).
    OLED_WR_Byte(0x02,OLED_CMD); // -> Mode Value (00h/01h/02h). -- select Page addressing mode (02h).
    /* 设置地址区间循环(非必要初始化) */
    OLED_WR_Byte(0x21,OLED_CMD); // Set Column Address (21h). -- set col/seg 0-127 to loop.
    OLED_WR_Byte(0x00,OLED_CMD); // -> Start Address (00h).
    OLED_WR_Byte(0x7F,OLED_CMD); // -> End Address (7Fh).
    OLED_WR_Byte(0x22,OLED_CMD); // Set Page Address (22h). -- set page 0-7 to loop.
    OLED_WR_Byte(0x00,OLED_CMD); // -> Start Address (00h).
    OLED_WR_Byte(0x07,OLED_CMD); // -> End Address (07h).
    /* 设置页和列的起始地址(非必要初始化) */
    OLED_WR_Byte(0xB0,OLED_CMD); // Set the page start address of the target display location by command B0h to B7h.
    OLED_WR_Byte(0x00,OLED_CMD); // Set the lower start column address of pointer by command 00h to 0Fh. 
    OLED_WR_Byte(0x10,OLED_CMD); // Set the upper start column address of pointer by command 10h to 1Fh.
    /* 设置偏移 */
    OLED_WR_Byte(0xD3,OLED_CMD); // Set Display Offset (D3h).
    OLED_WR_Byte(0x00,OLED_CMD); // -> Value (00h~3Fh). -- no vertical offset in row.
    /* 设置起始行 */
    OLED_WR_Byte(0x40,OLED_CMD); // Set Display Start Line (40h~7Fh). -- map-to row_0.
    /* 设置水平、垂直翻转 */
    OLED_WR_Byte(0xA1,OLED_CMD); // Set Segment Re-map (A0h/A1h). -- Horizontal Inverse / Normal.
    OLED_WR_Byte(0xC8,OLED_CMD); // Set COM Output Scan Direction (C0h/C8h). -- Vertical Inverse / Normal.
    /* 设置引脚硬件配置 */
    OLED_WR_Byte(0xDA,OLED_CMD); // Set COM Pins Hardware Configuration (DAh).
    OLED_WR_Byte(0x12,OLED_CMD); // -> DataSheet Note as (12h). -- no edit.
    /* 设置亮度 */
    OLED_WR_Byte(0x81,OLED_CMD); // Set Contrast Control for BANK0 (81h).
    OLED_WR_Byte(0x7F,OLED_CMD); // -> Value (00h~FFh). -- set for segment output current and brightness.
    /* 设置正常显示内容 */
    OLED_WR_Byte(0xA4,OLED_CMD); // Entire Display ON (A4h/A5h). -- show as GDDRAM contents / entire.
    OLED_WR_Byte(0xA6,OLED_CMD); // Set Normal/Inverse Display (A6h/A7h).
    /* 设置振荡器频率 */
    OLED_WR_Byte(0xD5,OLED_CMD); // Set Display Clock Divide Ratio / Oscillator Frequency (D5h).
    OLED_WR_Byte(0x80,OLED_CMD); // -> Value (80h).
    /* 使能电荷泵调节器 */
    OLED_WR_Byte(0x8D,OLED_CMD); // Set Charge Pump (8D).
    OLED_WR_Byte(0x14,OLED_CMD); // Enable/Disable charge pump regulator (14h/10h).
    /* 打开屏幕显示 */
    OLED_WR_Byte(0xAF,OLED_CMD); // Set Display OFF/ON (AEh/AFh).
/* -------------------- Application Notes Software Initialization Flow Chart : End. -------------------- */
#else // 商家例程配置指令,顺序有调整
    OLED_WR_Byte(0xAE,OLED_CMD);// Set Display ON/OFF (AE)
    OLED_WR_Byte(0xA8,OLED_CMD);// set multiplex ratio (A8), map to line nums.
    OLED_WR_Byte(0x3F,OLED_CMD);// set multiplex ratio as (1 to 64).
    // OLED_WR_Byte(0x20,OLED_CMD);// Set Page Addressing Mode (0x00/0x01/0x02)
    // OLED_WR_Byte(0x02,OLED_CMD);//
    // OLED_WR_Byte(0x00,OLED_CMD);// set low column address.
    // OLED_WR_Byte(0x10,OLED_CMD);// set high column address.
    OLED_WR_Byte(0xD3,OLED_CMD);// set display offset (D3) Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WR_Byte(0x00,OLED_CMD);// not offset
    OLED_WR_Byte(0x40,OLED_CMD);// set start line address. -- (0x40~0x7F) mapp to display start line (0x00~0x3F).
    OLED_WR_Byte(0xA1,OLED_CMD);// Set SEG/Column Mapping.     -- (A0/A1) left-right-Inverse/Normal.
    OLED_WR_Byte(0xC8,OLED_CMD);// Set COM/Row Scan Direction. -- (C0/C8) up-down-Inverse/Normal.
    OLED_WR_Byte(0xDA,OLED_CMD);// set com pins hardware configuration
    OLED_WR_Byte(0x12,OLED_CMD);
    OLED_WR_Byte(0x81,OLED_CMD);// set contrast control.
    OLED_WR_Byte(0xCF,OLED_CMD);// Set SEG Output Current Brightness.
    OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
    OLED_WR_Byte(0xA6,OLED_CMD);// Set Normal/Inverse Display. -- (A6/A7) Normal/Inverse.
    OLED_WR_Byte(0xD5,OLED_CMD);// set display clock divide ratio/oscillator frequency
    OLED_WR_Byte(0x80,OLED_CMD);// set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WR_Byte(0x8D,OLED_CMD);// set Charge Pump enable/disable
    OLED_WR_Byte(0x14,OLED_CMD);// set(0x10) disable

    OLED_WR_Byte(0xD9,OLED_CMD);// set pre-charge period
    OLED_WR_Byte(0xF1,OLED_CMD);// Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WR_Byte(0xDB,OLED_CMD);// set vcomh
    OLED_WR_Byte(0x40,OLED_CMD);// Set VCOM Deselect Level
    
    OLED_WR_Byte(0xAF,OLED_CMD);
    OLED_Clear();
#endif
}


/* Called by main.c */
void OLED_Demo()
{
    // printf("OLED_Demo: into.\n");
    OLED_Init();
    // OLED_ColorTurn(0); // 0正常显示，1反色显示
    // OLED_DisplayTurn(0); // 0正常显示，1屏幕翻转显示
    // OLED_DrawPoint(0,0);
    // OLED_DrawLine(20,0,50,60);
    // OLED_DrawCircle(64,32,20);
    OLED_Refresh();
    // printf("OLED_Demo: exit.\n");
}

void OLED_Process_Loop()
{
    // printf("OLED_Process_Loop: into.\n");

    // 显示图像
    OLED_ShowPicture(0,0,128,8,BMP3);
    SysTick_Block_Delay_ms(500);
    OLED_ShowPicture(0,0,128,8,BMP4);
    SysTick_Block_Delay_ms(500);
    
    // OLED_Clear_RAM(); 
    // OLED_Refresh(); // 加上这个清屏会导致屏幕闪烁黑屏
    OLED_ShowChinese(0,0,0,16); // 赫
    OLED_ShowChinese(16,0,1,16);// 比
    OLED_ShowChinese(32,0,2,16);// 国
    OLED_ShowChinese(48,0,3,16);// 际
    OLED_ShowChinese(64,0,4,16);// 有
    OLED_ShowChinese(80,0,5,16);// 限
    OLED_ShowChinese(96,0,6,16);// 公
    OLED_ShowChinese(112,0,7,16);//司
    OLED_ShowString(16,16,"Hi-P SH Corp",16);
    OLED_ShowString(20,32,"2024/02/04",16);
    OLED_ShowString(0,48,"ASCII:",16);
    OLED_ShowString(63,48,"CODE:",16);

    static unsigned char t = ' ';
    if(t++>'~') t=' ';
    OLED_ShowChar(48,48,t,16);      // 显示ASCII字符
    OLED_ShowNum(103,48,t,3,16);    // 显示ASCII值

    // OLED_Refresh();
    // SysTick_Block_Delay_ms(300);

    // OLED_ScrollDisplay(11,4);

    // printf("OLED_Process_Loop: exit.\n");
}



#ifdef SPI_SOFTWARE

void OLED_WR_Byte(u8 dat,u8 cmd)
{
    u8 i;
    // choose write data or cmd.
    if(cmd) // cmd==OLED_DATA=1
        OLED_DC_Set();
    else // cmd==OLED_CMD=0
        OLED_DC_Clr();
    OLED_CS_Clr();
    for(i=0;i<8;i++)
    {
        OLED_SCLK_Clr();
        if(dat&0x80)
            OLED_SDIN_Set();
        else 
            OLED_SDIN_Clr();
        OLED_SCLK_Set();
        dat<<=1;
    }
    OLED_CS_Set();
    OLED_DC_Set();
}

void OLED_GPIO_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    // 使能A端口时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    // 速度50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_4);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);    // 使能B端口时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    // 速度50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);
}

#elif defined SPI_HARDWARE

void OLED_WR_Byte(uint8_t data, uint8_t data_type)
{
    // Send as DATA.
    if(data_type)
        OLED_DC_Set();
    // Send as CMD.
    else
        OLED_DC_Clr();
    // Send Byte.
    SPI_SendByte(data);
}

void OLED_GPIO_Init(void)
{
    // 使能B端口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // 设置OLED_RST和OLED_DC引脚为输出模式
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    SPI_Config();
    #ifdef SPI_DMA_ENABLE
        SPI_DMA_Config();
    #endif
}
#endif

#ifdef SPI_DMA_ENABLE

void OLED_Refresh(void)
{
    printf("OLED_Refresh: into.\n");
    /* 设置水平寻址模式 */
    OLED_WR_Byte(0x20,OLED_CMD); // Set Memory Addressing Mode (20h).
    OLED_WR_Byte(0x00,OLED_CMD); // -> Mode Value (00h/01h/02h). -- select Horizontal addressing mode (02h).
    /* 设置地址区间循环 */
    OLED_WR_Byte(0x21,OLED_CMD); // Set Column Address (21h). -- set col/seg 0-127 to loop.
    OLED_WR_Byte(0x00,OLED_CMD); // -> Start Address (00h).
    OLED_WR_Byte(0x7F,OLED_CMD); // -> End Address (7Fh).
    OLED_WR_Byte(0x22,OLED_CMD); // Set Page Address (22h). -- set page 0-7 to loop.
    OLED_WR_Byte(0x00,OLED_CMD); // -> Start Address (00h).
    OLED_WR_Byte(0x07,OLED_CMD); // -> End Address (07h).
    // /* 设置起始列 */
    // OLED_WR_Byte(0x00,OLED_CMD); // Set the lower start column address of pointer by command 00h to 0Fh. 
    // OLED_WR_Byte(0x10,OLED_CMD); // Set the upper start column address of pointer by command 10h to 1Fh.
    // /* 设置起始行 */
    // OLED_WR_Byte(0x40,OLED_CMD); // Set Display Start Line (40h~7Fh). -- map-to row_0.
    
    /* Send as OLED_DATA, not OLED_CMD. */
    OLED_DC_Set();
    /* DMA设置 Normal，每Refresh 发送一次整个GDDRAM */
    SPI_DMA_Tx();
    /* 后面，DMA改Circular，取消Refresh，实现改内存自动刷新 */
    
    printf("OLED_Refresh: exit.\n");
}

#else

//更新显存到OLED
void OLED_Refresh(void)
{
    u8 i,n;
    for(i=0;i<8;i++)
    {
        OLED_WR_Byte(0xb0+i,OLED_CMD); //设置行起始地址
        OLED_WR_Byte(0x00,OLED_CMD);   //设置低列起始地址
        OLED_WR_Byte(0x10,OLED_CMD);   //设置高列起始地址
        for(n=0;n<128;n++)
            OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
    }
}

#endif
