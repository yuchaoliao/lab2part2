/* 
 * File:   newfile.h
 * Author: 
 *
 * Created on February 18, 2016, 4:06 PM
 */

#ifndef LCD_H
#define	LCD_H

void writeLCD(unsigned char word, unsigned int commandType, unsigned int delay);
void writeFourBits(unsigned char word, unsigned int commandType, unsigned int delayAfter, unsigned int lower);
void initLCD(void);
void clearLCD(void);
void moveCursorLCD(unsigned char x, unsigned char y);
void printCharLCD(char c);
void printStringLCD(const char* s);
void testLCD();



#endif

