#ifndef TIM_H
#define TIM_H

void TIM0_Init(void);
void TIM0_Set(float freq);
void TIM0_Start(void);
void TIM0_Stop(void);
void TIM0_Reload(void);
interrupt void cpu_timer0_isr(void);

#endif
