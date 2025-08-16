#ifndef GLOBAL_H
#define GLOBAL_H


//include files
#include "DSP28x_Project.h"
#include "c28x_fpu_fastrts.h"
#include "adc.h"
#include "pwm.h"
#include "notch.h"
#include "ramp.h"

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

extern NOTCH notch[6];
extern float error[6];
extern float result[6];
extern RAMP ramp;

#endif /* GLOBAL_H_ */
