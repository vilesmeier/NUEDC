#ifndef __LCD_ST7735S_H
#define __LCD_ST7735S_H

/*
    移植本文件
    改变define及init函数
*/

//接口封装
#define SET_LCD_CS 	    GpioDataRegs.GPCSET.bit.GPIO68=1;
#define SET_LCD_RES     GpioDataRegs.GPCSET.bit.GPIO66=1;
#define SET_LCD_A0 	    GpioDataRegs.GPCSET.bit.GPIO67=1;
#define SET_LCD_SDA     GpioDataRegs.GPCSET.bit.GPIO65=1;
#define SET_LCD_SCL     GpioDataRegs.GPCSET.bit.GPIO64=1;

#define	CLR_LCD_CS      GpioDataRegs.GPCCLEAR.bit.GPIO68=1;
#define	CLR_LCD_RES     GpioDataRegs.GPCCLEAR.bit.GPIO66=1;
#define	CLR_LCD_A0      GpioDataRegs.GPCCLEAR.bit.GPIO67=1;
#define	CLR_LCD_SDA     GpioDataRegs.GPCCLEAR.bit.GPIO65=1;
#define	CLR_LCD_SCL     GpioDataRegs.GPCCLEAR.bit.GPIO64=1;

//命令封装
#define WHITE	0xFFFF
#define BLACK	0x0000	  
#define BLUE	0x001F  
#define BRED	0XF81F
#define GRED	0XFFE0
#define GBLUE	0X07FF
#define RED		0xF800
#define MAGENTA	0xF81F
#define GREEN	0x07E0
#define CYAN	0x7FFF
#define YELLOW	0xFFE0
#define BROWN	0XBC40 //棕色
#define BRRED	0XFC07 //棕红色
#define GRAY	0X8430 //灰色
#define DARKBLUE	0X01CF	//深蓝色
#define LIGHTBLUE	0X7D7C	//浅蓝色  
#define GRAYBLUE	0X5458 //灰蓝色

//功能函数
void lcdGpioInit(void);
//初始化
void lcdInit(void);
void lcdSelectRegister(unsigned char data);
void lcdWriteDataU8(unsigned char data);
void lcdWriteDataU16(unsigned short data);
void lcdSetAddress(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
void lcdClear(unsigned short color);
unsigned long LCD_Pow(unsigned long X,unsigned long Y);
void LCD_DrawPoint(unsigned int x, unsigned int y, unsigned int color);
//开显示
void LCD_DisplayOn(void); 
//关显示
void LCD_DisplayOff(void); 
//画圆
void LCD_Draw_Circle(unsigned short x0, unsigned short y0, unsigned char r, unsigned short color); 
void LCD_DrawFullCircle(unsigned short Xpos, unsigned short Ypos, unsigned short Radius, unsigned short Color);
//填充区域
void LCD_Fill(unsigned short sx, unsigned short sy, unsigned short ex, unsigned short ey, unsigned short color);
//画线 
void LCD_DrawLine(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short color); 
//画矩形
void LCD_DrawRectangle(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short color); 
//写ASCII字符
void LCD_ShowChar(unsigned short x, unsigned short y, unsigned char num, unsigned short pen_color, unsigned short back_color);
//写ASCII字符串
void LCD_ShowString(unsigned short x, unsigned short y, unsigned short width, unsigned short height, unsigned char *p, unsigned short pen_color, unsigned short back_color);
//写二进制整数
void LCD_ShowBin(unsigned short x, unsigned short y, unsigned short width, unsigned short height,const int num,int Length, unsigned short pen_color, unsigned short back_color);
//写浮点数，小于10位，bin 位数
void LCD_ShowFloat(unsigned short x, unsigned short y,const float Number, unsigned char Length,unsigned char Flength,unsigned short pen_color, unsigned short back_color);



#endif
