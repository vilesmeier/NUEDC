#ifndef GLOBAL_H
#define GLOBAL_H

//file include
#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "power_meas_sine_analyzer.h"
#include "spll_1ph_sogi.h"
#include "adc.h"
#include "control.h"
#include "dq.h"
#include "oled.h"
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
#define FREQ_ISR      20000U
#define FREQ_GRID     50U

//global variable declaration



#endif // !GLOBAL_H
