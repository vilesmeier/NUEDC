#ifndef ADCA_H
#define ADCA_H

void ADC_Init(void);
void ADC_Get_Value(void);
interrupt void ADC_int_isr(void);

//#pragma CODE_SECTION(ADC_Init, ".TI.ramfunc");
#pragma CODE_SECTION(ADC_Get_Value, ".TI.ramfunc");
#pragma CODE_SECTION(ADC_int_isr, ".TI.ramfunc");

#endif /* ADCA_H_ */
