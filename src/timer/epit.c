/*********************************************************************
 * 版权所有: Copyright (c) 2020-2021  XXX Company. All rights reserved.
 * 系统名称: 
 * 文件名称: epit.c
 * 内容摘要: 简要描述本文件的内容，包括主要模块、函数及其功能的说明
 * 当前版本: 
 * 作    者: HJWang
 * 设计日期: 2020-09-20 11:01
 * 修改记录: 
 * 日    期          版    本          修改人          修改摘要
**********************************************************************/




/********************************** 标准库头文件 ***********************************/
//#include <xxx.h>


/********************************** 非标准库头文件 ***********************************/
#include "epit.h"


/********************************** 常量定义 ***********************************/


/********************************** 文件内部使用的宏 ***********************************/


/********************************** 文件内部使用的数据类型 ***********************************/


/********************************** 静态全局变量 ***********************************/


/********************************** 全局变量 ***********************************/


/********************************** 局部函数声明 ***********************************/


/********************************** 局部函数 ***********************************/


/********************************** 全局函数 ***********************************/


/********************************** 类的实现 ***********************************/

EPIT_Type EPIT1;

/***********************************************************************
 * 函数名称: EPIT_Init
 * 作    者: HJWang
 * 设计日期: 2020-09-20 13:56
 * 头 文 件: 
 * 功能描述:初始化EPIT定时器，时钟源使用 ipg = 66M
 * 参    数:  frac:预分频值，value：到计数值。 
 * 返 回 值: 
 * 注    释: 
 * 是否依赖: 
 * 被调函数: 
 * 被调描述: 
 * 修改日期:          修改人          修改内容
 ***********************************************************************/
void EPIT_Init(unsigned int frac, unsigned int value)
{
	if (frac > 0xFFF)
		frac = 0xFFF;
	EPIT1->CR = 0;					/*先清除CR寄存器*/

	/*
		CR寄存器：
		bit[25:24]:01 选择时钟源为 Peripheral clock = 66M
		bit[15:4]: 4 frac分频值
		bit[3:1]:1 当计数器到0的话从LR重新加载数值
		bit[2:1]：1 比较中断使能
		bit1: 1 初始计数值来源LR寄存器
		bit0: 0 先关闭EPIT1
	*/
	EPIT1->CR = (1 << 24 | frac << 4 | 1 << 3 | 1 << 2 | 1 << 1);
	EPIT1->LR = value;		/*加载寄存器值*/
	EPIT1->CMPR = 0;		/*比较寄存器值*/

	/*使能GIC中对应的中断*/
	GIC_EnableIRQ(EPIT1_IRQn);

	/*注册中断服务函数*/
	system_register_irqhandler(EPIT1_IRQn, (system_irq_handler_t) epit1_irqhandler, NULL);

	EPIT1->CR |= 1 << 0;		/*使能EPIT1*/
}

void epit1_irqhandler(void)
{
	static unsigned char state = 0;

	if (EPIT1->SR & (1 << 0))		/*判断比较事件发生*/
	{
		
	}
	EPIT1->SR |= 1 << 0;			/*清除中断标志位*/
	
}
void EPIT_StartUp(void)
{

}
void EPIT_Close(void)
{
	
}


