/*********************************************************************
 * ��Ȩ����: Copyright (c) 2020-2021  XXX Company. All rights reserved.
 * ϵͳ����: 
 * �ļ�����: lcd.c
 * ����ժҪ: ��Ҫ�������ļ������ݣ�������Ҫģ�顢�������书�ܵ�˵��
 * ��ǰ�汾: 
 * ��    ��: HJWang
 * �������: 2020-09-26 14:41
 * �޸ļ�¼: 
 * ��    ��          ��    ��          �޸���          �޸�ժҪ
**********************************************************************/




/********************************** ��׼��ͷ�ļ� ***********************************/
//#include <xxx.h>


/********************************** �Ǳ�׼��ͷ�ļ� ***********************************/
#include "lcd.h"
#include "gpio.h"
#include "delay.h"
#include "stdio.h"

/********************************** �������� ***********************************/


/********************************** �ļ��ڲ�ʹ�õĺ� ***********************************/


/********************************** �ļ��ڲ�ʹ�õ��������� ***********************************/
/*LCD�����ṹ��*/
struct tftlcd_typedef tftlcd_dev;



/********************************** ��̬ȫ�ֱ��� ***********************************/


/********************************** ȫ�ֱ��� ***********************************/


/********************************** �ֲ��������� ***********************************/


/********************************** �ֲ����� ***********************************/


/********************************** ȫ�ֺ��� ***********************************/
void lcd_init(void)
{
	lcdgpio_init();					/*��ʼ��IO*/
	lcdclk_init(32, 3, 5);			/*��ʼ��LCDʱ��*/

	lcd_reset();					/*��λLCD*/
	delayms(10);
	lcd_noreset();					/*������λELCDIF*/

	/*TFTLCD�����ṹ���ʼ��*/
	tftlcd_dev.height = 600;
	tftlcd_dev.width = 1024;
	tftlcd_dev.pixsize = 4;			/*ARG8888ģʽ��ÿ������4���ֽ�*/
	tftlcd_dev.vspw = 3;
	tftlcd_dev.vbpd = 20;
	tftlcd_dev.vfpd = 12;
	tftlcd_dev.hspw = 20;
	tftlcd_dev.hbpd =  140;
	tftlcd_dev.frambuffer = LCD_FRAMBUF_ADDR;
	tftlcd_dev.backcolor = LCD_WHITE;
	tftlcd_dev.forecolor = LCD_BLACK;

	/*
		��ʼ��ELCDIF��CTRL�Ĵ���
		bit[31]: 0 ֹͣ��λ
		bit[19]: 1 ��·������ģʽ 
		bit[17]: 1 LCD������dotclkģʽ
		bit[15:14]: 00 �������ݲ�����
		bit[13:12]: 00 csc������
		bit[11:10]: 11 24λ���߿��
		bit[9:8]:	11 24λ���ݿ�ȣ�Ҳ����RGB888
		bit[5]:		1	elcdif��������ģʽ
		bit[1]:		0	���е�24λ����Ч
	*/
	LCDIF->CTRL |= (1 << 19) | (1 << 17) | (0 << 14) | (0 << 12) | (3 << 10) | (3 << 8) | (1 << 5) | (0 << 1);

	/*
		��ʼ��ELCDIF�ļĴ���CTRL1
		bit[19:16] : 0x7 ARGBģʽ�£�����24λ���ݣ�Aͨ�����ô���
	*/
	LCDIF->CTRL1 = 0X7 << 16;

	/*
		��ʼ��ELCDIF�ļĴ���TRANSFER_COUNT�Ĵ���
		bit[31:16] : �߶�
		bit[15:0] : ���
	*/
	LCDIF->TRANSFER_COUNT = (tftlcd_dev.height << 16) | (tftlcd_dev.width << 0);

	/*
		��ʼ��ELCDIF��VDCTRL0�Ĵ���
		bit[29] : 0 VSYNC���
		bit[28] : 1 ʹ��ENABLE���
		bit[27] : 0 VSYNC�͵�ƽ��Ч
		bit[26] : 0 HSYNC�͵�ƽ��Ч
		bit[25] : 0 DOTCLK��������Ч
		bit[24] : 1 ENABLE�źŸߵ�ƽ��Ч
		bit[21] : 1 DOTCLKģʽ������1
		bit[20] : 1 DOTCLKģʽ������1
		bit[17:0] : vsw����
	*/
	LCDIF->VDCTRL0 = 0;		/*������*/
	LCDIF->VDCTRL0 = (0 << 29) | (1 << 28) | (0 << 27) | (0 << 26) | (0 << 25) | (1 << 24) | (1 << 21) | (tftlcd_dev.vspw << 0);

	/*
		��ʼ�� ELCDIF��VDCTRL1	�Ĵ���
		����VSYNC������
	*/
	LCDIF->VDCTRL1 = tftlcd_dev.height + tftlcd_dev.vspw + tftlcd_dev.vfpd + tftlcd_dev.vbpd;

	/*
		��ʼ��ELCDIF��VDCTRL2�Ĵ���
		����HSYNC����
		bit[31:18] : hsw
		bit[17:0] : HSYNC������
	*/
	LCDIF->VDCTRL2 = (tftlcd_dev.hspw << 18) | (tftlcd_dev.width + tftlcd_dev.hspw + tftlcd_dev.hfpd + tftlcd_dev.hbpd);

	/*
		��ʼ��ELCDIF��VDCTRL3�Ĵ���
		����HSYNC����
		bit[27:16] : ˮƽ�ȴ�ʱ����
		bit[15:0] : ��ֱ�ȴ�ʱ����
	*/
	LCDIF->VDCTRL3 = ((tftlcd_dev.hbpd +  tftlcd_dev.hspw) << 16 ) | ((tftlcd_dev.vbpd +tftlcd_dev.vspw));

	/*
		��ʼ��ELCDIF��CUR_BUF��NEXT_BUF�Ĵ���
		���õ�ǰ�Դ��ַ����һ֡���Դ��ַ
	*/
	LCDIF->CUR_BUF = (unsigned int)tftlcd_dev.frambuffer;
	LCDIF->NEXT_BUF = (unsigned int) tftlcd_dev.frambuffer;

	lcd_enable();		/*ʹ��LCD*/
	delayms(10);
	lcd_clear(LCD_WHITE);	/*����*/
	
}

void lcdgpio_init(void)
{

}
/*
	LCDʱ�ӳ�ʼ��, LCDʱ�Ӽ��㹫ʽ���£�
 *  LCD CLK = 24 * loopDiv / prediv / div
*/
void lcdclk_init(u8 loopDiv, u8 preDiv, u8 div)
{
	/*
		�ȳ�ʼ��video pll
		VIDEO PLL = OSC24m * (loopDivider + ��denominator / numerator��) / postDivider
		��ʹ��С����Ƶ���� ��� denominator �� numberator����Ϊ0
	*/
	CCM_ANALOG->PLL_VIDEO_NUM = 0;
	CCM_ANALOG->PPL_VIDEO_DENOM = 0;

	/*
		PLL_VIDEO�Ĵ���
		bit[13] : 1 ʹ��VIDEO PLLʱ��
		bit[10:19] : 2 ����postDividerΪ1��Ƶ
		bit[6:0] : 32 ����loopDivider�Ĵ���	
	*/
	CCM_ANALOG->PLL_VIDEO = (2 << 19) | (1 << 13) | (loopDiv << 0);

	/*
	MISC2�Ĵ�������
	bit[31:30] : 0 VIDEO��post-div���ã�ʱ��Դ��Դ��postDivider�� 1��Ƶ
	*/
	CCM_ANALOG->MISC2 &= ~(3 << 30);
	CCM_ANALOG->MISC2 = 0 << 30;

	/*
		LCDʱ����Դ��PLL15��Ҳ����VIDEO PLL
	*/
	CCM->CSCDR2 &= ~(7 << 15);
	CCM->CSCDR2 |= (2 << 15);				/*����LCDIF_PRE_CLKʹ��PLL15*/

	/*
		����LCDIF_PRE��Ƶ
	*/
	CCM->CSCDR2 &= ~(7 << 12);				/*���÷�Ƶ*/
	CCM->CSCDR2 |= (preDiv - 1) << 12;	

	/*����LCDIF��Ƶ*/
	CCM->CBCMR	&=	~(7 << 23);
	CCM->CBCMR	|= (div - 1) << 23;

	/*����LCDʱ��ԴΪLCDIF_PERʱ��*/
	CCM->CSCDR2 &= ~(7 << 9);				/*���ԭ��������*/
	CCM->CSCDR2 |= (0 << 9);				/*LCDIF_PERʱ��Դѡ��LCDIF_PERʱ��*/
}


void lcd_reset(void)
{
	LCDIF->CTRL = 1 << 31;		/*ǿ�Ƹ�λ*/
}

void lcd_noreset(void)
{
	LCDIF->CTRL = 0 << 31;		/*ȡ��ǿ�Ƹ�λ*/
}

void lcd_enable(void)
{
	LCDIF->CTRL |= 1 << 0;		/*ʹ��ELCDIF*/
}

inline void lcd_drawpoint(u16 x, u16 y, u32 color)
{
	*(unsigned int *)((unsigned int)tftlcd_dev.frambuffer + tftlcd_dev.pixsize * (tftlcd_dev.width * y + x)) = color;
}

inline u32 lcd_readpoint(u16 x, u16 y)
{
	return *(unsigned int *)((unsigned int)tftlcd_dev.frambuffer + tftlcd_dev.pixsize * (tftlcd_dev.width * y + x));
}

void  lcd_clear(u32 color)
{
	unsigned int num;
	unsigned int i = 0;

	unsigned int *startaddr = (unsigned int *)tftlcd_dev.frambuffer;
	num = (unsigned int )tftlcd_dev.width * (unsigned int)tftlcd_dev.height;
	for (i = 0; i < num; i++)
		startaddr[i] = color;
}


void lcd_fill(u16 x0, u16 y0, u16 x1, u16 y1, u32 color)
{
	unsigned short   x,y;
	if (x0 < 0) x0 = 0;
	if (y0 < 0) y0 = 0;
	if (x1 >= tftlcd_dev.width) 
		x1 = tftlcd_dev.width -1;
	if (y1 >= tftlcd_dev.height)
		y1 = tftlcd_dev.height;

	for (y = y0; y <= y1; y++)
	{
		for (x = x0; x <= x1; x++)
			lcd_drawpoint(x, y, color);
	}
}
/********************************** ���ʵ�� ***********************************/



