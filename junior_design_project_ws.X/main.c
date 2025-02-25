/**************************************************************
 * Author: Dhayalan Balasubramanian 
 * Date: April 2, 2024
 * Student ID number: 4754648
 * Course number: EE2361
 * Term: Spring
 * Lab/assignment number: 5
 **************************************************************/
//HIIIII rob
/*
 * Lab purpose: 

 */


#include "xc.h"
#include "lcd.h"
#include "voltmeter.h"
#include <stdio.h>

//SDA2 AND SCL2  PIN 6 AND 7
//upload lab 5 hex file to check
//master is the PIC24
// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)


// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled, 
                                       // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL
extern int writeToLcd;

/*
 * initialize the PIC24 microcontroller, 
 * mostly done to setup the LCD. 
 */
void pic24_init() {
    CLKDIVbits.RCDIV = 0;  //clock to 16
    AD1PCFG = 0xffff; //all pins digital
    TRISAbits.TRISA0 = 0; 
    I2C2BRG = 0x9D;
    I2C2CONbits.I2CEN = 1;
    _I2CSIDL = 0;
    IFS3bits.MI2C2IF=0;
}



int main(void) {
    pic24_init();
    lcd_init();
    adc_init();
    timer1_init();
    initBuffer();
    float voltage = 0;
    
       while (1) {
         
           if (writeToLcd == 1) {
                char adStr[20];
                //_AD1IF = 0;
                // Update LCD every 100ms
                voltage = (3.3 / 1024) * getAvg(); // Calculate average voltage
                sprintf(adStr, "%6.4f V", voltage); // Format voltage string
                //lcd_clr();
                lcd_setCursor(0, 0);

                lcd_printStr(adStr); // Display voltage on LCD

                lcd_setCursor(0,1);
                lcd_printStr("ADC LAB6");
                
                writeToLcd = 0;
           }
       }   
    
    return 0;
}
