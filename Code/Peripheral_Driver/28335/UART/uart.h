#ifndef UART_H
#define UART_H

void UART_Init();
void UART_Set(Uint32 baud);
void UART_SendChar(Uint8 send);
void UART_ReceiveChar(Uint8 *data);
void UART_SendString(Uint8 *data, Uint8 length);
void UART_ReceiveString(Uint8 *data, Uint8 length);

#endif
