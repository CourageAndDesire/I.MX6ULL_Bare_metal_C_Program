/*********************************************************************
 * 版权所有: Copyright (c) 2020-2021  XXX Company. All rights reserved.
 * 系统名称: 
 * 文件名称: lcd.c
 * 内容摘要: 简要描述本文件的内容，包括主要模块、函数及其功能的说明
 * 当前版本: 
 * 作    者: HJWang
 * 设计日期: 2020-09-26 14:41
 * 修改记录: 
 * 日    期          版    本          修改人          修改摘要
**********************************************************************/




/********************************** 标准库头文件 ***********************************/
//#include <xxx.h>


/********************************** 非标准库头文件 ***********************************/
#include "lcd.h"
#include "gpio.h"
#include "delay.h"
#include "stdio.h"

/********************************** 常量定义 ***********************************/


/********************************** 文件内部使用的宏 ***********************************/


/********************************** 文件内部使用的数据类型 ***********************************/
/*LCD参数结构体*/
struct tftlcd_typedef tftlcd_dev;



/********************************** 静态全局变量 ***********************************/


/********************************** 全局变量 ***********************************/


/********************************** 局部函数声明 ***********************************/


/********************************** 局部函数 ***********************************/


/********************************** 全局函数 ***********************************/
void lcd_init(void)
{
	lcdgpio_init();					/*初始化IO*/
	lcdclk_init(32, 3, 5);			/*初始加LCD时钟*/

	lcd_reset();					/*复位LCD*/
	delayms(10);
	lcd_noreset();					/*结束复位ELCDIF*/

	/*TFTLCD参数结构体初始化*/
	tftlcd_dev.height = 600;
	tftlcd_dev.width = 1024;
	tftlcd_dev.pixsize = 4;			/*ARG8888模式。每个像素4个字节*/
	tftlcd_dev.vspw = 3;
	tftlcd_dev.vbpd = 20;
	tftlcd_dev.vfpd = 12;
	tftlcd_dev.hspw = 20;
	tftlcd_dev.hbpd =  140;
	tftlcd_dev.frambuffer = LCD_FRAMBUF_ADDR;
	tftlcd_dev.backcolor = LCD_WHITE;
	tftlcd_dev.forecolor = LCD_BLACK;

	/*
		初始化ELCDIF的CTRL寄存器
		bit[31]: 0 停止复位
		bit[19]: 1 旁路计数器模式 
		bit[17]: 1 LCD工作在dotclk模式
		bit[15:14]: 00 输入数据不交换
		bit[13:12]: 00 csc不交换
		bit[11:10]: 11 24位总线宽度
		bit[9:8]:	11 24位数据宽度，也就是RGB888
		bit[5]:		1	elcdif工作在主模式
		bit[1]:		0	所有的24位均有效
	*/
	LCDIF->CTRL |= (1 << 19) | (1 << 17) | (0 << 14) | (0 << 12) | (3 << 10) | (3 << 8) | (1 << 5) | (0 << 1);

	/*
		初始化ELCDIF的寄存器CTRL1
		bit[19:16] : 0x7 ARGB模式下，传输24位数据，A通道不用传输
	*/
	LCDIF->CTRL1 = 0X7 << 16;

	/*
		初始化ELCDIF的寄存器TRANSFER_COUNT寄存器
		bit[31:16] : 高度
		bit[15:0] : 宽度
	*/
	LCDIF->TRANSFER_COUNT = (tftlcd_dev.height << 16) | (tftlcd_dev.width << 0);

	/*
		初始化ELCDIF的VDCTRL0寄存器
		bit[29] : 0 VSYNC输出
		bit[28] : 1 使能ENABLE输出
		bit[27] : 0 VSYNC低电平有效
		bit[26] : 0 HSYNC低电平有效
		bit[25] : 0 DOTCLK上升沿有效
		bit[24] : 1 ENABLE信号高电平有效
		bit[21] : 1 DOTCLK模式下设置1
		bit[20] : 1 DOTCLK模式下设置1
		bit[17:0] : vsw参数
	*/
	LCDIF->VDCTRL0 = 0;		/*先清零*/
	LCDIF->VDCTRL0 = (0 << 29) | (1 << 28) | (0 << 27) | (0 << 26) | (0 << 25) | (1 << 24) | (1 << 21) | (tftlcd_dev.vspw << 0);

	/*
		初始化 ELCDIF的VDCTRL1	寄存器
		设置VSYNC总周期
	*/
	LCDIF->VDCTRL1 = tftlcd_dev.height + tftlcd_dev.vspw + tftlcd_dev.vfpd + tftlcd_dev.vbpd;

	/*
		初始化ELCDIF的VDCTRL2寄存器
		设置HSYNC周期
		bit[31:18] : hsw
		bit[17:0] : HSYNC总周期
	*/
	LCDIF->VDCTRL2 = (tftlcd_dev.hspw << 18) | (tftlcd_dev.width + tftlcd_dev.hspw + tftlcd_dev.hfpd + tftlcd_dev.hbpd);

	/*
		初始化ELCDIF的VDCTRL3寄存器
		设置HSYNC周期
		bit[27:16] : 水平等待时钟数
		bit[15:0] : 垂直等待时钟数
	*/
	LCDIF->VDCTRL3 = ((tftlcd_dev.hbpd +  tftlcd_dev.hspw) << 16 ) | ((tftlcd_dev.vbpd +tftlcd_dev.vspw));

	/*
		初始化ELCDIF的CUR_BUF和NEXT_BUF寄存器
		设置当前显存地址和下一帧的显存地址
	*/
	LCDIF->CUR_BUF = (unsigned int)tftlcd_dev.frambuffer;
	LCDIF->NEXT_BUF = (unsigned int) tftlcd_dev.frambuffer;

	lcd_enable();		/*使能LCD*/
	delayms(10);
	lcd_clear(LCD_WHITE);	/*清屏*/
	
}

void lcdgpio_init(void)
{

}
/*
	LCD时钟初始化, LCD时钟计算公式如下：
 *  LCD CLK = 24 * loopDiv / prediv / div
*/
void lcdclk_init(u8 loopDiv, u8 preDiv, u8 div)
{
	/*
		先初始化video pll
		VIDEO PLL = OSC24m * (loopDivider + （denominator / numerator）) / postDivider
		不使用小数分频器， 因此 denominator 和 numberator设置为0
	*/
	CCM_ANALOG->PLL_VIDEO_NUM = 0;
	CCM_ANALOG->PPL_VIDEO_DENOM = 0;

	/*
		PLL_VIDEO寄存器
		bit[13] : 1 使能VIDEO PLL时钟
		bit[10:19] : 2 设置postDivider为1分频
		bit[6:0] : 32 设置loopDivider寄存器	
	*/
	CCM_ANALOG->PLL_VIDEO = (2 << 19) | (1 << 13) | (loopDiv << 0);

	/*
	MISC2寄存器设置
	bit[31:30] : 0 VIDEO的post-div设置，时钟源来源于postDivider， 1分频
	*/
	CCM_ANALOG->MISC2 &= ~(3 << 30);
	CCM_ANALOG->MISC2 = 0 << 30;

	/*
		LCD时钟来源于PLL15，也就是VIDEO PLL
	*/
	CCM->CSCDR2 &= ~(7 << 15);
	CCM->CSCDR2 |= (2 << 15);				/*设置LCDIF_PRE_CLK使用PLL15*/

	/*
		设置LCDIF_PRE分频
	*/
	CCM->CSCDR2 &= ~(7 << 12);				/*设置分频*/
	CCM->CSCDR2 |= (preDiv - 1) << 12;	

	/*设置LCDIF分频*/
	CCM->CBCMR	&=	~(7 << 23);
	CCM->CBCMR	|= (div - 1) << 23;

	/*设置LCD时钟源为LCDIF_PER时钟*/
	CCM->CSCDR2 &= ~(7 << 9);				/*清除原来的设置*/
	CCM->CSCDR2 |= (0 << 9);				/*LCDIF_PER时钟源选择LCDIF_PER时钟*/
}


void lcd_reset(void)
{
	LCDIF->CTRL = 1 << 31;		/*强制复位*/
}

void lcd_noreset(void)
{
	LCDIF->CTRL = 0 << 31;		/*取消强制复位*/
}

void lcd_enable(void)
{
	LCDIF->CTRL |= 1 << 0;		/*使能ELCDIF*/
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
/********************************** 类的实现 ***********************************/



