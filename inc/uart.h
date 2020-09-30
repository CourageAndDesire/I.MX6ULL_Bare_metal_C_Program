/*********************************************************************
 * 版权所有: Copyright (c) 2020-2021  XXX Company. All rights reserved.
 * 系统名称: 
 * 文件名称: uart.h
 * 内容摘要: 
 			UART的配置步骤：1.设置UART的时钟源    				The "Module Clock" is the UART_CLK which comes from CCM. The "PeripheralClock" is the IPG_CLK which comes from CCM
 													设置为 01 IrDA (Interface)模式
 							2.初始化UART 
 							3.使能UART        
 							4.编写UART数据收发函数
 * 当前版本: 
 * 作    者: HJWang
 * 设计日期: 2020-09-26 03:09
 * 修改记录: 
 * 日    期          版    本          修改人          修改摘要
**********************************************************************/




#ifndef __UART_H__
#define __UART_H__
/********************************** 其它条件编译选项 ***********************************/


/********************************** 标准库头文件 ***********************************/
//#include <xxx.h>


/********************************** 非标准库头文件 ***********************************/
//#include ".h"
#include "imx6ul.h"


/********************************** 常量定义 ***********************************/


/********************************** 全局宏 ***********************************/


/********************************** 数据类型 ***********************************/


/********************************** 函数声明 ***********************************/


/********************************** 类定义 ***********************************/


/********************************** 模板 ***********************************/


void uart_Init(void);
void uart_io_Init(void);
void uart_disable(UART_Type *base);
void uart_enable(UART_Type *base);
void uart_softreset(UART_Type *base);
void uart_setbaudrate(UART_Type *base, unsigned int baudrate, unsigned int srcclock_hz);
void putc(unsigned char c);
void puts(char *str);
unsigned char getc(void);
void raise(int sig_nr);


#endif /* __UART_H__ */


