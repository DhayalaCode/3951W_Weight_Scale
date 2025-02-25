
/**************************************************************
 * Author: Dhayalan Balasubramanian
 * Date: March 17, 2024
 * Student ID number: 4754648
 * Course number: EE2361
 * Term: Spring
 * Lab/assignment number: 5
 **************************************************************/

/*
 * File:   lcd.c
 * Author: Dhayalan Balasubramanian
 *
 * Created on March 18, 2024, 11:51 AM
 * Purpose of this file: I2C LCD library for the AQM0802A-RN-GBW and ST7032 - Controller chip.
 * This library has initialization, command(for more functions), animation, print char/string, and
 * a clear function.
 */



#include "xc.h"
#include <string.h>

/* 
 * Delays execution for the specified number of milliseconds.
 * ms: The number of milliseconds to delay.
 */
void delay_ms(unsigned int ms) {
    while(ms-- > 0) {
        asm("repeat #15998");
        asm("nop");
    }
}

void lcd_init(void) {
    I2C2BRG = 0x9D; //set to a clock freq of 100kHz and 16MHz Fcy/ BAUD RATE
    IFS3bits.MI2C2IF = 0; // clr Int flag
    I2C2CONbits.I2CEN = 1; //Enable I2C mode
    
    delay_ms(40);
    
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00111001); // function set, extended instruction mode
    lcd_cmd(0b00010100); // interval osc
    lcd_cmd(0b01110000); // contrast C3-C0
    lcd_cmd(0b01011110); // Ion, Bon, C5-C4
    lcd_cmd(0b01101100); // follower control

    
    delay_ms(200);
    
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00001100); // Display On
    lcd_cmd(0b00000001); // Clear Display

    
    delay_ms(1);

    
    
}

//lcd command function which sends proper control bits using I2C control register and polling.
void  lcd_cmd(char Package) {
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.SEN = 1; //sending start bit
    while(I2C2CONbits.SEN==1); //waiting for zero
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b01111100;
    while(IFS3bits.MI2C2IF==0);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b00000000;
    while(IFS3bits.MI2C2IF==0);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = Package; //input goes here
    while(IFS3bits.MI2C2IF==0);
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN==1);
    IFS3bits.MI2C2IF = 0;
}  

/*function with a masked and calculated command to set
cursor to row y, column x on the display.
 */
void lcd_setCursor(char x, char y){
    char position = 0x40 * y + x;
    lcd_cmd((1 << 7) + position);
};

/*
 * Prints a singular character.
 */
void lcd_printChar(char myChar){
    I2C2CONbits.SEN = 1; //sending start bit
    while(I2C2CONbits.SEN == 1);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = 0b01111100; // 8-bits consisting of the slave address and the R/nW bit
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = 0b01000000; // 8-bits consisting of control byte /w RS=1
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = myChar;
    while(IFS3bits.MI2C2IF==0);
    
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN==1); // PEN will clear when Stop bit is complete
    IFS3bits.MI2C2IF = 0;
};  

//clears the entire lcd screen.
void lcd_clr(){
    lcd_cmd(0b00000001);
}

/*animates the lcd screen and moves the given string to the left
over and over again by sending one lcd command. 
 */
void lcd_animate(){
    lcd_cmd(0b00011000); 
}

/*Prints a string without using the print char function.
This gives the proper overhead bits and then iterates through every char and prints it.
 */
void lcd_printStr(const char s[]) {
    int len = strlen(s);
    I2C2CONbits.SEN = 1;        //Start
        while(I2C2CONbits.SEN);
    IFS3bits.MI2C2IF=0;
    I2C2TRN= 0b01111100;        //Slave address and R/W bit
        while(!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF=0;
   
    for (int i=0; i < len - 1; i++) {
        I2C2TRN = 0b11000000;       //Control byte, RS = 1
            while(!IFS3bits.MI2C2IF);
        IFS3bits.MI2C2IF=0;
        I2C2TRN = s[i];          
            while(!IFS3bits.MI2C2IF);
        IFS3bits.MI2C2IF=0;
    }
    I2C2TRN = 0b01000000;       //Control byte, RS = 0
    while(!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF=0;
    I2C2TRN = s[len-1];         
    while(!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF=0;
    I2C2CONbits.PEN = 1;        
    while(I2C2CONbits.PEN);
}
