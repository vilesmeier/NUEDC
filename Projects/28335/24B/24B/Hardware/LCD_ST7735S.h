#ifndef __LCD_ST7735S_H
#define __LCD_ST7735S_H

/*
    ��ֲ���ļ�
    �ı�define��init����
*/

//�ӿڷ�װ
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

//�����װ
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
#define BROWN	0XBC40 //��ɫ
#define BRRED	0XFC07 //�غ�ɫ
#define GRAY	0X8430 //��ɫ
#define DARKBLUE	0X01CF	//����ɫ
#define LIGHTBLUE	0X7D7C	//ǳ��ɫ  
#define GRAYBLUE	0X5458 //����ɫ

//���ܺ���
void lcdGpioInit(void);
//��ʼ��
void lcdInit(void);
void lcdSelectRegister(unsigned char data);
void lcdWriteDataU8(unsigned char data);
void lcdWriteDataU16(unsigned short data);
void lcdSetAddress(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
void lcdClear(unsigned short color);
unsigned long LCD_Pow(unsigned long X,unsigned long Y);
void LCD_DrawPoint(unsigned int x, unsigned int y, unsigned int color);
//����ʾ
void LCD_DisplayOn(void); 
//����ʾ
void LCD_DisplayOff(void); 
//��Բ
void LCD_Draw_Circle(unsigned short x0, unsigned short y0, unsigned char r, unsigned short color); 
void LCD_DrawFullCircle(unsigned short Xpos, unsigned short Ypos, unsigned short Radius, unsigned short Color);
//�������
void LCD_Fill(unsigned short sx, unsigned short sy, unsigned short ex, unsigned short ey, unsigned short color);
//���� 
void LCD_DrawLine(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short color); 
//������
void LCD_DrawRectangle(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short color); 
//дASCII�ַ�
void LCD_ShowChar(unsigned short x, unsigned short y, unsigned char num, unsigned short pen_color, unsigned short back_color);
//дASCII�ַ���
void LCD_ShowString(unsigned short x, unsigned short y, unsigned short width, unsigned short height, unsigned char *p, unsigned short pen_color, unsigned short back_color);
//д����������
void LCD_ShowBin(unsigned short x, unsigned short y, unsigned short width, unsigned short height,const int num,int Length, unsigned short pen_color, unsigned short back_color);
//д��������С��10λ��bin λ��
void LCD_ShowFloat(unsigned short x, unsigned short y,const float Number, unsigned char Length,unsigned char Flength,unsigned short pen_color, unsigned short back_color);



#endif
