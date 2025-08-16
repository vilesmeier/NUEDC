#ifndef GLOBAL_H
#define GLOBAL_H


//include files
#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "power_meas_sine_analyzer.h"
#include "power_1ph.h"
#include "adc.h"
#include "oled.h"
#include "pr.h"
#include "pwm.h"
#include "rms.h"
#include "state.h"
#include "vsg.h"
#include "uart.h"


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

//PR
extern PR pr_Vol,pr_Cur;

//machine state
extern STATE state;

//XINT
extern Uint8 xint_select;

//RMS
extern RMS Grid_state;

//POWER_1PH
extern POWER power_v;

//VSG
extern VSG vsg;

#endif /* GLOBAL_H_ */
