#ifndef XINT_H
#define XINT_H

void XINT_GPIO_Init(void);
void XINT_Init(void);
interrupt void XINT1_isr(void);
interrupt void XINT2_isr(void);
interrupt void XINT3_isr(void);
interrupt void XINT4_isr(void);
interrupt void XINT5_isr(void);
interrupt void XINT6_isr(void);
interrupt void XINT7_isr(void);

#endif /* XINT_H_ */
