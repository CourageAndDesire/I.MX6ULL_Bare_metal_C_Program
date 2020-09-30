/*********************************************************************
 * ��Ȩ����: Copyright (c) 2020-2021  XXX Company. All rights reserved.
 * ϵͳ����: 
 * �ļ�����: uart.h
 * ����ժҪ: 
 			UART�����ò��裺1.����UART��ʱ��Դ    				The "Module Clock" is the UART_CLK which comes from CCM. The "PeripheralClock" is the IPG_CLK which comes from CCM
 													����Ϊ 01 IrDA (Interface)ģʽ
 							2.��ʼ��UART 
 							3.ʹ��UART        
 							4.��дUART�����շ�����
 * ��ǰ�汾: 
 * ��    ��: HJWang
 * �������: 2020-09-26 03:09
 * �޸ļ�¼: 
 * ��    ��          ��    ��          �޸���          �޸�ժҪ
**********************************************************************/




#ifndef __UART_H__
#define __UART_H__
/********************************** ������������ѡ�� ***********************************/


/********************************** ��׼��ͷ�ļ� ***********************************/
//#include <xxx.h>


/********************************** �Ǳ�׼��ͷ�ļ� ***********************************/
//#include ".h"
#include "imx6ul.h"


/********************************** �������� ***********************************/


/********************************** ȫ�ֺ� ***********************************/


/********************************** �������� ***********************************/


/********************************** �������� ***********************************/


/********************************** �ඨ�� ***********************************/


/********************************** ģ�� ***********************************/


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


