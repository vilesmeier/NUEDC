#include "global.h"
#include "vofa.h"

// command buffer for SCI-c
UART_DATA_1Ch cmd_ch1;
UART_DATA_2Ch cmd_ch2;
UART_DATA_3Ch cmd_ch3;

/**
 * @brief Init CMD buffer for VOFA
 * 
 */
void VOFA_Init(void)
{
    // Init Frame tail
    cmd_ch1.data.data = 0.0f;
    cmd_ch1.send_float[4] = 0x00;
    cmd_ch1.send_float[5] = 0x00;
    cmd_ch1.send_float[6] = 0x80;
    cmd_ch1.send_float[7] = 0x7f;

    cmd_ch2.data1.data = 0.0f;
    cmd_ch2.data2.data = 0.0f;
    cmd_ch2.send_float[8] = 0x00;
    cmd_ch2.send_float[9] = 0x00;
    cmd_ch2.send_float[10] = 0x80;
    cmd_ch2.send_float[11] = 0x7f;

    cmd_ch3.data1.data = 0.0f;
    cmd_ch3.data2.data = 0.0f;
    cmd_ch3.data3.data = 0.0f;
    cmd_ch3.send_float[12] = 0x00;
    cmd_ch3.send_float[13] = 0x00;
    cmd_ch3.send_float[14] = 0x80;
    cmd_ch3.send_float[15] = 0x7f;
}

#if DSP28_28335
/**
 * @brief send one float to VOFA by Justfloat protocol
 * 
 * @param data transmit data
 */
void VOFA_1Ch_ShowFloat(float data)
{
    cmd_ch1.data.data = data;
    cmd_ch1.send_float[0] = cmd_ch1.data.dat[0];
    cmd_ch1.send_float[1] = (cmd_ch1.data.dat[0]>>8);
    cmd_ch1.send_float[2] = cmd_ch1.data.dat[1];
    cmd_ch1.send_float[3] = (cmd_ch1.data.dat[1]>>8);


    UART_SendString(cmd_ch1.send_float,8);

}

/**
 * @brief send two float to VOFA by Justfloat protocol
 * 
 * @param data1 transmit data 1
 * @param data2 transmit data 2
 */
void VOFA_2Ch_ShowFloat(float data1, float data2)
{
    cmd_ch2.data1.data = data1;
    cmd_ch2.data2.data = data2;

    cmd_ch2.send_float[0] = cmd_ch2.data1.dat[0];
    cmd_ch2.send_float[1] = (cmd_ch2.data1.dat[0]>>8);
    cmd_ch2.send_float[2] = cmd_ch2.data1.dat[1];
    cmd_ch2.send_float[3] = (cmd_ch2.data1.dat[1]>>8);

    cmd_ch2.send_float[4] = cmd_ch2.data2.dat[0];
    cmd_ch2.send_float[5] = (cmd_ch2.data2.dat[0]>>8);
    cmd_ch2.send_float[6] = cmd_ch2.data2.dat[1];
    cmd_ch2.send_float[7] = (cmd_ch2.data2.dat[1]>>8);

    UART_SendString(cmd_ch2.send_float,12);
}

/**
 * @brief send two float to VOFA by Justfloat protocol
 * 
 * @param data1 transmit data 1
 * @param data2 transmit data 2
 * @param data3 transmit data 3
 */
void VOFA_3Ch_ShowFloat(float data1, float data2, float data3)
{
    cmd_ch3.data1.data = data1;
    cmd_ch3.data2.data = data2;
    cmd_ch3.data3.data = data3;

    cmd_ch3.send_float[0] = cmd_ch3.data1.dat[0];
    cmd_ch3.send_float[1] = (cmd_ch3.data1.dat[0]>>8);
    cmd_ch3.send_float[2] = cmd_ch3.data1.dat[1];
    cmd_ch3.send_float[3] = (cmd_ch3.data1.dat[1]>>8);

    cmd_ch3.send_float[4] = cmd_ch3.data2.dat[0];
    cmd_ch3.send_float[5] = (cmd_ch3.data2.dat[0]>>8);
    cmd_ch3.send_float[6] = cmd_ch3.data2.dat[1];
    cmd_ch3.send_float[7] = (cmd_ch3.data2.dat[1]>>8);

    cmd_ch3.send_float[8] = cmd_ch3.data3.dat[0];
    cmd_ch3.send_float[9] = (cmd_ch3.data3.dat[0]>>8);
    cmd_ch3.send_float[10] = cmd_ch3.data3.dat[1];
    cmd_ch3.send_float[11] = (cmd_ch3.data3.dat[1]>>8);

    UART_SendString(cmd_ch3.send_float,16);
}
#endif // DSP28_28335

