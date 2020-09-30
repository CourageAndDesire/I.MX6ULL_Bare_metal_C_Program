/*********************************************************************
 * 版权所有: Copyright (c) 2020-2021  XXX Company. All rights reserved.
 * 系统名称: 
 * 文件名称: int.c
 * 内容摘要: 简要描述本文件的内容，包括主要模块、函数及其功能的说明
 * 当前版本: 
 * 作    者: HJWang
 * 设计日期: 2020-09-19 20:50
 * 修改记录: 
 * 日    期          版    本          修改人          修改摘要
**********************************************************************/




/********************************** 标准库头文件 ***********************************/
//#include <xxx.h>


/********************************** 非标准库头文件 ***********************************/
#include "int.h"


/********************************** 常量定义 ***********************************/


/********************************** 文件内部使用的宏 ***********************************/


/********************************** 文件内部使用的数据类型 ***********************************/


/********************************** 静态全局变量 ***********************************/


/********************************** 全局变量 ***********************************/


/********************************** 局部函数声明 ***********************************/


/********************************** 局部函数 ***********************************/


/********************************** 全局函数 ***********************************/


/********************************** 类的实现 ***********************************/
static unsigned int irqNesting;
static sys_irq_handle_t irqTable[NUMBER_OF_INT_VECTORS];
void int_Init(void);
{
	/*初始化GIC*/
	GIC_Init();
	/*初始化中断向量表*/
	system_irqtable_init();
	/*中断向量表偏移*/
	__set_VBAR((uint32_t)0x87800000);
}
void system_irqtable_init(void)
{
	unsigned int i = 0;
	
	irqNesting = 0;
	/*先将所有的中断服务函数设置为默认值*/
	for (i = 0; i < NUMBER_OF_INT_VECTORS; i++)
	{
		system_register_irqhandler((IRQn_Type) i,
									default_irqhandler,
									NULL);
	}
}

/*C语言中断服务函数，irq汇编中中断服务含税会调用此函数，此函通过在中断服务列表中查找指定中断号所对应的中断处理函数并执行*/
void system_register_irqhandler(IRQn_Type irq,
								system_irq_handler_t handler,
								void *userParam)
{
	irqTable[irq].irqHandler = handler;
	irqTable[irq].userParam = userParam;
}

/*C语言中断服务函数，irq汇编中中断服务含税会调用此函数，此函通过在中断服务列表中查找指定中断号所对应的中断处理函数并执行*/
void system_irqhandler(unsigned int giccIar)
{
	uint32_t intNum = giccIar & 0x3FFUL;

	/*检查中断号是否符合要求*/
	if ((intNum == 1020) || (intNum >= NUMBER_OF_INT_VECTORS))
	{
		return;
	}
	
	irqNesting++;		/*中断嵌套计数器加1*/

	/*根据传递进来的中断号，在irqtable中调用确定的中断服务函数*/
	irqTable[intNum].irqHandler(intNum,irqTable[intNum].userParam);

	irqNesting--;		/*中断执行完成，中断嵌套寄存器减1*/
	
}
void default_irqhandler(unsigned int giccIar, void *userParam)
{
	while(1)
	{

	}
}


