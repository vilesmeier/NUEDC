#include "global.h"
#include "oled_font.h"
#include "oled.h"

void IIC_Start()
{
    EALLOW;
	OLED_SCK_Set();
	DELAY_US(1);
	OLED_SDA_Set();
	DELAY_US(1);
	OLED_SDA_Clr();
	DELAY_US(1);
	OLED_SCK_Clr();
	DELAY_US(1);
	EDIS;
}

void IIC_Stop()
{
    EALLOW;
    OLED_SCK_Set() ;
    DELAY_US(1);
	OLED_SDA_Clr();
	DELAY_US(1);
	OLED_SDA_Set();
	DELAY_US(1);
	EDIS;
}

void IIC_Wait_Ack()
{
    EALLOW;
	OLED_SCK_Set() ;
	DELAY_US(1);
	OLED_SCK_Clr();
	DELAY_US(1);
	EDIS;
}

void Write_IIC_Byte(Uint8 IIC_Byte)
{
    EALLOW;
    Uint8 i;
    Uint8 m,da;
	da=IIC_Byte;
	DELAY_US(1);
	OLED_SCK_Clr();
	DELAY_US(1);
	for(i=0;i<8;i++)		
	{
		m=da;
		m=m&0x80;
		if(m==0x80)
		{
			OLED_SDA_Set();
			DELAY_US(1);
		}
		else
		{
			OLED_SDA_Clr();
			DELAY_US(1);
		} 
		OLED_SCK_Set();
		DELAY_US(1);
		OLED_SCK_Clr();
		DELAY_US(1);
	}
	EDIS;
}

void Write_IIC_Command(Uint8 IIC_Command)
{
    IIC_Start();
    DELAY_US(1);
    Write_IIC_Byte(0x78);            //Slave address,SA0=0
    DELAY_US(1);
	IIC_Wait_Ack();	
	DELAY_US(1);
    Write_IIC_Byte(0x00);			//write command
    DELAY_US(1);
	IIC_Wait_Ack();
	DELAY_US(1);
    Write_IIC_Byte(IIC_Command);
    DELAY_US(1);
	IIC_Wait_Ack();
	DELAY_US(1);
    IIC_Stop();
    DELAY_US(1);
}

void Write_IIC_Data(Uint8 IIC_Data)
{
	IIC_Start();
	DELAY_US(1);
	Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
	DELAY_US(1);
	IIC_Wait_Ack();
	DELAY_US(1);
	Write_IIC_Byte(0x40);			//write data
	DELAY_US(1);
	IIC_Wait_Ack();
	DELAY_US(1);
	Write_IIC_Byte(IIC_Data);
	DELAY_US(1);
	IIC_Wait_Ack();	
	DELAY_US(1);
	IIC_Stop();
	DELAY_US(1);
}

void OLED_WR_Byte(Uint8 dat,Uint8 cmd)
{
	if(cmd)
	{
		Write_IIC_Data(dat);
	}
	else 
	{
		Write_IIC_Command(dat);
	}
}

void OLED_Set_Pos(Uint8 x, Uint8 y)
{ 	
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}   	  

void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  
	OLED_WR_Byte(0X14,OLED_CMD);  
	OLED_WR_Byte(0XAF,OLED_CMD);  
}

void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  
	OLED_WR_Byte(0X10,OLED_CMD);  
	OLED_WR_Byte(0XAE,OLED_CMD);  
}		   			 

void OLED_Clear(void)  
{  
	Uint8 i,n;
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);
		OLED_WR_Byte (0x00,OLED_CMD);
		OLED_WR_Byte (0x10,OLED_CMD);
		for(n=0;n<128;n++)
		{
			OLED_WR_Byte(0,OLED_DATA); 
		}
	} 
}

void OLED_On(void)  
{  
	Uint8 i,n;
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);
		OLED_WR_Byte (0x00,OLED_CMD);
		OLED_WR_Byte (0x10,OLED_CMD);
		for(n=0;n<128;n++)
		{
			OLED_WR_Byte(1,OLED_DATA); 
		}
	} 
}

/**
 * @brief show single char at (x,y)
 * 
 * @param x 
 * @param y 
 * @param chr 
 */
void OLED_ShowChar(Uint8 x,Uint8 y,Uint8 chr)
{      	
    Uint8 c=0,i=0;
	c=chr-' ';
	if(x>Max_Column-1)
	{
		x=0;
		y=y++;
	}
	OLED_Set_Pos(x,y);	
	for(i=0;i<6;i++)
	{
		OLED_WR_Byte(F8x6[c*6+i],OLED_DATA);
	}	
}

Uint32 oled_pow(Uint8 m,Uint8 n)
{
    Uint32 result=1;
	while(n--)
	{
		result*=m;
	}    
	return result;
}				  

/**
 * @brief
 *
 * @param x 璧风偣鍧愭爣
 * @param y 璧风偣鍧愭爣
 * @param num 瑕佹樉绀虹殑鏁板瓧
 * @param len 鏁板瓧鐨勪綅鏁�
 */
void OLED_ShowNum(Uint8 x,Uint8 y,Uint32 num,Uint8 len)
{         	
	Uint8 t,temp;
	Uint8 enshow=0;
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if( enshow==0 && t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(6)*t,y,'0');
				continue;
			}
			else 
			{
				enshow=1; 
			}
		}
		OLED_ShowChar(x+6*t,y,temp+'0'); 
	}
} 

/**
 * @brief
 *
 * @param x 璧风偣鍧愭爣
 * @param y 璧风偣鍧愭爣
 * @param num 鏄剧ず鏁板瓧
 * @param len 鏁板瓧闀垮害
 */
void OLED_ShowSignedNum(Uint8 x,Uint8 y,int16 num,Uint8 len)
{
    if(num >= 0)
    {
        OLED_ShowChar(x,y,' ');
        OLED_ShowNum(x+6,y,(Uint32)num,len);
    }
    else
    {
        num = -num;
        OLED_ShowChar(x,y,'-');
        OLED_ShowNum(x+6,y,(Uint32)num,len);
    }
}

/**
 * @brief
 *
 * @param x 璧风偣鍧愭爣
 * @param y 璧风偣鍧愭爣
 * @param num 瑕佹樉绀虹殑鏁板瓧
 * @param len 鏁板瓧鐨勯暱搴︼紝涓嶅寘鎷皬鏁扮偣鍜岃礋鍙�
 * @param Flen 灏忔暟鐨勪綅鏁�
 */
void OLED_ShowFloat(Uint8 x,Uint8 y,float32 num,Uint8 len,Uint8 Flen)
{
    float32 num1;
    Uint8 num2, num3;
    num1 = num;
    num2 = (Uint8)num1;
    num3 = (num - num2) * oled_pow(10, Flen);

    OLED_ShowNum(x,y,num2,len-Flen);
    OLED_ShowChar(x+6*(len-Flen),y,'.');

    OLED_ShowNum(x+6*(len-Flen+1),y,num3,Flen);
}

/**
 * @brief 鏄剧ず鏈夌鍙锋诞鐐规暟
 *
 * @param x 璧风偣鍧愭爣
 * @param y 璧风偣鍧愭爣
 * @param num 瑕佹樉绀虹殑鏁板瓧
 * @param len 鏁板瓧鐨勯暱搴︼紝涓嶅寘鎷皬鏁扮偣鍜岃礋鍙�
 * @param Flen 灏忔暟鐨勪綅鏁�
 */
void OLED_ShowSignedFloat(Uint8 x,Uint8 y,float32 num,Uint8 len,Uint8 Flen)
{
    if(num >= 0)
    {
        OLED_ShowChar(x,y,' ');
        OLED_ShowFloat(x+6,y,num,len,Flen);
    }
    else
    {
        num = -num;
        OLED_ShowChar(x,y,'-');
        OLED_ShowFloat(x+6,y,num,len,Flen);
    }
}

void OLED_ShowString(Uint8 x,Uint8 y,Uint8 *chr)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		
		OLED_ShowChar(x,y,chr[j]);
		x+=6;
		if(x>122)
		{
			x=0;
			y+=1;
		}
		j++;
	}
}

void OLED_Init(void)
{ 	
    EALLOW;
    GpioDataRegs.GPBDAT.bit.GPIO32 = 1;
    GpioDataRegs.GPBDAT.bit.GPIO33 = 1;
    GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;
    GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;
    GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;
    GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;
    GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0;
    GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;
    GpioCtrlRegs.GPBDIR.bit.GPIO33 = 1;
    EDIS;

    DELAY_US(800000);
    OLED_WR_Byte(0xAE,OLED_CMD);//--display off
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD); // contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--128   
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
}  





























