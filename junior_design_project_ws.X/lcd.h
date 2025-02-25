/**************************************************************
 * Author: Dhayalan Balasubramanian
 * Date: March 17, 2024
 * Student ID number: 4754648
 * Course number: EE2361
 * Term: Spring
 * Lab/assignment number: 5
 **************************************************************/

#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif

void lcd_cmd(char Package);
void lcd_init();
void lcd_setCursor(char x, char y);
void lcd_printChar(char myChar);
void delay_ms(unsigned int ms);
void lcd_printStr(const char s[]);
void lcd_clr();
void lcd_animate();

#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */