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

//Global Variables

//state
extern STATE state;

//PR
extern PR pr_Input,pr_Output_Cur;

//PID
extern PID pi_Input_Cur_D,pi_Input_Cur_Q,pi_Output,pi_DC;

//DQ
extern DQ dq_Input,dq_output;

//SOGI
extern SPLL_1PH_SOGI sogi_Input,sogi_Output;

//RAMP
extern RAMP ramp;

//RMS
extern RMS rms_Input_Cur;

#endif /* GLOBAL_H_ */
