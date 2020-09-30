/*********************************************************************
 * 版权所有: Copyright (c) 2020-2021  XXX Company. All rights reserved.
 * 系统名称: 
 * 文件名称: start.s
 * 内容摘要: 运行C语言环境的启动汇编文件。完成一些初始化的操作。1.设置SVC模式（特权模式）。2.设置堆栈
 * 当前版本: 
 * 作    者: HJWang
 * 设计日期: 2020-09-05 17:05
 * 修改记录: 
 * 日    期          版    本          修改人          修改摘要
**********************************************************************/
	/*@汇编程序的默认入口标号是_start,不过我们也可以在链接脚本中使用ENTRY来指明其他的入口点。
	@.global ：用于在gcc编译器中声明汇编的全局标号。
	@_start :表明程序执行的入口。*/

.global	_start

_start:
	ldr	pc,	=Reset_Handler
	ldr	pc,	=Undefined_Handler
	ldr	pc, =SVC_Handler
	ldr pc, =PreAbort_Handler
	ldr pc, =DataAbort_Handler
	ldr pc, =NotUsed_Handler
	ldr pc, =IRQ_Handler
	ldr pc, =FIQ_Handler

Reset_Handler:
	/*关闭全局中断*/
	cpsid	i
	cpsid	f
	
	/*关闭 D cache、I cache、 MMU*/
	mrc p15, 0, r0, c1, c0, 0		/*读取CP15的c1寄存器到r0中*/
	bic	r0,	r0, #(0x1 << 12)		/*清除C1的I位，关闭 I cache*/	
	bic	r0,	r0, #(0x1 << 2)			/*清除C1的C位， 关闭D cache*/
	bic	r0,	r0, #0x2				/*清除C1的A位，关闭 对齐检查*/
	bic	r0,	r0, #(0x1 << 11)		/*清除C1的Z位，	关闭分支预测*/
	bic	r0,	r0, #0x01				/*清除C1的M位，关闭MMU*/
	mcr	p15, 0, r0, c1, c0, 0		/*将r0 的值写入到CP15的C1中*/

	
	/*设置中断向量表，重定位*/
	ldr		r0, =0x87800000

	dsb
	isb
	mcr	p15, 0, r0,c12, c0, 0		/*VBAR寄存器，也就是向量表基地址寄存器*/
	dsb
	isb
	/*
		设置各个模式的堆栈，为C语言环境做准备
	*/
	/*IRQ*/
	mrs	r0,	cpsr				/*[r0] = [cpsr]*/
	bic r0,r0, #0x1f				/*[r0] = [r0] & 0xffffffE0*/
	orr	r0,r0, #0x12				/*设置为IRQ模式*/
	msr	cpsr, r0				/*[cpsr] = [r0]*/

	ldr sp, =0x80600000			/*设置堆栈，栈顶为0x80600000 大小为2MB*/

	/*SYS*/
	mrs	r0,	cpsr				/*[r0] = [cpsr]*/
	bic r0,r0, #0x1f				/*[r0] = [r0] & 0xffffffE0*/
	orr	r0,r0, #0x1f				/*设置为IRQ模式*/
	msr	cpsr, r0				/*[cpsr] = [r0]*/

	ldr sp, =0x80400000			/*设置堆栈，栈顶为0x80600000 大小为2MB*/
	
	/*SVC*/
	mrs	r0,	cpsr				/*[r0] = [cpsr]*/
	bic r0,r0, #0x1f				/*[r0] = [r0] & 0xffffffE0*/
	orr	r0,r0, #0x13				/*设置为IRQ模式*/
	msr	cpsr, r0				/*[cpsr] = [r0]*/

	ldr sp, =0x80200000			/*设置堆栈，栈顶为0x80600000 大小为2MB*/

	/*打开全局中断*/
	cpsie	i
	cpsie	f
	
	/*跳转到main*/
	b	main
	
Undefined_Handler:
	ldr r0, =Undefined_Handler
	bx r0

SVC_Handler:
	ldr r0, =SVC_Handler
	bx r0
PreAbort_Handler:
	ldr r0, =PreAbort_Handler
	bx r0
DataAbort_Handler:
	ldr r0, =DataAbort_Handler
	bx r0
NotUsed_Handler:
	ldr r0, =NotUsed_Handler
	bx r0
IRQ_Handler:
	/*保存链接地址*/
	push {lr}			/*入栈，保存lr的地址，这里保存的是 SVC模式下的 下一条地址。因为进入mian里面的模式是 SVC模式*/
	/*保存寄存器*/
	push {r0-r3,r12}	/*保存 r0到r3以及r12寄存器，因为在中断发生的时候， r4~r11编译器会自动保存*/	
	/**保存spsr*/
	mrs	r0,	spsr		/*这里保存的是SCV模式下的 CPSR的状态*/
	push {r0}

	/*IRQ 中断的处理，找到哪个外设发生了中断*/

	mrc	p15,4,r1,c15,c0,0	/*读取 CBAR寄存器的值，CBAR保存着GIC寄存器的基础物理地址*/
	add	r1,r1,#0x2000		/*得到CPU接口端基地址，是在 GIC寄存器基地址偏移 0x2000*/
	ldr r0, [r1,#0x000C]	/*找到GICC_IAR寄存器，从中获取 发生中断的 CPUID 和 中断ID*/

	push {r0,r1}			/*保存 获取 发生中断的 CPUID 和 中断ID 和 CPU接口端基地址*/ /*这样做的原因是为了允许中断嵌套，当发生其他的中断之后，这些数据要保留*/

	/*进入SVC 模式，允许其他的IRQ中断进来，进行中断嵌套*/
	cps	#0x13	
	push {lr}				/*保存IRQ的lr地址*/
	/*跳转到中断服务函数进行处理*/
	ldr r2, =system_irqhandler	/*加载中断服务函数到r2中*/
	blx r2						/*执行中断服务函数，带一个R0参数*/

	/*IRQ中断执行完毕*/
	pop {lr}					/*执行完中断服务函数，进行出栈*/
	cps	#0x12					/*进入到 IRQ模式*/
	pop {r0,r1}
	str r0, [r1,#0x10]							/*中断执行完成，写EOIR*/
	
	/*还原现场，程序继续执行*/
	pop {r0}
	msr	spsr_cxsf, r0					/*恢复spsr,会自动执行 [cpsr] = [spsr]*/

	pop {r0-r3,r12}
	pop {lr}
	subs pc, lr, #4
	
	
FIQ_Handler:
	ldr r0, =FIQ_Handler
	bx r0






	
	






