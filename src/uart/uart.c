/*********************************************************************
 * ��Ȩ����: Copyright (c) 2020-2021  XXX Company. All rights reserved.
 * ϵͳ����: 
 * �ļ�����: uart.c
 * ����ժҪ: ��Ҫ�������ļ������ݣ�������Ҫģ�顢�������书�ܵ�˵��
 * ��ǰ�汾: 
 * ��    ��: HJWang
 * �������: 2020-09-26 03:08
 * �޸ļ�¼: 
 * ��    ��          ��    ��          �޸���          �޸�ժҪ
**********************************************************************/




/********************************** ��׼��ͷ�ļ� ***********************************/
#include <xxx.h>


/********************************** �Ǳ�׼��ͷ�ļ� ***********************************/
#include "xxx.h"


/********************************** �������� ***********************************/


/********************************** �ļ��ڲ�ʹ�õĺ� ***********************************/


/********************************** �ļ��ڲ�ʹ�õ��������� ***********************************/


/********************************** ��̬ȫ�ֱ��� ***********************************/


/********************************** ȫ�ֱ��� ***********************************/


/********************************** �ֲ��������� ***********************************/


/********************************** �ֲ����� ***********************************/


/********************************** ȫ�ֺ��� ***********************************/


/********************************** ���ʵ�� ***********************************/

 UART_Type UART1;
void uart_Init(void)
{
	/*��ʼ������IO*/
	uart_io_Init();

	/*��ʼ��UART1*/
	uart_disable(UART1);			/*�ȹر�UART1*/
	uart_softreset(UART1);			/*�����λUART1*/

	UART1->UCR1 = 0;				/*�����UCR1�Ĵ���*/
	UART1->UCR1 &=	~(1 << 14);		/*�ر��Զ������ʼ��*/

	/*
		����UART��UCR2�Ĵ����������ֳ���ֹͣλ��У��ģʽ���ر�Ӳ������
		bit14: 1 ����RTS����
		bit8: 0 �ر���ż����
		bit6: 0 1λֹͣλ
		bit5: 1 8λ����λ
		bit2: 1 �򿪷���
		bit1��1 �򿪽���
	*/
	UART1->UCR2 |= (1 << 14) | (1 << 5) | (1 << 2) | (1 << 1);
	UART1->UCR3 |= 1 << 2;						/*UCR3��bit2 ����Ϊ1*/

	/*
		���ò�����
		�����ʼ��㹫ʽ:Baud Rate = Ref Freq / (16 * (UBMR + 1)/(UBIR+1))
		115200 = 80000000��80M�� / ��16 * ��3124 + ���� / ��71 + 1����
	*/
	UART1->UFCR = 5 << 7;		/*ref freq ���� ipg_clk /1 = 80M*/
	UART1->UBIR = 71;
	UART1->UBMR = 3124;

#if 0
	uart_setbaudrate(UART1, 115200, 80000000);
#endif

	uart_enable(UART1);		/*ʹ�ܴ���*/
}

void uart_io_Init(void)
{
	/*	
		��ʼ������IO
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
	base->UCR2 &= ~(1 << 0);				/*��λUART*/
	while((base->UCR2 & 0x01) == 0);		/*�ȴ���λ���*/
}

void putc(unsigned char c)
{
	while(((UART1->USR2 >> 3) & 0x01) == 0);			/*�ȴ���һ�η������*/
	UART1->UTXD		= c & 0xFF;							/*��������*/
}

void puts(char * str)
{
	char *p = str;
	while (*p)
		putc(*p++);
}

unsigned char getc(void)
{
	while((UART1->USR2 & 0xx1) == 0);					/*�ȴ��������*/
	return UART1->URXD;									/*���ؽ��յ�������*/
}

void raise(int sig_nr)
{

}