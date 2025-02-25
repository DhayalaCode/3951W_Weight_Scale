/*
 * File:   voltmeter.c
 * Author: jansiranibalasubramanian
 *
 * Created on April 8, 2024, 9:36 AM
 */


#include "xc.h"
#define BUFFSIZE 1024
#define NUMSAMPLES 128
#define K 128

int adc_buffer[BUFFSIZE];
int buffer_index = 0;
int write_index = 0;
int writeToLcd = 0;

/*
 * Interrupt service routine for the ADC1 interrupt. 
 * Everytime interrupt flag is 1, we put a value in the buffer.
 */
void __attribute__((__interrupt__, __auto_psv__)) _ADC1Interrupt(void) {
    _AD1IF = 0; // Clear Timer 1 interrupt flag
    putVal(ADC1BUF0); // Increment counter
}
/*
 * Interrupt service routine for the IC1 interrupt. 
 * Everytime interrupt flag is 1, we write the running avg to LCD screen.
 */
void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void) {
    _T1IF = 0; // Clear Timer 1 interrupt flag
    writeToLcd = 1;
}



/*
 * Initialize the ADC interrupt
 */
void adc_init() {
    TRISAbits.TRISA0 = 1; //inputs
    AD1PCFGbits.PCFG0 = 0;//I/O setup
    
    AD1CON2bits.VCFG = 0b000; // vdd(3.3) and vss(gnd)
    AD1CON3bits.ADCS = 0b00000001; //tad - 75ns
    AD1CON1bits.SSRC = 0b010; // sample on timer 3
    AD1CON3bits.SAMC = 0b00001; //1 auto sample time bit
    AD1CON1bits.FORM = 0b00; //unsigned bit form 
    
    AD1CON1bits.ASAM = 0b1; //turning on sampling
    AD1CON2bits.SMPI = 0b0000; //1/16 sample conversion rate
    AD1CON1bits.ADON = 1; // turn on adc
    
    _AD1IF = 0; // clear interrupt flag
    _AD1IE = 1; //enable the interrupt
    
    TMR3 = 0; //initialize timer3
    T3CON = 0;
    T3CONbits.TCKPS = 0b11;
    //1/k = tcy*64*PR3
    
    PR3 = 62499 / K;
    T3CONbits.TON = 1;
  
}

/*
 * basic 100ms timer, no interrupts;
 */
void timer1_init() {
    T1CON = 0; //clear
    TMR1 = 0; // Clear Timer 1 value
    T1CONbits.TCKPS = 0b10;
    PR1 = 24999; // Set period for 100ms with a 16 MHz clock (Fcy = 16 MHz)
    IFS0bits.T1IF = 0; // Clear Timer 1 interrupt flag (NO INTERRUPT)
    T1CONbits.TON = 1; // Turn on Timer 1
    IEC0bits.T1IE = 1;
}

/*
 * The getAVG function loops through the buffer and returns the average.
 */
int getAvg() {
    long int sum = 0;
    int start_index = (write_index - NUMSAMPLES + BUFFSIZE) % BUFFSIZE; 
    for (int i = 0; i < NUMSAMPLES; i++) {
        int index = (start_index + i) % BUFFSIZE; //index at which we are looking at.
        sum += adc_buffer[index];
    }
    return sum / NUMSAMPLES;
}


/*
 * initializes all buffer entries to zero.
*/
void initBuffer() {
    for(int i = 0; i < BUFFSIZE; i++) {
        adc_buffer[i] = 0;
    }
}
/*
 * This function puts all of the Values in the the buffer linearly.
 */
void putVal(int ADCvalue) {
    adc_buffer[write_index++] = ADCvalue; //writing the ADC val in buffer.
    write_index %= BUFFSIZE; 
}
