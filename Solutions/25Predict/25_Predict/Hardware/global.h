#ifndef GLOBAL_H
#define GLOBAL_H

//file include
#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "adc.h"
#include "filter.h"
#include "notch.h"
#include "oled.h"
#include "pid.h"
#include "pir.h"
#include "power_1ph.h"
#include "pr.h"
#include "pwm.h"
#include "ramp.h"
#include "rms.h"
#include "state.h"
#include "uart.h"
#include "xint.h"

//global definitions
#define PI            3.1415926535897f
#define SQUARE_2      1.4142135623730f
#define SQUARE_3      1.7320508075688f
#define T_ISR         0.00005f
#define T_GRID        0.02f
#define FREQ_ISR      20000U
#define FREQ_GRID     50U

// state machine
// ALL flag should init at 0
// 0 means start state of transformer
// VDC protect
#define FLAG_VDC_NORMAL 0
#define FLAG_VDC_OV     1

// DC capacitor pre-charge
#define FLAG_VDC_LOSS   0
#define FLAG_VDC_SET    1

// Inverter output enable
#define FLAG_INV_DIS    0
#define FLAG_INV_EN     1

// Inverter soft-start
#define FLAG_INV_SOFT   0
#define FLAG_INV_NORMAL 1


//global variable declaration
//state machine
extern STATE state;

extern RAMP ramp;

extern PID pid_vdc;

extern PR pr_input, pr_output_a, pr_output_c;

extern RMS rms_input, rms_ref;

extern NOTCH notch_vdc;

extern POWER power_input;

#endif // !GLOBAL_H
