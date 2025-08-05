#ifndef PWM_H
#define PWM_H

void EPWM1_Init(void);
void EPWM2_Init(void);
void EPWM3_Init(void);
void EPWM4_Init(void);
void EPWM_Init(void);
void EPWM_ON(void);
void EPWM_OFF(void);
interrupt void epwm1_isr(void);

#endif /* EPWM_H_ */
