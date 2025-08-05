#ifndef ADCA_H
#define ADCA_H


void ADC_Init(void);
void ADC_Get_Value(void);
interrupt void ADC_int_isr(void);

#endif /* ADCA_H_ */
