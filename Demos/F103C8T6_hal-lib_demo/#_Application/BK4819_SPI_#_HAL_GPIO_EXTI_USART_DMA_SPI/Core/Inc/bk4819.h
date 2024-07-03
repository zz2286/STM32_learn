#ifndef __BK4819_H__
#define __BK4819_H__

/* ============================== Includes ============================== */

#include "main.h"
#include "usart.h"

/* ============================== Defines ============================== */

/* BK4819 Registers' Value in hex. */

#define REG_37  0x1D00
#define REG_48  0x3000

/* ============================== Functions ============================== */

void AAA_Delay_NOP(uint32_t nops);

/* GPIO mode setup. */

void AAA_SPI_SDA_INPUT(void);
void AAA_SPI_SDA_OUTPUT(void);

/* GPIO operation. */

void SPI_SCN_SET(void);
void SPI_SCN_CLR(void);
void SPI_SCK_SET(void);
void SPI_SCK_CLR(void);
void SPI_SDA_SET(void);
void SPI_SDA_CLR(void);
GPIO_PinState SPI_SDA_READ(void);

/* ---------- 3-wire SPI Control Interface. ---------- */

void AAA_SPI_Send_Byte(uint8_t byte);
uint16_t AAA_SPI_Get_Word(void);
void AAA_RF_Write(uint8_t addr, uint16_t data);
uint16_t AAA_RF_Read(uint8_t addr);

void AAA_RF_SetGpioOut(uint8_t num, uint8_t type, uint8_t val);
uint8_t AAA_RF_GetGpioIn(uint8_t num);

void AAA_RF_Main(void);
void AAA_RF_Loop(void);
void AAA_RF_Process(uint32_t tick_interval);


#endif /* __BK4819_H__ */
