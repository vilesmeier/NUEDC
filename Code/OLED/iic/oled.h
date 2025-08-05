#ifndef OLED_H
#define OLED_H

#define OLED_CMD  0 //写命令
#define OLED_DATA 1 //写数据

#define devAddr  0x3C  //OLED的从机地址
#define Max_Column  128
#define Max_Row     64

Uint16  WriteData( Uint16 SlaveAddress, Uint16 RomAddress,Uint16 number, Uint8 *Wdata);
void IICwriteBit(Uint16 slaveaddress, Uint16 regaddress, Uint8 bitNum, Uint8 data);
void IICwriteBits(Uint16 slaveaddress,Uint16 regaddress,Uint8 bitStart,Uint8 length,Uint8 data);
Uint16  I2C_xrdy();
Uint16  I2C_rrdy();
void Send_Byte(Uint8 dat,Uint16 rom);
void OLED_WR_Byte(Uint8 dat,Uint8 mode);
void OLED_Set_Pos(Uint8 x, Uint8 y);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Clear(void);
void OLED_ShowChar(Uint8 x,Uint8 y,Uint8 chr);
void OLED_ShowString(Uint8 x,Uint8 y,Uint8 *chr);
Uint32 OLED_Pow(Uint8 m,Uint8 n);
void OLED_ShowNum(Uint8 x,Uint8 y,Uint32 num,Uint8 len);
void OLED_Init(void);

#endif
