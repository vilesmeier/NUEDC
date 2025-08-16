#ifndef OLED_H
#define OLED_H

#include "global.h"

#define Max_Column	128
#define Max_Row		64
#define OLED_CMD  0	//Ð´ÃüÁî
#define OLED_DATA 1	//Ð´Êý¾Ý
#define OLED_SDA_Set()        GpioDataRegs.GPBDAT.bit.GPIO32=1
#define OLED_SDA_Clr()        GpioDataRegs.GPBDAT.bit.GPIO32=0
#define OLED_SCK_Set()        GpioDataRegs.GPBDAT.bit.GPIO33=1
#define OLED_SCK_Clr()        GpioDataRegs.GPBDAT.bit.GPIO33=0


void IIC_Start();
void IIC_Stop();
void Write_IIC_Command(Uint8 IIC_Command);
void Write_IIC_Data(Uint8 IIC_Data);
void Write_IIC_Byte(Uint8 IIC_Byte);
void IIC_Wait_Ack();
void OLED_WR_Byte(Uint8 dat,Uint8 cmd);
void OLED_Set_Pos(Uint8 x, Uint8 y);
void OLED_Display_On(void);
void OLED_Display_Off(void);
Uint32 OLED_Pow(Uint8 m,Uint8 n);
void OLED_On(void);
void OLED_Clear(void);
void OLED_Init(void);
void OLED_ShowChar(Uint8 x,Uint8 y,Uint8 chr);
void OLED_ShowString(Uint8 x,Uint8 y,Uint8 *chr);
void OLED_Set_Pos(Uint8 x, Uint8 y);
void OLED_ShowNum(Uint8 x,Uint8 y,Uint32 num,Uint8 len);
void OLED_ShowSignedNum(Uint8 x,Uint8 y,int16 num,Uint8 len);
void OLED_ShowFloat(Uint8 x,Uint8 y,float32 num,Uint8 len,Uint8 Flen);
void OLED_ShowSignedFloat(Uint8 x,Uint8 y,float32 num,Uint8 len,Uint8 Flen);




#endif  
	 



