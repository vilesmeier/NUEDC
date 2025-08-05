#ifndef VSG_H
#define VSG_H

// Virtual Impedance
typedef struct VIR_Z
{
    //user input data
    float L;                    // total inductor value of output filter
    float r;                    // total serial resistor of inductor
    float input;                // virtual impedace input signal
    float fsw,delta_t;          // step length of discrete system

    //output data
    float output;               // output data

    // temp data
    float A0,B0,B1;             // parameter for difference equetion
    float input_last;           // input value
    float output_last;          // output value
} VIR_Z;

// VSG
typedef struct VSG
{
    // User input value
    float fsw,delta_t;          // step length of discrete system
    float U_rms_nom;            // nominal output amplitude
    float U_grid;               // grid voltage for virtual impedance
    float P_ref,Q_ref;          // set target power value
    float P,Q;                  // actual output power value
    float J,D;                  // parameter for VSG
    float w0;                   // nominal omega
    float clamp_delta_w_upper;  // upper limit of delta w
    float clamp_delta_w_lower;  // lower limit of delta w

    // VSG output value
    float I_ref;                // VSG output Currnent ref signal
    float wt;                   // output ref angle

    // temp value
    float delta_w;              // adjust value of w
    float w;                    // output omega
    float Uamp;                 // Amplitude of output voltage
    float U_ref;                // VSG output Voltage ref signal
    float pw_A0,pw_B0,pw_B1;    // parameter for transfer function of P-W
    float pw_result_last;       // output stroge
    float pw_err_last;          // err stroge
    float w_last;               // w stroge
    float T_input;              // P-w transfer function input

    // Virtual Inpedance for VSG
    VIR_Z vir;
} VSG;


void VSG_Init(VSG *vsg);
void VSG_Set(VSG *vsg, float fsw, float w0, float u_rms_nom, float Pset, float Qset, float D, float J, float L, float r, float upper, float lower);
void VSG_Run(VSG *vsg, float P, float Q, float Ugrid);
void VSG_Pre_Sync(VSG *vsg);

void VIR_Z_Init(VIR_Z *vir);
void VIR_Z_Set(VIR_Z *vir, float fsw, float L, float r);
void VIR_Z_Run(VIR_Z *vir, float input);

#endif // !VSG_H
