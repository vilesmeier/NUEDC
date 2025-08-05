#include "global.h"

void main(void)
{
    //Power On Reset
    InitSysCtrl();
    InitGpio();
    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();
    System_Clock_Close();
    EINT;
    ERTM;

    // Control Loop Init

    
    // Pherial Init
    XINT_Init();
    ADC_Init();
    UART_Init();
    UART_Set(3000000);
    EPWM_Init();

    EPWM_AQ_SoftForce_LOW();



    //Enable Interrupt And JTAG Debug
    EINT;
    ERTM;

    while(1)
    {

    }
}
