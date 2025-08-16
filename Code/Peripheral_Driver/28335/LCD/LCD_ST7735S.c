#include "Font.h"
#include "LCD_ST7735S.h"
#include "DSP28x_Project.h"
#include "stdio.h"

/*
    Ӳ����ST7735S LCD��ʾ��
    author�� SDUEE mcb
    encoding: GBK
    ע����ֲ���ļ����ı�define��init����
*/

void lcdGpioInit(void)
{
    InitGpio();
    EALLOW;
    //LCD_PIN_CS
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO1 = 0;
    GpioDataRegs.GPASET.bit.GPIO1 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;


    //LCD_PIN_RES
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO3 = 0;
    GpioDataRegs.GPACLEAR.bit.GPIO3 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO3 = 1;
    

    //LCD_PIN_A0
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;
    GpioDataRegs.GPACLEAR.bit.GPIO5 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO5 = 1;
    

    //LCD_PIN_SDA
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO7 = 0 ;
    GpioDataRegs.GPACLEAR.bit.GPIO7 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO7  = 1;
    

    //LCD_PIN_SCL
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO9 = 0;
    GpioDataRegs.GPACLEAR.bit.GPIO9 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO9 = 1;

    EDIS;  
}

unsigned long LCD_Pow(unsigned long X,  unsigned long Y)
{
	unsigned long Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

void lcdInit(void)
{
    lcdGpioInit();
    
    SET_LCD_RES
    DSP28x_usDelay(100000);
	CLR_LCD_RES
	DSP28x_usDelay(100000);
	SET_LCD_RES
	DSP28x_usDelay(200000);
    
    lcdSelectRegister(0x11); //Sleep out 
    DSP28x_usDelay(120000);
    
    lcdSelectRegister(0xB1); 
    lcdWriteDataU8(0x05); 
    lcdWriteDataU8(0x3C);
    lcdWriteDataU8(0x3C); 
    
    lcdSelectRegister(0xB2); 
    lcdWriteDataU8(0x05); 
    lcdWriteDataU8(0x3C); 
    lcdWriteDataU8(0x3C); 
    
    lcdSelectRegister(0xB3); 
    lcdWriteDataU8(0x05); 
    lcdWriteDataU8(0x3C); 
    lcdWriteDataU8(0x3C); 
    lcdWriteDataU8(0x05); 
    lcdWriteDataU8(0x3C); 
    lcdWriteDataU8(0x3C); 
    
    lcdSelectRegister(0xB4);
    lcdWriteDataU8(0x03); 
    
    lcdSelectRegister(0xC0); 
    lcdWriteDataU8(0x28); 
    lcdWriteDataU8(0x08); 
    lcdWriteDataU8(0x04); 
    
    lcdSelectRegister(0xC1); 
    lcdWriteDataU8(0XC0); 
    
    lcdSelectRegister(0xC2); 
    lcdWriteDataU8(0x0D); 
    lcdWriteDataU8(0x00); 
    
    lcdSelectRegister(0xC3); 
    lcdWriteDataU8(0x8D); 
    lcdWriteDataU8(0x2A); 
    
    lcdSelectRegister(0xC4); 
    lcdWriteDataU8(0x8D); 
    lcdWriteDataU8(0xEE); 
    
    lcdSelectRegister(0xC5);
    lcdWriteDataU8(0x1A); 
    
    lcdSelectRegister(0x36);
    lcdWriteDataU8(0xC0); 
    
    lcdSelectRegister(0xE0); 
    lcdWriteDataU8(0x04); 
    lcdWriteDataU8(0x22); 
    lcdWriteDataU8(0x07); 
    lcdWriteDataU8(0x0A); 
    lcdWriteDataU8(0x2E); 
    lcdWriteDataU8(0x30); 
    lcdWriteDataU8(0x25); 
    lcdWriteDataU8(0x2A); 
    lcdWriteDataU8(0x28); 
    lcdWriteDataU8(0x26); 
    lcdWriteDataU8(0x2E); 
    lcdWriteDataU8(0x3A); 
    lcdWriteDataU8(0x00); 
    lcdWriteDataU8(0x01); 
    lcdWriteDataU8(0x03); 
    lcdWriteDataU8(0x13); 
    
    lcdSelectRegister(0xE1); 
    lcdWriteDataU8(0x04); 
    lcdWriteDataU8(0x16); 
    lcdWriteDataU8(0x06); 
    lcdWriteDataU8(0x0D); 
    lcdWriteDataU8(0x2D); 
    lcdWriteDataU8(0x26); 
    lcdWriteDataU8(0x23); 
    lcdWriteDataU8(0x27); 
    lcdWriteDataU8(0x27); 
    lcdWriteDataU8(0x25); 
    lcdWriteDataU8(0x2D); 
    lcdWriteDataU8(0x3B); 
    lcdWriteDataU8(0x00); 
    lcdWriteDataU8(0x01); 
    lcdWriteDataU8(0x04); 
    lcdWriteDataU8(0x13); 
    
    lcdSelectRegister(0x3A);
    lcdWriteDataU8(0x05);
    
    lcdSelectRegister(0x29); 
}

void lcdSelectRegister(unsigned char data)
{
    CLR_LCD_A0
    CLR_LCD_CS
    int i = 0;
    for(i = 0; i < 8; i++)
    {
        if(data & 0x80)
        {
            SET_LCD_SDA
        }
        else 
        {
            CLR_LCD_SDA
        }
        CLR_LCD_SCL
        SET_LCD_SCL
        data = data << 1;
    }	 
    SET_LCD_CS
}

void lcdWriteDataU8(unsigned char data)
{
    SET_LCD_A0
    CLR_LCD_CS
    int i;
    for(i = 0; i < 8; i++)
    {
        if( data & 0x80 )
        {
            SET_LCD_SDA
        }
        else 
        {
            CLR_LCD_SDA
        }
        CLR_LCD_SCL
        SET_LCD_SCL
        data = data << 1;
    }
    SET_LCD_CS
}

void lcdWriteDataU16(unsigned short data)
{
    lcdWriteDataU8(data>>8);
    lcdWriteDataU8(data);
}

void lcdSetAddress(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2)
{	     
    lcdSelectRegister(0x2A); 
    lcdWriteDataU8(x1>>8); 
    lcdWriteDataU8(x1&0xFF);
    lcdWriteDataU8(x2>>8); 
    lcdWriteDataU8(x2&0xFF);

    lcdSelectRegister(0x2B); 
    lcdWriteDataU8(y1>>8); 
    lcdWriteDataU8(y1&0xFF);
    lcdWriteDataU8(y2>>8); 
    lcdWriteDataU8(y2&0xFF);
    
    lcdSelectRegister(0x2C);
}

void lcdClear(unsigned short color)
{
    lcdSetAddress(0, 0, 130, 162);
    unsigned char i , j ;
    for(i = 0; i < 130; i++)
    {
        for (j = 0; j < 162; j++)
	   	{
            lcdWriteDataU16(color);
	    }
    }
} 

//LCD����
void LCD_DrawPoint(unsigned int x, unsigned int y, unsigned int color)
{
	lcdSetAddress(x, y, x, y);//���ù��λ�� 
	lcdWriteDataU16(color);
}

//���ٻ���
//x,y:����
//color:��ɫ
void LCD_Fast_DrawPoint(unsigned int x, unsigned int y, unsigned int color)
{
	lcdWriteDataU16(color);
}	

//LCD������ʾ
void LCD_DisplayOn(void)
{					   
	lcdSelectRegister(0x29);	//������ʾ
}	 
//LCD�ر���ʾ
void LCD_DisplayOff(void)
{	   
	lcdSelectRegister(0x28);	//�ر���ʾ
}   

//��ָ����������䵥����ɫ
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void LCD_Fill(unsigned short sx, unsigned short sy, unsigned short ex, unsigned short ey, unsigned short color)
{          
	unsigned short i, j;
	unsigned short xlen = 0;
	unsigned short ylen = 0;
	
	xlen = ex - sx + 1;
	ylen = ey - sy + 1;
		
	lcdSetAddress(sx, sy, ex, ey);
    for(i = 0; i < xlen; i++)
	{
		for(j = 0; j < ylen; j++)
		{
			lcdWriteDataU16(color);
		}
	}		 
}  

//����
//x1,y1:�������
//x2,y2:�յ�����  
void LCD_DrawLine(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short color)
{
	unsigned short t; 
	int xerr = 0, yerr = 0, delta_x, delta_y, distance; 
	int incx, incy, uRow, uCol; 
	delta_x = x2 - x1; //������������ 
	delta_y = y2 - y1; 
	uRow = x1; 
	uCol = y1; 
	
	//���õ������� 
	if( delta_x > 0 )
	{
		incx=1; 
	}
	else if( delta_x == 0 )//��ֱ�� 
	{
		incx=0;
	}
	else
	{
		incx = -1;
		delta_x = -delta_x;
	}

	
	if( delta_y > 0 )
	{
		incy=1; 
	}
	else if( delta_y == 0 )//ˮƽ�� 
	{
		incy=0;
	}
	else
	{
		incy = -1;
		delta_y = -delta_y;
	} 
	
	if( delta_x > delta_y )//ѡȡ�������������� 
	{
		distance = delta_x; 
	}
	else
	{
		distance=delta_y; 
	}
	
	for(t = 0; t <= distance + 1; t++ )//������� 
	{  
		LCD_DrawPoint(uRow, uCol, color);//���� 
		xerr += delta_x ; 
		yerr += delta_y ; 
		if( xerr > distance ) 
		{ 
			xerr -= distance; 
			uRow += incx; 
		} 
		
		if( yerr > distance ) 
		{ 
			yerr -= distance; 
			uCol += incy; 
		} 
	}  
}  

void LCD_DrawRectangle(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short color)
{
	LCD_DrawLine(x1, y1, x2, y1, color);
	LCD_DrawLine(x1, y1, x1, y2, color);
	LCD_DrawLine(x1, y2, x2, y2, color);
	LCD_DrawLine(x2, y1, x2, y2, color);
}

//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void LCD_Draw_Circle(unsigned short x0, unsigned short y0, unsigned char r, unsigned short color)
{
	int a, b;
	int di;
	a = 0;
	b = r;	  
	di = 3 - ( r<<1 );             //�ж��¸���λ�õı�־
	while( a <= b )
	{
		LCD_DrawPoint(x0+a, y0-b, color);
 		LCD_DrawPoint(x0+b, y0-a, color);        
		LCD_DrawPoint(x0+b, y0+a, color);       
		LCD_DrawPoint(x0+a, y0+b, color);
		LCD_DrawPoint(x0-a, y0+b, color);
 		LCD_DrawPoint(x0-b, y0+a, color);
		LCD_DrawPoint(x0-a, y0-b, color);
  		LCD_DrawPoint(x0-b, y0-a, color);
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if( di < 0 )
		{
			di += 4 * a + 6;	 
		} 
		else
		{
			di += 10 + 4 * ( a - b );   
			b--;
		} 						    
	}
} 	

//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void LCD_DrawFullCircle(unsigned short Xpos, unsigned short Ypos, unsigned short Radius, unsigned short Color)
{
	Uint16 x, y, r = Radius;
	for(y = Ypos - r; y < Ypos + r; y++)
	{
		for(x = Xpos - r;x < Xpos + r; x++)
		{
			if(((x - Xpos) * (x - Xpos) + (y - Ypos) * (y - Ypos)) <= r * r)
			{
				LCD_DrawPoint(x, y, Color);
			}
		}
	}
}

//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16/24
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowChar(unsigned short x, unsigned short y, unsigned char num, unsigned char mode, unsigned short pen_color, unsigned short back_color)
{  							  
    unsigned char temp, t1, t;
	unsigned short y0 = y;
 	num = num - ' ';//�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
	
	for(t = 0; t < 16; t++)
	{   
		temp=asc2_1608[num][t];
		for(t1 = 0; t1 < 8; t1++)
		{			    
			if( temp & 0x80 )
			{
				LCD_DrawPoint(x, y, pen_color);
			}
			else if( mode == 0)
			{
				LCD_DrawPoint(x, y, back_color);
			}
			temp <<= 1;
			y++;
			
			if(y >= 240)//������
			{
				return;	
			}	
			
			if((y-y0) == 16)
			{
				y = y0;
				x++;
				if(x>=320)//������
				{
					return;	
				}
				break;
			}
		}  	 
	}  	    	   	 	  
}

//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ		  
void LCD_ShowString(unsigned short x, unsigned short y, unsigned short width, unsigned short height, unsigned char *p, unsigned short pen_color, unsigned short back_color)
{         
	unsigned char x0 = x;
	width += x;
	height += y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x >= width)
		{
			x = x0;
			y += 16;
		}
		
        if(y >= height)//�˳�
		{
			break;
		}
		
        LCD_ShowChar(x, y, *p, 0, pen_color, back_color);
        x += 8;
        p++;
    }  
}
//��ʾ˫���ȸ�����
void LCD_ShowDouble(unsigned short x, unsigned short y, unsigned short width, unsigned short height, double num, unsigned short pen_color, unsigned short back_color)
{
    char tem[LCD_LENGTH];
    sprintf(tem,"%lf",num);
    LCD_ShowString(x,y,width,height,tem,BLACK,WHITE);
}

//��ʾʮ��������
void LCD_ShowInt(unsigned short x, unsigned short y, unsigned short width, unsigned short height,  int num, unsigned short pen_color, unsigned short back_color)
{
    char tem[LCD_LENGTH];
    sprintf(tem,"%d",num);
    LCD_ShowString(x,y,width,height,tem,pen_color,back_color);
}

//��ʾʮ����������
void LCD_ShowHex(unsigned short x, unsigned short y, unsigned short width, unsigned short height, int num, unsigned short pen_color, unsigned short back_color)
{
    char tem[LCD_LENGTH];
    sprintf(tem,"%x",num);
    LCD_ShowString(x,y,width,height,tem,pen_color,back_color);
}

//��ʾ�˽�������
void LCD_ShowOct(unsigned short x, unsigned short y, unsigned short width, unsigned short height, int num, unsigned short pen_color, unsigned short back_color)
{
    char tem[LCD_LENGTH];
    sprintf(tem,"%o",num);
    LCD_ShowString(x,y,width,height,tem,pen_color,back_color);
}

//��ʾ����������
void LCD_ShowBin(unsigned short x, unsigned short y, unsigned short width, unsigned short height, int num,int Length, unsigned short pen_color, unsigned short back_color)
{
    int i;
    for (i = 0; i < Length; i++)
    {
        LCD_ShowChar(x, y + i * 16, num / LCD_Pow(2, Length - i - 1) % 2 + '0', 1,pen_color,back_color);
    }
}
