#ifndef GLOBAL_H
#define GLOBAL_H


//include files
#include "DSP28x_Project.h"
#include "power_3ph.h"
#include "pwm.h"
#include "ramp.h"
#include "c28x_fpu_fastrts.h"
#include "uart.h"
#include "rms.h"

#define PI 3.1415926535f

extern RAMP ramp;

extern POWER_3PH v;

//extern RMS va,vb,vc,ia,ib,ic;


#endif /* GLOBAL_H_ */
