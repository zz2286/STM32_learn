/* 本文件用于添加寄存器地址及结构体定义, Encode in UTF-8 */
#ifndef __STM32F10X_H
#define __STM32F10X_H

/* 片上外设基地址  */
#define PERIPH_BASE				((unsigned int)0x40000000)
/* APB1 总线基地址 */
#define  APB1PERIPH_BASE		PERIPH_BASE
/* APB2 总线基地址, GPIOx挂载在APB2总线上 */
#define APB2PERIPH_BASE			(PERIPH_BASE + 0x10000)
/* AHB 总线基地址 */
#define AHBPERIPH_BASE			(PERIPH_BASE + 0x20000)


/* RCC 外设基地址 */
#define RCC_BASE      		(AHBPERIPH_BASE + 0x1000)
/* RCC 的 APB2 时钟使能寄存器地址,强制转换成指针 */
#define RCC_APB2ENR			*(unsigned int*)(RCC_BASE+0x18)
	

/* GPIOx 外设基地址 */
#define GPIOA_BASE				(APB2PERIPH_BASE + 0x0800)
#define GPIOB_BASE				(APB2PERIPH_BASE + 0x0C00)
#define GPIOC_BASE				(APB2PERIPH_BASE + 0x1000)

/* GPIOA 寄存器地址,强制转换成指针 */
#define GPIOA_CRL			*(unsigned int*)(GPIOA_BASE+0x00)
#define GPIOA_CRH			*(unsigned int*)(GPIOA_BASE+0x04)
#define GPIOA_IDR			*(unsigned int*)(GPIOA_BASE+0x08)
#define GPIOA_ODR			*(unsigned int*)(GPIOA_BASE+0x0C)
#define GPIOA_BSRR			*(unsigned int*)(GPIOA_BASE+0x10)
#define GPIOA_BRR			*(unsigned int*)(GPIOA_BASE+0x14)
#define GPIOA_LCKR			*(unsigned int*)(GPIOA_BASE+0x18)
/* GPIOB 寄存器地址,强制转换成指针 */
#define GPIOB_CRL			*(unsigned int*)(GPIOB_BASE+0x00)
#define GPIOB_CRH			*(unsigned int*)(GPIOB_BASE+0x04)
#define GPIOB_IDR			*(unsigned int*)(GPIOB_BASE+0x08)
#define GPIOB_ODR			*(unsigned int*)(GPIOB_BASE+0x0C)
#define GPIOB_BSRR			*(unsigned int*)(GPIOB_BASE+0x10)
#define GPIOB_BRR			*(unsigned int*)(GPIOB_BASE+0x14)
#define GPIOB_LCKR			*(unsigned int*)(GPIOB_BASE+0x18)
/* GPIOC 寄存器地址,强制转换成指针 */
#define GPIOC_CRL			*(unsigned int*)(GPIOC_BASE+0x00)
#define GPIOC_CRH			*(unsigned int*)(GPIOC_BASE+0x04)
#define GPIOC_IDR			*(unsigned int*)(GPIOC_BASE+0x08)
#define GPIOC_ODR			*(unsigned int*)(GPIOC_BASE+0x0C)
#define GPIOC_BSRR	  		*(unsigned int*)(GPIOC_BASE+0x10)
#define GPIOC_BRR			*(unsigned int*)(GPIOC_BASE+0x14)
#define GPIOC_LCKR			*(unsigned int*)(GPIOC_BASE+0x18)

// 寄存器的值常常是芯片外设自动更改的，即使 CPU 没有执行程序，也有可能发生变化
// 编译器有可能会对没有执行程序的变量进行优化，所以用 volatile 来修饰寄存器变量
/* volatile 表示易变的变量，__IO 防止编译器优 */
#define __IO volatile
typedef unsigned int      uint32_t;
typedef unsigned short    uint16_t;

/* GPIO 寄存器结构体定义 */
typedef struct
{
	__IO uint32_t CRL;
	__IO uint32_t CRH;
	__IO uint32_t IDR;
	__IO uint32_t ODR;
	__IO uint32_t BSRR;
	__IO uint32_t BRR;
	__IO uint32_t LCKR;
}GPIO_TypeDef;


typedef struct
{
	uint32_t CR;
	uint32_t CFGR;
	uint32_t CIR;
	uint32_t APB2RSTR;
	uint32_t APB1RSTR;
	uint32_t AHBENR;
	uint32_t APB2ENR;
	uint32_t APB1ENR;
	uint32_t BDCR;
	uint32_t CSR;
}RCC_TypeDef;

#define GPIOA   ((GPIO_TypeDef*)GPIOA_BASE)
#define GPIOB   ((GPIO_TypeDef*)GPIOB_BASE)
#define GPIOC   ((GPIO_TypeDef*)GPIOC_BASE)
#define RCC     ((RCC_TypeDef*)RCC_BASE)

#endif /* __STM32F10X_H */