/* 
 * File:   I2C.h
 * Author: 
 *
 * Created on April 19, 2024, 3:27 PM
 */

#ifndef I2C_H
#define	I2C_H
#include <xc.h>
#define MAX30102_ADDRESS_WRITE 0xAE //writing
#define MAX30102_ADDRESS_READ  0xAF //reading

#ifdef	__cplusplus
extern "C" {
#endif

void delay_ms(unsigned int ms);
void init_I2C1(void);
void I2C1_start();
void I2C1_stop();
void I2C1_write(uint8_t data);

void I2C_start();
void I2C_stop();
void I2C_write(uint8_t data);
uint32_t I2C_read();
void I2C_repeated_start();
void I2C_ACK();
void I2C_NACK();
void I2C_read_multiple_bytes_params(uint8_t reg, int numSamples);
uint8_t I2C_read_with_params(uint8_t reg);
void I2C_write_with_params(uint8_t reg, uint8_t val);
#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */
