/*********************************************************************
 * ��Ȩ����: Copyright (c) 2020-2021  XXX Company. All rights reserved.
 * ϵͳ����: 
 * �ļ�����: main.h
 * ����ժҪ: �������� mainԴ�ļ���ͷ�ļ�
 * ��ǰ�汾: 
 * ��    ��: HJWang
 * �������: 2020-09-05 18:13
 * �޸ļ�¼: 
 * ��    ��          ��    ��          �޸���          �޸�ժҪ
**********************************************************************/




#ifndef __MAIN_H__
#define __MIAN_H__


#include "led.h"
#include "MCIMX6Y2.h"
#include "clk.h"

/********************************** ������������ѡ�� ***********************************/
//ȷ��LED��beep��key����Ҫʹ�õļĴ�����ַ
/*
	1��ʱ�ӡ�������Ϊ I.MX6ULL��bin�ļ�ͷ��������������ӣ����У����г�ʼ��ʱ�ӣ����Բ���
	2.SW_MUX	���üĴ���
	3.SW_PAD	���üĴ���
	4.GPIO�����Ĵ���			��������Ҫ�ǶԵ�ƽ���в�����������Ҫ��ע�ļĴ����� GDIR(���÷���),DR�����øߵ͵�ƽ��
*/

/*ʱ��*/
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

/*LED�Ĵ���*/
#define 	IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03	(*(volatile unsigned int *)0x020c4068)
#define 	IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO03	(*(volatile unsigned int *)0x020e02f4)
#define 	GPIO1_GDIR	(*(volatile unsigned int *)0x0209c004)
#define 	GPIO1_DR	(*(volatile unsigned int *)0x0209c000)



/********************************** ��׼��ͷ�ļ� ***********************************/



/********************************** �Ǳ�׼��ͷ�ļ� ***********************************/


/********************************** �������� ***********************************/


/********************************** ȫ�ֺ� ***********************************/


/********************************** �������� ***********************************/


/********************************** �������� ***********************************/


/********************************** �ඨ�� ***********************************/


/********************************** ģ�� ***********************************/


#endif /* __MAIN_H__ */


