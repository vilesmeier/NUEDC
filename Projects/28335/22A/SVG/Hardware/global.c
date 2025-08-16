#include "global.h"

//state
STATE state;

//PR
PR pr_Input;

//PID
PID pi_DC;

//RMS
RMS rms_Vol_Ref;
RMS rms_Vol_Input;

//NOTCH
NOTCH notch;

Uint8 xint_select = 0;
float d = 11.5f;
float e = 100.0f;

