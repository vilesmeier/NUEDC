#ifndef GLOBAL_H
#define GLOBAL_H

//file include
#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "dq.h"
#include "pwm.h"
#include "svpwm.h"
#include "uart.h"
#include "ramp.h"

//global definitions
#define PI            3.14159265359f
#define SQUARE_2      1.41421356237f
#define T_ISR         0.00005f
#define T_GRID        0.02f
#define FREQ_ISR      20000U
#define FREQ_GRID     50U

//global variable declaration
extern SVPWM svpwm;
extern DQ dq;
extern RAMP ramp;

#endif // !GLOBAL_H
