#ifndef PWM_H
#define PWM_H

void EPWM_Init(void);
void EPWM_ON(void);
void EPWM_OFF(void);
void Rectifier_ON(void);
void Rectifier_OFF(void);
void Inverter_ON(void);
void Inverter_OFF(void);
interrupt void epwm1_isr(void);

#endif /* EPWM_H_ */
