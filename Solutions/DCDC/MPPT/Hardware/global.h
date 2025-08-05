#ifndef GLOBAL_H
#define GLOBAL_H


//include files
#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "adc.h"
#include "oled.h"
#include "pid.h"
#include "pwm.h"
#include "state.h"
#include "xint.h"
#include "uart.h"
#include "mppt.h"

//global definitions
#define PI            3.14159265359f
#define SQUARE_2      1.41421356237f
#define T_ISR         0.00005f
#define T_GRID        0.02f
#define FREQ_ISR      20000
#define FREQ_GRID     50

#define MODE_CURRENT        0
#define MODE_SLOPE_LOW      1
#define MODE_SLOPE_HIGH     2

#define SOFT_START          0
#define NORMAL              1

#define SET_KP 0
#define SET_KI 1

//global variable
extern PID pid_vol;
extern PID pid_cur;

extern STATE state;

extern MPPT_INCC mppt_vol;
extern MPPT_INCC_I mppt_cur;
extern MPPT_PNO mppt_pno;



#endif /* GLOBAL_H_ */
