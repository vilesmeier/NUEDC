#ifndef CAP_H
#define CAP_H

#include "DSP28x_Project.h"

//struct typedef
typedef struct CAP_DUTY
{
    //CAP1-4 register value
    Uint32 cap1,cap2,cap3,cap4;
    //Duty cycle
    float  duty1,duty2;

} CAP_DUTY;

//config definitions
#define EVENT_POSEDGE  0
#define EVENT_NEGEDGE  1
#define EVENT_NOACTION 0
#define EVENT_RESET    1

#define MODE_COUNT   0
#define MODE_ONECHOT 1
#define MODE_CAP     0
#define MODE_PWM     1

#define CAP_DISABLE 0
#define CAP_ENABLE  1

//functions
void ECAP1_Pherial_Init(void);
void ECAP_Struct_Init(CAP_DUTY *cap);
void ECAP_Init(CAP_DUTY *cap);
void ECAP_Enable(void);
void ECAP_Disable(void);
void ECAP_Get_Duty(CAP_DUTY *cap);
interrupt void ecap1_isr(void);

#endif
