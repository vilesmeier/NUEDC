#include "global.h"

//state
STATE state;

//PR
PR pr_Input,pr_Output_Cur;

//PID
PID pi_Input_Cur_D,pi_Input_Cur_Q,pi_Output,pi_DC;

//DQ
DQ dq_Input,dq_output;

//SOGI
SPLL_1PH_SOGI sogi_Input,sogi_Output;

//RAMP
RAMP ramp;

//RMS
RMS rms_Input_Cur;
