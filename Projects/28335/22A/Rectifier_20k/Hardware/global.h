#ifndef GLOBAL_H
#define GLOBAL_H


//include files
#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "power_meas_sine_analyzer.h"
#include "adc.h"
#include "oled.h"
#include "pid.h"
#include "pr.h"
#include "pwm.h"
#include "rms.h"
#include "state.h"
#include "xint.h"
#include "notch.h"
#include "uart.h"

//global definitions
#define PI            3.14159265359f
#define SQUARE_2      1.41421356237f
#define T_ISR         0.00005f
#define T_GRID        0.02f
#define FREQ_ISR      20000
#define FREQ_GRID     50

//Flags Definition
#define OUTPUT_ENABLE  1
#define OUTPUT_DISABLE 0

#define INPUT_ENABLE   1
#define INPUT_DISABLE  0

#define SOFT_START     1
#define NORMAL         0

#define L       1.0f
#define C       -1.0f
#define R       0.0f

#define PR_KP 0
#define PR_KR 1

//Global Variables

//state
extern STATE state;

//PR
extern PR pr_Input,pr_Output;

//PID
extern PID pi_Input_D,pi_Input_Q,pi_Output,pi_DC,pi_Iint;

//RMS
extern RMS rms_Cur_Input;
extern RMS rms_Vol_Input;

//NOTCH
extern NOTCH notch;

extern Uint8 xint_select;
extern float d;
extern float e;

#endif /* GLOBAL_H_ */
