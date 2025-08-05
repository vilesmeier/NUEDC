#ifndef UART_H
#define UART_H

// transfer data type by union
typedef union Change
{
    float data;
    Uint16 dat[2];
}Change;

//data package and union
typedef struct UART_DATA_1Ch
{
    Change data;
    Uint8 send_float[8];
}UART_DATA_1Ch;

typedef struct UART_DATA2_Ch
{
    Change data1;
    Change data2;
    Uint8 send_float[12];
}UART_DATA_2Ch;

void UART_Init();
void UART_Set(Uint32 baud);
void UART_SendChar(Uint8 send);
void UART_ReceiveChar(Uint8 *data);
void UART_VOFA_1Ch_ShowFloat(float data);
void UART_VOFA_2Ch_ShowFloat(float data1,float data2);

#endif
