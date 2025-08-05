#ifndef GLOBAL_H
#define GLOBAL_H


//include files
#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "adc.h"
#include "filter.h"
#include "oled.h"
#include "pid.h"
#include "power_3ph.h"
#include "pr.h"
#include "pwm.h"
#include "ramp.h"
#include "rms.h"
#include "state.h"
#include "svpwm.h"
#include "uart.h"
#include "xint.h"

//global definitions
#define PI            3.1415926535897f
#define SQUARE_2      1.4142135623730f
#define SQUARE_2_INV  0.7071067811865f
#define SQUARE_3      1.7320508075688f
#define SQUARE_3_INV  0.5773502691896f
#define T_ISR         0.00005f
#define T_GRID        0.02f
#define FREQ_ISR      20000
#define FREQ_GRID     50
#define VDC           60.0f

#define MODE_DISABLE     0
#define MODE_ENABLE      1

#define SOFT_START       0
#define NORMAL           1

#define FREQ_NO_CHANGE   0
#define FREQ_CHANGE      1

#define CAP_NOT_CHARGE   0
#define CAP_CHARGE_END   1

#define MODE_VOLTAGE     0
#define MODE_CURRENT     1

//global variable

//PR
extern PR pr_vol_A,pr_vol_C,pr_cur_A,pr_cur_C,pr_rec_a,pr_rec_c;

//ramp
extern RAMP ramp;

//machine state
extern STATE state;

//RMS
extern RMS rms_A,rms_C,rms_Test,rms_vol_A;

//SVPWM
extern SVPWM svpwm,svpwm_rec;

//FILTER
extern RC_LPF rc_rec_a;
extern RC_LPF rc_rec_c;

// 3ph power analyzer
extern POWER_3PH power;

#endif /* GLOBAL_H_ */
