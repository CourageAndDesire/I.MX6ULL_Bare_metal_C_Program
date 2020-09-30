/*********************************************************************
 * ��Ȩ����: Copyright (c) 2020-2021  XXX Company. All rights reserved.
 * ϵͳ����: 
 * �ļ�����: start.s
 * ����ժҪ: ����C���Ի�������������ļ������һЩ��ʼ���Ĳ�����1.����SVCģʽ����Ȩģʽ����2.���ö�ջ
 * ��ǰ�汾: 
 * ��    ��: HJWang
 * �������: 2020-09-05 17:05
 * �޸ļ�¼: 
 * ��    ��          ��    ��          �޸���          �޸�ժҪ
**********************************************************************/
	/*@�������Ĭ����ڱ����_start,��������Ҳ���������ӽű���ʹ��ENTRY��ָ����������ڵ㡣
	@.global ��������gcc����������������ȫ�ֱ�š�
	@_start :��������ִ�е���ڡ�*/

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
	/*�ر�ȫ���ж�*/
	cpsid	i
	cpsid	f
	
	/*�ر� D cache��I cache�� MMU*/
	mrc p15, 0, r0, c1, c0, 0		/*��ȡCP15��c1�Ĵ�����r0��*/
	bic	r0,	r0, #(0x1 << 12)		/*���C1��Iλ���ر� I cache*/	
	bic	r0,	r0, #(0x1 << 2)			/*���C1��Cλ�� �ر�D cache*/
	bic	r0,	r0, #0x2				/*���C1��Aλ���ر� ������*/
	bic	r0,	r0, #(0x1 << 11)		/*���C1��Zλ��	�رշ�֧Ԥ��*/
	bic	r0,	r0, #0x01				/*���C1��Mλ���ر�MMU*/
	mcr	p15, 0, r0, c1, c0, 0		/*��r0 ��ֵд�뵽CP15��C1��*/

	
	/*�����ж��������ض�λ*/
	ldr		r0, =0x87800000

	dsb
	isb
	mcr	p15, 0, r0,c12, c0, 0		/*VBAR�Ĵ�����Ҳ�������������ַ�Ĵ���*/
	dsb
	isb
	/*
		���ø���ģʽ�Ķ�ջ��ΪC���Ի�����׼��
	*/
	/*IRQ*/
	mrs	r0,	cpsr				/*[r0] = [cpsr]*/
	bic r0,r0, #0x1f				/*[r0] = [r0] & 0xffffffE0*/
	orr	r0,r0, #0x12				/*����ΪIRQģʽ*/
	msr	cpsr, r0				/*[cpsr] = [r0]*/

	ldr sp, =0x80600000			/*���ö�ջ��ջ��Ϊ0x80600000 ��СΪ2MB*/

	/*SYS*/
	mrs	r0,	cpsr				/*[r0] = [cpsr]*/
	bic r0,r0, #0x1f				/*[r0] = [r0] & 0xffffffE0*/
	orr	r0,r0, #0x1f				/*����ΪIRQģʽ*/
	msr	cpsr, r0				/*[cpsr] = [r0]*/

	ldr sp, =0x80400000			/*���ö�ջ��ջ��Ϊ0x80600000 ��СΪ2MB*/
	
	/*SVC*/
	mrs	r0,	cpsr				/*[r0] = [cpsr]*/
	bic r0,r0, #0x1f				/*[r0] = [r0] & 0xffffffE0*/
	orr	r0,r0, #0x13				/*����ΪIRQģʽ*/
	msr	cpsr, r0				/*[cpsr] = [r0]*/

	ldr sp, =0x80200000			/*���ö�ջ��ջ��Ϊ0x80600000 ��СΪ2MB*/

	/*��ȫ���ж�*/
	cpsie	i
	cpsie	f
	
	/*��ת��main*/
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
	/*�������ӵ�ַ*/
	push {lr}			/*��ջ������lr�ĵ�ַ�����ﱣ����� SVCģʽ�µ� ��һ����ַ����Ϊ����mian�����ģʽ�� SVCģʽ*/
	/*����Ĵ���*/
	push {r0-r3,r12}	/*���� r0��r3�Լ�r12�Ĵ�������Ϊ���жϷ�����ʱ�� r4~r11���������Զ�����*/	
	/**����spsr*/
	mrs	r0,	spsr		/*���ﱣ�����SCVģʽ�µ� CPSR��״̬*/
	push {r0}

	/*IRQ �жϵĴ����ҵ��ĸ����跢�����ж�*/

	mrc	p15,4,r1,c15,c0,0	/*��ȡ CBAR�Ĵ�����ֵ��CBAR������GIC�Ĵ����Ļ��������ַ*/
	add	r1,r1,#0x2000		/*�õ�CPU�ӿڶ˻���ַ������ GIC�Ĵ�������ַƫ�� 0x2000*/
	ldr r0, [r1,#0x000C]	/*�ҵ�GICC_IAR�Ĵ��������л�ȡ �����жϵ� CPUID �� �ж�ID*/

	push {r0,r1}			/*���� ��ȡ �����жϵ� CPUID �� �ж�ID �� CPU�ӿڶ˻���ַ*/ /*��������ԭ����Ϊ�������ж�Ƕ�ף��������������ж�֮����Щ����Ҫ����*/

	/*����SVC ģʽ������������IRQ�жϽ����������ж�Ƕ��*/
	cps	#0x13	
	push {lr}				/*����IRQ��lr��ַ*/
	/*��ת���жϷ��������д���*/
	ldr r2, =system_irqhandler	/*�����жϷ�������r2��*/
	blx r2						/*ִ���жϷ���������һ��R0����*/

	/*IRQ�ж�ִ�����*/
	pop {lr}					/*ִ�����жϷ����������г�ջ*/
	cps	#0x12					/*���뵽 IRQģʽ*/
	pop {r0,r1}
	str r0, [r1,#0x10]							/*�ж�ִ����ɣ�дEOIR*/
	
	/*��ԭ�ֳ����������ִ��*/
	pop {r0}
	msr	spsr_cxsf, r0					/*�ָ�spsr,���Զ�ִ�� [cpsr] = [spsr]*/

	pop {r0-r3,r12}
	pop {lr}
	subs pc, lr, #4
	
	
FIQ_Handler:
	ldr r0, =FIQ_Handler
	bx r0






	
	






