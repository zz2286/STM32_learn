#include "bk4819.h"

/* ============================== Variables ============================== */

uint8_t flag_RF_Loop_Process = 0;

/* ============================== Functions ============================== */

/**
 * @brief Delay in __NOP().
 * @param nops how many times to do __NOP();
 */
void AAA_Delay_NOP(uint32_t nops)
{
  while (nops--)
  {
    __NOP();
  }
}

/**
 * @brief GPIO Mode Configuration for STM32 PA7 as SPI_SDA.
 * @note Configure PA7 in input mode with pull-up.
 */
void AAA_SPI_SDA_INPUT(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
 * @brief GPIO Mode Configuration for STM32 PA7 as SPI_SDA.
 * @note Configure PA7 in output push-pull mode with pull-down.
 */
void AAA_SPI_SDA_OUTPUT(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
}

/* SPI_SCN - PA4 - Output high and delay nops. */
void SPI_SCN_SET(void) {HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);AAA_Delay_NOP(10);}
/* SPI_SCN - PA4 - Output low and delay nops. */
void SPI_SCN_CLR(void) {HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);AAA_Delay_NOP(10);}
/* SPI_SCK - PA5 - Output high and delay nops. */
void SPI_SCK_SET(void) {HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);AAA_Delay_NOP(10);}
/* SPI_SCK - PA5 - Output low and delay nops. */
void SPI_SCK_CLR(void) {HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);AAA_Delay_NOP(10);}
/* SPI_SDA - PA7 - Output high and delay nops. */
void SPI_SDA_SET(void) {HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);AAA_Delay_NOP(10);}
/* SPI_SDA - PA7 - Output low and delay nops. */
void SPI_SDA_CLR(void) {HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET);AAA_Delay_NOP(10);}
/* SPI_SDA - PA7 - Get Input state GPIO_PIN_RESET or GPIO_PIN_SET. */
GPIO_PinState SPI_SDA_READ(void) {return HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7);}

/**
 * @brief Send 8-bit data byte.
 * @param byte a 8-bit data byte to be sent.
 */
void AAA_SPI_Send_Byte(uint8_t byte)
{
  uint8_t temp = byte;

  for (uint8_t i=0;i<8;i++)
  {
    SPI_SCK_CLR();

    if (temp & 0x80)
    {
      SPI_SDA_SET();
    }
    else
    {
      SPI_SDA_CLR();
    }

    SPI_SCK_SET();

    temp <<= 1;
  }
}

/**
 * @brief Get 16-bit data word.
 * @retval a received 16-bit data word.
 */
uint16_t AAA_SPI_Get_Word(void)
{
  uint16_t data = 0;

  AAA_SPI_SDA_INPUT();
  SPI_SCK_CLR();

  for (uint8_t i=0;i<16;i++)
  {
    data <<= 1;

    SPI_SCK_SET();
    
    if (SPI_SDA_READ())
    {
      data |= 1;
    }

    SPI_SCK_CLR();
  }

  return data;
}

/**
 * @brief write to change BK4819 register's value.
 * @param addr register address.
 * @param data new value write into register.
 */
void AAA_RF_Write(uint8_t addr, uint16_t data)
{
  uint16_t temp = data >> 8;

  AAA_SPI_SDA_OUTPUT();
  SPI_SCK_CLR();
  SPI_SCN_CLR();

  AAA_SPI_Send_Byte(addr);
  AAA_SPI_Send_Byte(temp&0xff);
  AAA_SPI_Send_Byte(data&0xff);

  SPI_SCN_SET();
}

/**
 * @brief Read to get BK4819 register's value.
 * @param addr register address.
 * @retval 16-bit data word in the target address register.
 */
uint16_t AAA_RF_Read(uint8_t addr)
{
  uint16_t data = 0;

  AAA_SPI_SDA_OUTPUT();
  SPI_SCK_CLR();
  SPI_SCN_CLR();

  AAA_SPI_Send_Byte(addr | 0x80);

  AAA_Delay_NOP(10);

  data = AAA_SPI_Get_Word();

  SPI_SCN_SET();

  return data;
}

/** AAA_RF_SetGpioOut(num,type,val) additional description.
 * [GPIOx Output Type Selection]: 
 * type=0 -> High/Low
 * type=1 -> Interrupt
 * type=2 -> Squelch
 * type=3 -> VoX
 * type=4 -> Subau Result
 * type=5 -> CTCSS Result
 * type=6 -> CDCSS Result
 * type=7 -> Tail
 * type=8 -> DTMF/5Tone
 * type=9 -> CTCSS/CDCSS Wave
 * [e.g.]
 * Set GPIO6 = High:             RF_SetGpioOut(6,0,1);
 * Set GPIO2 = CTCSS/CDCSS Wave: RF_SetGpioOut(2,9,0);
 * Set GPIO5 = Squelch:          RF_SetGpioOut(5,2,0);
 */

/**
 * @brief Configure BK4819_GPIOx output mode.
 * @param num number x range 0 ~ 6 of BK4819_GPIOx.
 * @param type for BK4819_GPIOx Output Type Selection.
 * @param val configuration value.
 * @note [e.g.] Set GPIOx output low/high: RF_SetGpioOut(x,0,0/1);
 */
void AAA_RF_SetGpioOut(uint8_t num, uint8_t type, uint8_t val)
{
  uint16_t rdata;

  /* Enable GPIOx Output. */
  rdata = AAA_RF_Read(0x33);
  rdata &= ~(0x4040>>num);  /* GPIOx Output Enable & Value Low. */
  rdata |= (val<<6)>>num;   /* GPIOx Output Value High/Low. */
  AAA_RF_Write(0x33,rdata);

  /* GPIOx Output Type Selection. */
  switch(num)
  {
 		case 0: 
 			rdata = AAA_RF_Read(0x35);
			rdata &= 0xf0ff;  /* REG_35<11:8> GPIO0. */
			rdata |= type<<8;
			AAA_RF_Write(0x35,rdata);
 			break;
			
		case 1: 
			rdata = AAA_RF_Read(0x35);
			rdata &= 0xff0f;  /* REG_35<7:4> GPIO1. */
			rdata |= type<<4;
			AAA_RF_Write(0x35,rdata);
			break;

		case 2: 
			rdata = AAA_RF_Read(0x35);
			rdata &= 0xfff0;
			rdata |= type;
			AAA_RF_Write(0x35,rdata);
			break;

		case 3: 
 			rdata = AAA_RF_Read(0x34);
			rdata &= 0x0fff;
			rdata |= type<<12;
			AAA_RF_Write(0x34,rdata);
			break;

		case 4: 
			rdata = AAA_RF_Read(0x34);
			rdata &= 0xf0ff;
			rdata |= type<<8;
			AAA_RF_Write(0x34,rdata);
			break;
			
		case 5: 
			rdata = AAA_RF_Read(0x34);
			rdata &= 0xff0f;
			rdata |= type<<4;
			AAA_RF_Write(0x34,rdata);
			break;
			
		case 6: 
			rdata = AAA_RF_Read(0x34);
			rdata &= 0xfff0;
			rdata |= type;
			AAA_RF_Write(0x34,rdata);
			break;
  }
}

/**
 * @brief Read to get BK4819_GPIOx input state.
 * @param num number x range 0 ~ 6 of BK4819_GPIOx.
 * @retval GPIOx input state 0-low or 1-high.
 */
uint8_t AAA_RF_GetGpioIn(uint8_t num)
{
  uint16_t rdata;

  rdata = AAA_RF_Read(0x33);
  /* Disable GPIOx Output. */
  rdata |= (0x4000>>num);
  AAA_RF_Write(0x33,rdata);
  
  /* Read GPIO Input Indicator, REG_0A read only. */
  rdata = AAA_RF_Read(0x0A);

  if (rdata & (0x40>>num))
    return 1;
  else
    return 0;
}


/**
 * @brief RF BK4819 Reset, PowerUp and custom Setup.
 * @note Function called inner main() and before while(1) loop. 
 */
void AAA_RF_Main(void)
{
  /* Soft Reset. REG_00<15>: 1=Reset, 0=Normal. */
  AAA_RF_Write(0x00, 0x8000);
  AAA_RF_Write(0x00, 0x0000);

  /* Power Up. REG_37<7:0>: ANA/VCO/RF/PLL LDO Bypass, DSP/XTAL/Band-Gap Enable. */
  AAA_RF_Write(0x37, REG_37 | 0x0F);

  #if 0
  // Set AGC Table
  RF_SetAgc(0);

  // Set MIC Sensitivity
  RF_Write(0x7D,MIC_GAIN | REG_7D); //bit[4:0]=MicSens灵敏�???

  // Set Volume 
  RF_Write(0x48,REG_48 | VOL_GAIN<<4 | DAC_GAIN); //bit[9:4]=音量数字增益；bit[3:0]音量模拟增益

  // Others Setting
  RF_Write(0x1F,0x5454); 	// 2020.9.5 set rfpll_regvco_vbit=0101
  RF_Write(0x1F,0x1454);	// 2020.8.24 RF_Write(0x1F,0x1858);

  RF_Write(0x25,0xC1BA);
  RF_Write(0x77,0x88EF);

  RF_Write(0x3E,0xA037); //v3版本芯片必须修改！！�???

  //以下两组设置可以提高1M Block 1~2dB，但接收灵敏度会变差1~2dB
  RF_Write(0x13,0x037E);
  RF_Write(0x7D,MIC_GAIN | 0xE340);

  RF_SetTxAu(0);
  #endif

}

/**
 * @brief to trigger RF_Process in polling.
 * @note Function called inner while(1) loop.
 */
void AAA_RF_Loop(void)
{
  if (flag_RF_Loop_Process)
  {
    AAA_RF_Process(2000);
  }
}

/**
 * @brief RF process.
 * @param tick_interval the trigger interval of systick in ms.
 * @note triggered by RF_Loop in polling.
 */
void AAA_RF_Process(uint32_t tick_interval)
{
  if (tick_interval != 0)
  {
    static uint32_t tick_counter = 0;
    if ((HAL_GetTick() - tick_counter) < tick_interval) return;
    tick_counter = HAL_GetTick();
  }

  if (AAA_RF_GetGpioIn(5))
  {
    printf("Get GPIO_5 High.\n");
    AAA_RF_SetGpioOut(6,0,1);
    printf("Set GPIO_6 High.\n");
  }
  else
  {
    printf("Get GPIO_5 Low.\n");
    AAA_RF_SetGpioOut(6,0,0);
    printf("Set GPIO_6 low.\n");
  }
}



