#ifndef UART_H
#define UART_H

typedef union Change
{
   float data;
   Uint16 dat[2];
}Change;

typedef struct UART_DATA
{
    Change data;
    Uint8 send_float[8];
}UART_DATA;

void UART_Init(Uint32 baud);
void UART_SendChar(Uint8 send);
void UART_ReceiveChar(Uint8 *data);
void UART_VOFA_ShowFloat(float data);

#endif
