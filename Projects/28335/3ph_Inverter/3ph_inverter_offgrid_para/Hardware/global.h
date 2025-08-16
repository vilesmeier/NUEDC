#ifndef GLOBAL_H
#define GLOBAL_H


//include files
#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "power_meas_sine_analyzer.h"
#include "adc.h"
#include "dq.h"
#include "oled.h"
#include "pid.h"
#include "pr.h"
#include "pwm.h"
#include "ramp.h"
#include "rms.h"
#include "state.h"
#include "xint.h"
#include "svpwm.h"
#include "uart.h"
#include "pll_3ph.h"

//global definitions
#define PI            3.14159265359f
#define SQUARE_2      1.41421356237f
#define T_ISR         0.00005f
#define T_GRID        0.02f
#define FREQ_ISR      20000
#define FREQ_GRID     50

#define MODE_UNKNOWN     0
#define MODE_VOLTAGE     1
#define MODE_CURRENT     2

#define OUTPUT_ENABLE    0
#define OUTPUT_DISABLE   1

#define SOFT_START       0
#define NORMAL           1

//global variable

//PID
extern PID pid_vol,pid_cur;

//PR
extern PR pr_vol_A,pr_vol_B,pr_vol_C,pr_cur_A,pr_cur_B,pr_cur_C,pr_VC_A,pr_VC_B,pr_VC_C;

//DQ
extern DQ dq_vol, dq_cur;

//ramp
extern RAMP ramp;

//machine state
extern STATE state;

//XINT
extern Uint8 xint_select;

//RMS
extern RMS rms_A,rms_B,rms_C;

//SVPWM
extern SVPWM svpwm;

//PLL_3PH
extern SRF_3PH srf_3ph;

#endif /* GLOBAL_H_ */
