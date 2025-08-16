#ifndef OLED_H
#define OLED_H

#define OLED_CMD  0 //写命令
#define OLED_DATA 1 //写数据

#define OLED_ADDR  0x3C  //OLED的从机地址
#define Max_Column  128
#define Max_Row     64

void OLED_Init(void);
void WriteData(Uint8 Wdata, Uint8 mode);
Uint32 OLED_Pow(Uint8 m,Uint8 n);
void OLED_Set_Pos(Uint8 x, Uint8 y);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Clear(void);
void OLED_ShowChar(Uint8 x,Uint8 y,Uint8 chr);
void OLED_ShowString(Uint8 x,Uint8 y,Uint8 *chr);
void OLED_ShowNum(Uint8 x,Uint8 y,Uint32 num,Uint8 len);
void OLED_ShowSignedNum(Uint8 x,Uint8 y,int16 num,Uint8 len);
void OLED_ShowFloat(Uint8 x,Uint8 y,float32 num,Uint8 len,Uint8 Flen);
void OLED_ShowSignedFloat(Uint8 x,Uint8 y,float32 num,Uint8 len,Uint8 Flen);

//#pragma CODE_SECTION(OLED_Init, ".TI.ramfunc");
//#pragma CODE_SECTION(WriteData, ".TI.ramfunc");
//#pragma CODE_SECTION(OLED_Pow, ".TI.ramfunc");
//#pragma CODE_SECTION(OLED_Set_Pos, ".TI.ramfunc");
//#pragma CODE_SECTION(OLED_DisPlay_On, ".TI.ramfunc");
//#pragma CODE_SECTION(OLED_DisPlay_Off, ".TI.ramfunc");
//#pragma CODE_SECTION(OLED_ShowChar, ".TI.ramfunc");
//#pragma CODE_SECTION(OLED_Clear, ".TI.ramfunc");
//#pragma CODE_SECTION(OLED_ShowString, ".TI.ramfunc");
//#pragma CODE_SECTION(OLED_ShowNum, ".TI.ramfunc");
//#pragma CODE_SECTION(OLED_ShowSignedNum, ".TI.ramfunc");
//#pragma CODE_SECTION(OLED_ShowFloat, ".TI.ramfunc");
//#pragma CODE_SECTION(OLED_ShowSignedFloat, ".TI.ramfunc");

#endif
