/*********************************************************************
 * 版权所有: Copyright (c) 2020-2021  XXX Company. All rights reserved.
 * 系统名称: 
 * 文件名称: main.h
 * 内容摘要: 裸机程序的 main源文件的头文件
 * 当前版本: 
 * 作    者: HJWang
 * 设计日期: 2020-09-05 18:13
 * 修改记录: 
 * 日    期          版    本          修改人          修改摘要
**********************************************************************/




#ifndef __MAIN_H__
#define __MIAN_H__


#include "led.h"
#include "MCIMX6Y2.h"
#include "clk.h"

/********************************** 其它条件编译选项 ***********************************/
//确定LED、beep、key所需要使用的寄存器地址
/*
	1。时钟。不过因为 I.MX6ULL在bin文件头部进行了数据添加，其中，就有初始化时钟，所以不用
	2.SW_MUX	复用寄存器
	3.SW_PAD	配置寄存器
	4.GPIO操作寄存器			在这里主要是对电平进行操作，所以主要关注的寄存器有 GDIR(设置方向),DR（设置高低电平）
*/

/*时钟*/
//#define 	CCM_CCGR0	(*(volatile unsigned int *)0x020c4068)
//#define 	CCM_CCGR1	(*(volatile unsigned int *)0x020c406C)
//#define 	CCM_CCGR2	(*(volatile unsigned int *)0x020c4070)
//#define 	CCM_CCGR3	(*(volatile unsigned int *)0x020c4074)
//#define 	CCM_CCGR4	(*(volatile unsigned int *)0x020c4078)
//#define 	CCM_CCGR5	(*(volatile unsigned int *)0x020c407C)
//#define 	CCM_CCGR6	(*(volatile unsigned int *)0x020c4080)

//#define		CCM_base			(0x020c4068)

//typedef struct{
//	__IO uint32_t CCM_CCGR01;
//	__IO uint32_t CCM_CCGR11;
//	__IO uint32_t CCM_CCGR21;
//	__IO uint32_t CCM_CCGR31;
//	__IO uint32_t CCM_CCGR41;
//	__IO uint32_t CCM_CCGR51;
//	//volatile unsigned int CCM_CCGR6;
//}typexx;

//#define		ccmx				((typexx *)CCM_base)

/*LED寄存器*/
#define 	IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03	(*(volatile unsigned int *)0x020c4068)
#define 	IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO03	(*(volatile unsigned int *)0x020e02f4)
#define 	GPIO1_GDIR	(*(volatile unsigned int *)0x0209c004)
#define 	GPIO1_DR	(*(volatile unsigned int *)0x0209c000)



/********************************** 标准库头文件 ***********************************/



/********************************** 非标准库头文件 ***********************************/


/********************************** 常量定义 ***********************************/


/********************************** 全局宏 ***********************************/


/********************************** 数据类型 ***********************************/


/********************************** 函数声明 ***********************************/


/********************************** 类定义 ***********************************/


/********************************** 模板 ***********************************/


#endif /* __MAIN_H__ */


