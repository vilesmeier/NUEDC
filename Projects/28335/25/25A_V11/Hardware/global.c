#include "global.h"

//PR
PR pr_vol_A,pr_vol_C,pr_cur_A,pr_cur_C,pr_rec_a,pr_rec_c;

//RAMP
RAMP ramp;

//STATE
STATE state;

//RMS
RMS rms_A,rms_C,rms_Test,rms_vol_A;

//SVPWM
SVPWM svpwm,svpwm_rec;

//FILTER
RC_LPF rc_rec_a,rc_rec_c;

// 3ph power analyzer
POWER_3PH power;
