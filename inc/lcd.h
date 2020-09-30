/*********************************************************************
 * 版权所有: Copyright (c) 2020-2021  XXX Company. All rights reserved.
 * 系统名称: 
 * 文件名称: lcd_api.h
 * 内容摘要: 简要描述本文件的内容，包括主要模块、函数及其功能的说明
 * 当前版本: 
 * 作    者: HJWang
 * 设计日期: 2020-09-26 14:42
 * 修改记录: 
 * 日    期          版    本          修改人          修改摘要
**********************************************************************/




#ifndef __LCD_H__
#define __LCD_H__
/********************************** 其它条件编译选项 ***********************************/


/********************************** 标准库头文件 ***********************************/
//#include <xxx.h>


/********************************** 非标准库头文件 ***********************************/
#include "imx6ul.h"


/********************************** 常量定义 ***********************************/


/********************************** 全局宏 ***********************************/

/*颜色*/
#define		LCD_BLUE		0X000000FF
#define		LCD_GREEN		0X0000FF00
#define		LCD_RED			0X00FF0000
#define		LCD_CYAN		0X0000FFFF
#define		LCD_MAGENTA		0X00FF00FF
#define		LCD_YELLOW		0X00FFFF00
#define		LCD_LIGHTGREEN	0X0080FF80
#define		LCD_LIGHTBULE	0X008080FF
#define		LCD_RED			0X00FF8080
#define		LCD_LIGHTCYAN	0X0080FFFF
#define		LCD_LIGHTMAGENTA 0X00FF80FF
#define		LCD_LIGHTYELLOW	0X00FFFF80
#define		LCD_DARKBLUE	0X00000080
#define		LCD_DARKGREEN	0X00008000
#define		LCD_DARKRED		0X00800000
#define		LCD_DARKCYAN	0X00008080
#define		LCD_DARKMAGENTA	0X00800080
#define		LCD_DARKYELLOW	0X00808000
#define		LCD_WHITE		0X00FFFFFF
#define		LCD_LIGHTGRAY	0X00D3D3D3
#define		LCD_GRAY		0X00808080
#define		LCD_DARKGRAY	0X00404040
#define		LCD_BLACK		0X00000000
#define		LCD_BROWN		0X00A52A2A
#define		LCD_ORANGE		0X00FFA500
#define		LCD_TRANSPARENT 0X00000000

/*LCD显存地址*/
#define		LCD_FRAMBUF_ADDR	(0X89000000)

/********************************** 数据类型 ***********************************/
/*LCD控制参数结构体*/
struct	tftlcd_typedef{
	u16	height;				/*LCD屏幕高度*/
	u16 width;				/*LCD屏幕宽度*/
	u16 pixsize;			/*LCD每个像素所占字节大小*/

	u16 vspw;
	u16 vbpd;
	u16 vfpd;
	u16 hspw;
	u16 hbpd;
	u16 hfpd;

	u32 frambuffer;			/*LCD显存首地址*/
	u32 forecolor;			/*前景色*/
	u32 backcolor;			/*背景色*/
};

extern struct tftlcd_typedef tftlcd_dev;


/********************************** 函数声明 ***********************************/

void  lcd_init(void);
void lcdgpio_init(void);
void lcdclk_init(u8 loopDiv, u8 preDiv, u8 div);
void lcd_reset(void);
void lcd_noreset(void);
void lcd_enable(void);
void video_pllinit(u8 loopdivi, u8 postdivi);

inline void lcd_drawpoint(u16 x, u16 y, u32 color);
inline u32 lcd_readpoint(u16 x, u16 y);
void lcd_clear(u32 color);
void lcd_fill(u16 x0, u16 y0, u16 x1, u16 y1, u32 color);

/********************************** 类定义 ***********************************/


/********************************** 模板 ***********************************/


#endif /* __LCD_H__ */


