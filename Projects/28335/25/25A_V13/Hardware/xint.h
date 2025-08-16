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

//#pragma CODE_SECTION(XINT_GPIO_Init, ".TI.ramfunc");
//#pragma CODE_SECTION(XINT_Init, ".TI.ramfunc");
//#pragma CODE_SECTION(XINT1_isr, ".TI.ramfunc");
//#pragma CODE_SECTION(XINT2_isr, ".TI.ramfunc");
//#pragma CODE_SECTION(XINT3_isr, ".TI.ramfunc");
//#pragma CODE_SECTION(XINT4_isr, ".TI.ramfunc");
//#pragma CODE_SECTION(XINT5_isr, ".TI.ramfunc");
//#pragma CODE_SECTION(XINT6_isr, ".TI.ramfunc");
//#pragma CODE_SECTION(XINT7_isr, ".TI.ramfunc");


#endif /* XINT_H_ */
