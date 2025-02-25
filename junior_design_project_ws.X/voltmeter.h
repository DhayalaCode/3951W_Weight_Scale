/**************************************************************
 * Author: Dhayalan Balasubramanian
 * Date: March 17, 2024
 * Student ID number: 4754648
 * Course number: EE2361
 * Term: Spring
 * Lab/assignment number: 6
 **************************************************************/ 
#ifndef VOLTMETER_H
#define	VOLTMETER_H

#include <xc.h> // include processor files - each processor file is guarded.  

#ifdef	__cplusplus
extern "C" {
#endif

void __attribute__((__interrupt__, __auto_psv__)) _ADC1Interrupt(void);
void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void);
void adc_init();
void timer1_init();
int getAvg();
void initBuffer() ;
void putVal(int ADCvalue);

#ifdef	__cplusplus
}
#endif

#endif	/* VOLTMETER_H */



