#ifndef ADC_H
#define ADC_H

void ADC_Init(void);
void ADC_Get_Value(void);
interrupt void ADC_int_isr(void);

#endif /* ADC_H_ */
