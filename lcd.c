/*
 * File:   lcd.c
 * Authors:
 *
 * Created on December 31, 2014, 1:39 PM
 */

/* TODO: Make define statements for each pin used in the LCD
 */
#include <xc.h>
#include "lcd.h"
#include "timer.h"

/* The pins that you will use for the lcd control will be
 * Use these so that we will be able to test your code for grading and to
 * help you debug your implementation!
 */

#define TRIS_D7  TRISAbits.TRISA7
#define TRIS_D6  TRISGbits.TRISG14 		
#define TRIS_D5  TRISGbits.TRISG12 		
#define TRIS_D4  TRISGbits.TRISG13 

#define TRIS_RS  TRISCbits.TRISC4		
#define TRIS_E   TRISCbits.TRISC2


#define OUTPUT 0
#define INPUT 1
#define ENABLED 1
#define DISABLED 0


#define LCD_D7 LATGbits.LATG13
#define LCD_D6 LATGbits.LATG12
#define LCD_D5 LATGbits.LATG14
#define LCD_D4 LATAbits.LATA7

#define LCD_RS  LATCbits.LATC4
#define LCD_E   LATCbits.LATC2




/* This function should take in a two-byte word and writes either the lower or upper
 * byte to the last four bits of LATE. Additionally, according to the LCD data sheet
 * It should set LCD_RS and LCD_E with the appropriate values and delays.
 * After these commands are issued, there should be a delay.
 * The command type is a simplification. From the data sheet, the RS is '1'
 * when you are simply writing a character. Otherwise, RS is '0'.
 */
void writeFourBits(unsigned char word, unsigned int commandType, unsigned int delayAfter, unsigned int lower)
{
    if (lower == 1) //lower
    {
        LCD_D7 = word & 0x01;
        LCD_D6 = (word & 0x02) >> 1;
        LCD_D5 = (word & 0x04) >> 2;
        LCD_D4 = (word & 0x08) >> 3;
     
    }
    
    else if (lower == 0) //upper
    {
        LCD_D7 = (word & 0x10) >> 4;
        LCD_D6 = (word & 0x20) >> 5;
        LCD_D5 = (word & 0x40) >> 6;
        LCD_D4 = (word & 0x80) >> 7;
       
    }
 
    
    LCD_RS = commandType;
    
    //enable
    LCD_E = ENABLED; //Enable write
    
    //delay
    delayUs(100); // 
    
    //disable
    LCD_E = DISABLED;
    delayUs(delayAfter); //Execution Time delay
}

/* Using writeFourBits, this function should write the two bytes of a character
 * to the LCD.
 */
void writeLCD(unsigned char word, unsigned int commandType, unsigned int delayAfter)
{
     //Writes upper
    writeFourBits(word , commandType, delayAfter,0);
    
    //Writes lower
    writeFourBits(word, commandType, delayAfter,1);
}
    

/* Given a character, write it to the LCD. RS should be set to the appropriate value.
 */
void printCharLCD(char c) 
{
    writeLCD(c, ENABLED, 51);
}
/*Initialize the LCD
 */
void initLCD(void) 
{
    // Setup D, RS, and E to be outputs (0).
    TRIS_D7 = OUTPUT;	// D7		
	TRIS_D6 = OUTPUT;	// D6		
	TRIS_D5 = OUTPUT;	// D5		
	TRIS_D4 = OUTPUT;	// D4		
    
	TRIS_RS = OUTPUT;	// RS		
	TRIS_E  = OUTPUT;	// E

    // Initilization sequence utilizes specific LCD commands before the general configuration
    // commands can be utilized. The first few initilition commands cannot be done using the
    // WriteLCD function. Additionally, the specific sequence and timing is very important.
    
	LCD_RS = OUTPUT;
	LCD_E = OUTPUT;
    
    delayUs(1600);

    // Enable 4-bit interface
    writeFourBits(0x03, 0, 4500,1);
    writeFourBits(0x03, 0, 110,1);
    writeFourBits(0x03, 0, 50,1);
    writeFourBits(0x02, 0, 50,1);

    // Function Set (specifies data width, lines, and font).
    writeLCD(0x28, 0, 50);

    // 4-bit mode initialization is complete. We can now configure the various LCD
    // options to control how the LCD will function.

    // TODO: Display On/Off Control
        // Turn Display (D) Off
    writeLCD(0x08, 0, 44);
    
    
    // TODO: Clear Display (The delay is not specified in the data sheet at this point. You really need to have the clear display delay here.
    clearLCD();

    // TODO: Entry Mode Set
        // Set Increment Display, No Shift (i.e. cursor move)
    writeLCD(0x06, 0, 44);

    
    // TODO: Display On/Off Control
        // Turn Display (D) On, Cursor (C) Off, and Blink(B) Off
    writeLCD(0x0C, 0, 44);
}

/*
 * You can use printCharLCD here. Note that every time you write a character
 * the cursor increments its position automatically.
 * Since a string is just a character array, try to be clever with your use of pointers.
 */
void printStringLCD(const char* s) 
{
    int i;
    for(i = 0; i < strlen(s); i++) 
    {
        printCharLCD(s[i]);
    }
}

/*
 * Clear the display.
 */
void clearLCD()
{
    writeLCD(0x01, 0, 1800); 
}

/*
 Use the command for changing the DD RAM address to put the cursor somewhere.
 */
void moveCursorLCD(unsigned char x, unsigned char y)
{
//    unsigned char address;
//    address = (y-1)*0x40+(x-1);
//    address = address + 0x80;
//    writeLCD(address, 0, 44);
    
     unsigned char tempAddress = 0;
    if (x>0) 
    {
		tempAddress = 0x40 + y;
    }
     
	else 
    {
            tempAddress = y;
    }
     
    tempAddress|=0x80; //add 1 at 7th bit
        
	writeLCD(tempAddress, 0, 40);
    
}

/*
 * This function is called in lab1p2.c for testing purposes.
 * If everything is working properly, you should get this to look like the video on D2L
 * However, it is suggested that you test more than just this one function.
 */
void testLCD()
{
    initLCD();
    int i = 0;
    printCharLCD('c');
    for(i = 0; i < 1000; i++) delayUs(1000);
    clearLCD();
    printStringLCD("Hello!");
    moveCursorLCD(1, 2);
    for(i = 0; i < 1000; i++) delayUs(1000);
    printStringLCD("Hello!");
    for(i = 0; i < 1000; i++) delayUs(1000);
}