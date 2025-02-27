#include "I2C.h"


void init_I2C1(void){
    I2C1BRG = 37; //set to a clock frequency of 400 kHz section 16.3 PIC24 D.S.
    IFS1bits.MI2C1IF = 0; //Clear Interrupt Flag
    I2C1CONbits.I2CEN = 1; //Enable I2C Mode
}

void I2C1_start() {
    I2C1CONbits.SEN = 1;   // Initiate start condition
    while (I2C1CONbits.SEN);
};

void I2C1_stop() {
    I2C1CONbits.PEN = 1;   // Initiate stop condition
    while (I2C1CONbits.PEN);
}
// Function to write data over I2C
void I2C1_write(uint8_t data) {
    I2C1TRN = data;        //  I2C transimit reg
    while (I2C1STATbits.TRSTAT); // Wait for transmission to complete
}

//MAX30102 I2C now
void I2C_start() {
    I2C2CONbits.SEN = 1;   // Initiate start condition
    while (I2C2CONbits.SEN);
};

void I2C_stop() {
    I2C2CONbits.PEN = 1;   // Initiate stop condition
    while (I2C2CONbits.PEN);
};
// Function to write data over I2C
void I2C_write(uint8_t data) {
    I2C2TRN = data;        //  I2C transimit reg
    while (I2C2STATbits.TRSTAT); // Wait for transmission to complete
}

// Function to read data over I2C
uint32_t I2C_read() {
    I2C2RCV = 0;
    I2C2CONbits.RCEN = 1; // Enable receive mode
    while (I2C2CONbits.RCEN == 1);
    return I2C2RCV;        // I2C receive re
}

void I2C_repeated_start() {
    I2C2CONbits.RSEN = 1;  // Initiate repeated start condition
    while (I2C2CONbits.RSEN == 1);
    //while (I2C2CONbits.RSEN); // Wait for repeated start condition to complete
}

void I2C_ACK() {
    I2C2CONbits.ACKDT = 0;
    I2C2CONbits.ACKEN = 1;
    while (I2C2CONbits.ACKEN == 1);
}

void I2C_NACK() {
    I2C2CONbits.ACKDT = 1;
    I2C2CONbits.ACKEN = 1;
    while (I2C2CONbits.ACKEN == 1);
}

uint8_t I2C_read_with_params(uint8_t reg) {
    I2C_start();

    // Send device address with write bit
    I2C_write(MAX30102_ADDRESS_WRITE);

    // Send register address
    I2C_write(reg);

    // Restart I2C communication
    I2C_repeated_start();

    // Send device address with read bit
    I2C_write(MAX30102_ADDRESS_READ);

    // Read received data
    uint8_t data = I2C_read();
    
    I2C2CONbits.ACKDT = 1;
        
    I2C2CONbits.ACKEN = 1;
    while (I2C2CONbits.ACKEN == 1);

    // Stop I2C communication
    I2C_stop();

    return data;
}


void I2C_write_with_params(uint8_t reg, uint8_t val) {
    I2C_start();  
    I2C_write(MAX30102_ADDRESS_WRITE); // Send device address + write mode

    I2C_write(reg); 

    I2C_write(val);       

    I2C_stop();
}