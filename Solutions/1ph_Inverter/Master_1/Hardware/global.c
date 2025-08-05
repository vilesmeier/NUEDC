#include "global.h"

//PID
PID pid_Vol, pid_Cur;
PID* pid_Vol_Addr = &pid_Vol;
PID* pid_Cur_Addr = &pid_Cur;

//PR
PR pr_Vol,pr_Cur;
PR* pr_Vol_Addr = &pr_Vol;
PR* pr_Cur_Addr = &pr_Cur;

//SOGI
SPLL_1PH_SOGI sogi_Vol;
SPLL_1PH_SOGI* sogi_Vol_Addr = &sogi_Vol;

//DQ
DQ dq_Vol ,dq_Cur;
DQ* dq_Vol_Addr = &dq_Vol;
DQ* dq_Cur_Addr = &dq_Cur;

//RAMP
RAMP ramp;
RAMP* ramp_Addr = &ramp;

//STATE
STATE state;

//XINT
Uint8 xint_select = 0;

//RMS
RMS Grid_state;
