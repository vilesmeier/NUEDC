#include "global.h"

//state
STATE state;

//PR
PR pr_Input,pr_Output;

//PID
PID pi_Input_D,pi_Input_Q,pi_Output,pi_DC,pi_Iint;

//DQ
DQ dq_Input,dq_output;

//SOGI
SPLL_1PH_SOGI sogi_Input,sogi_Output;

//RAMP
RAMP ramp;

//RMS
RMS rms_Cur_Input;
RMS rms_Vol_Input;

//NOTCH
NOTCH notch;

Uint8 xint_select = 0;
float d = 11.5f;
float e = 100.0f;

