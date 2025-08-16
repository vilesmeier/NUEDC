#ifndef PWM_H
#define PWM_H

void EPWM1_Init(void);
void EPWM2_Init(void);
void EPWM3_Init(void);
void EPWM4_Init(void);
void EPWM5_Init(void);
void EPWM6_Init(void);
void EPWM_Init(void);
void EPWM_AQ_Inverter_SoftForce_OFF(void);
void EPWM_AQ_Inverter_SoftForce_LOW(void);
void EPWM_AQ_Rectifier_SoftForce_OFF(void);
void EPWM_AQ_Rectifier_SoftForce_LOW(void);
void EPWM_TZ_Software_OST_Set(void);
void EPWM_TZ_Software_CBC_Set(void);
void EPWM_TZ_Software_OST_Clear(void);
void EPWM_TZ_Software_CBC_Clear(void);
interrupt void epwm1_isr(void);

#endif /* EPWM_H_ */
