#include "global.h"

//PID
PID pid_vol, pid_cur;

//PR
PR pr_vol_A,pr_vol_B,pr_vol_C,pr_cur_A,pr_cur_B,pr_cur_C,pr_VC_A,pr_VC_B,pr_VC_C;

//DQ
DQ dq_vol ,dq_cur;

//RAMP
RAMP ramp;

//STATE
STATE state;

//XINT
Uint8 xint_select = 0;

//RMS
RMS rms_A,rms_B,rms_C;

//SVPWM
SVPWM svpwm;




