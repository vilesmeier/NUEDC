#ifndef GLOBAL_H
#define GLOBAL_H


//include files
#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "power_meas_sine_analyzer.h"
#include "spll_1ph_sogi.h"
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

//global definitions
#define PI            3.14159265359f
#define SQUARE_2      1.41421356237f
#define T_ISR         0.00005f
#define T_GRID        0.02f
#define FREQ_ISR      20000
#define FREQ_GRID     50

#define MODE_VOLTAGE   0
#define MODE_CURRENT   1
#define MODE_UNKNOWN   2

#define OUTPUT_DISABLE 0
#define OUTPUT_ENABLE  1

#define SELECT_VOL     0
#define SELECT_I_TOTAL 1
#define SELECT_K_I     2

#define SOFT_START     0
#define NORMAL         1

//global variable

//PID
extern PID pid_Vol,pid_Cur;
extern PID* pid_Vol_Addr;
extern PID* pid_Cur_Addr;

//PR
extern PR pr_Vol,pr_Cur;
extern PR* pr_Vol_Addr;
extern PR* pr_Cur_Addr;

//SOGIPLL
extern SPLL_1PH_SOGI sogi_Vol;
extern SPLL_1PH_SOGI* sogi_Vol_Addr;

//DQ
extern DQ dq_Vol , dq_Cur;
extern DQ* dq_Vol_Addr;
extern DQ* dq_Cur_Addr;

//ramp
extern RAMP ramp;
extern RAMP* ramp_Addr;

//machine state
extern STATE state;

//XINT
extern Uint8 xint_select;

//RMS
extern RMS Grid_state;

#endif /* GLOBAL_H_ */
