#ifndef GLOBAL_H
#define GLOBAL_H


//include files
#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "adc.h"
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

//global definitions
#define PI            3.14159265359f
#define SQUARE_2      1.41421356237f
#define SQUARE_3      1.732050808f
#define T_ISR         0.00005f
#define T_GRID        0.02f
#define FREQ_ISR      20000
#define FREQ_GRID     50

#define MODE_DISABLE     0
#define MODE_ENABLE      1

#define SOFT_START       0
#define NORMAL           1

#define FREQ_NO_CHANGE   0
#define FREQ_CHANGE      1


//global variable

//PR
extern PR pr_vol_A,pr_vol_C,pr_VC_A,pr_VC_C,pr_rec_a,pr_rec_c;

//ramp
extern RAMP ramp;

//machine state
extern STATE state;

//RMS
extern RMS rms_A,rms_C,rms_Test;

//SVPWM
extern SVPWM svpwm;

#endif /* GLOBAL_H_ */
