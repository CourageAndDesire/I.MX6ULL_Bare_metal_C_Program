/*********************************************************************
 * 版权所有: Copyright (c) 2020-2021  XXX Company. All rights reserved.
 * 系统名称: 
 * 文件名称: uart.c
 * 内容摘要: 简要描述本文件的内容，包括主要模块、函数及其功能的说明
 * 当前版本: 
 * 作    者: HJWang
 * 设计日期: 2020-09-26 03:08
 * 修改记录: 
 * 日    期          版    本          修改人          修改摘要
**********************************************************************/




/********************************** 标准库头文件 ***********************************/
#include <xxx.h>


/********************************** 非标准库头文件 ***********************************/
#include "xxx.h"


/********************************** 常量定义 ***********************************/


/********************************** 文件内部使用的宏 ***********************************/


/********************************** 文件内部使用的数据类型 ***********************************/


/********************************** 静态全局变量 ***********************************/


/********************************** 全局变量 ***********************************/


/********************************** 局部函数声明 ***********************************/


/********************************** 局部函数 ***********************************/


/********************************** 全局函数 ***********************************/


/********************************** 类的实现 ***********************************/

 UART_Type UART1;
void uart_Init(void)
{
	/*初始化串口IO*/
	uart_io_Init();

	/*初始化UART1*/
	uart_disable(UART1);			/*先关闭UART1*/
	uart_softreset(UART1);			/*软件复位UART1*/

	UART1->UCR1 = 0;				/*先清除UCR1寄存器*/
	UART1->UCR1 &=	~(1 << 14);		/*关闭自动波特率检测*/

	/*
		设置UART的UCR2寄存器，设置字长、停止位、校验模式、关闭硬件流控
		bit14: 1 忽略RTS引脚
		bit8: 0 关闭奇偶检验
		bit6: 0 1位停止位
		bit5: 1 8位数据位
		bit2: 1 打开发送
		bit1：1 打开接收
	*/
	UART1->UCR2 |= (1 << 14) | (1 << 5) | (1 << 2) | (1 << 1);
	UART1->UCR3 |= 1 << 2;						/*UCR3的bit2 必须为1*/

	/*
		设置波特率
		波特率计算公式:Baud Rate = Ref Freq / (16 * (UBMR + 1)/(UBIR+1))
		115200 = 80000000（80M） / （16 * （3124 + ·） / （71 + 1））
	*/
	UART1->UFCR = 5 << 7;		/*ref freq 等于 ipg_clk /1 = 80M*/
	UART1->UBIR = 71;
	UART1->UBMR = 3124;

#if 0
	uart_setbaudrate(UART1, 115200, 80000000);
#endif

	uart_enable(UART1);		/*使能串口*/
}

void uart_io_Init(void)
{
	/*	
		初始化串口IO
		UART1_RXD->UART1_TX_DATA
		UART1_TXD->UART1_RX_DATA
	*/
	IOMUXC_SetPinMux(IOMUXC_UART1_TX_DATA_UART1_TX, 0);
	IOMUXC_SetPinMux(IOMUXC_UART1_TX_DATA_UART1_RX, 0);
	IOMUXC_SetPinConfig(IOMUXC_UART1_TX_DATA_UART1_TX, 0x10B0);
	IOMUXC_SetPinConfig(IOMUXC_UART1_RX_DATA_UART1_RX, 0x10B0);
	
}

void uart_setbaudrate(UART_Type * base, unsigned int baudrate, unsigned int srcclock_hz)
{

}

void uart_disable(UART_Type * base)
{
	base->UCR1 &= ~(1 << 0);
}

void uart_enable(UART_Type * base)
{
	base->UCR1 |= (1 << 0);
}

void uart_softreset(UART_Type * base)
{
	base->UCR2 &= ~(1 << 0);				/*复位UART*/
	while((base->UCR2 & 0x01) == 0);		/*等待复位完成*/
}

void putc(unsigned char c)
{
	while(((UART1->USR2 >> 3) & 0x01) == 0);			/*等待上一次发送完成*/
	UART1->UTXD		= c & 0xFF;							/*发送数据*/
}

void puts(char * str)
{
	char *p = str;
	while (*p)
		putc(*p++);
}

unsigned char getc(void)
{
	while((UART1->USR2 & 0xx1) == 0);					/*等待接收完成*/
	return UART1->URXD;									/*返回接收到的数据*/
}

void raise(int sig_nr)
{

}