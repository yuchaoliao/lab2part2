//yuchao liao


#include <xc.h>
#include "keypad.h"
#include "timer.h"

/* Initialize the rows as ODC outputs and the columns as inputs with pull-up
 * resistors. Don't forget about other considerations...
 */

#define TRIS_ROW1  TRISEbits.TRISE3    
#define TRIS_ROW2  TRISEbits.TRISE0 
#define TRIS_ROW3  TRISEbits.TRISE2    
#define TRIS_ROW4  TRISEbits.TRISE4    

#define TRIS_COL1 TRISEbits.TRISE5
#define TRIS_COL2 TRISEbits.TRISE7
#define TRIS_COL3 TRISEbits.TRISE6


#define INPUT 1
#define OUTPUT 0


#define ROW1 LATEbits.LATE3
#define ROW2 LATEbits.LATE0
#define ROW3 LATEbits.LATE2
#define ROW4 LATEbits.LATE4

#define COL1 PORTEbits.RE5 
#define COL2 PORTEbits.RE7
#define COL3 PORTEbits.RE6

//find ANSEL registers, declare them as digital inputs
//set interrupts
//set input pins as digital
//set flags for columns

void initKeypad(void)
{
    TRIS_ROW1 = OUTPUT; //0
    TRIS_ROW2 = OUTPUT;
    TRIS_ROW3 = OUTPUT;
    TRIS_ROW4 = OUTPUT;
    
    ODCEbits.ODCE3 = 1;
    ODCEbits.ODCE4 = 1;
    ODCEbits.ODCE2 = 1;
    ODCEbits.ODCE0 = 1;
    
  
    

   
    
    TRIS_COL1 = INPUT; //1
    TRIS_COL2 = INPUT;
    TRIS_COL3 = INPUT;

  //enables pull ups
    CNPUEbits.CNPUE5 = 1;
    CNPUEbits.CNPUE6 = 1;
    CNPUEbits.CNPUE7 = 1;
   

    
    CNCONEbits.ON = 1;
    
    CNENEbits.CNIEE5 = 1;
    CNENEbits.CNIEE6 = 1;
    CNENEbits.CNIEE7 = 1;
    
     // put the flag down
    IFS1bits.CNEIF = 0;
    IEC1bits.CNEIE = 0;
    IPC8bits.CNIP = 7;
    
    

    ANSELEbits.ANSE7 = 0;
    ANSELEbits.ANSE5 = 0;
    ANSELEbits.ANSE6 = 0;
    
 
    ROW1 = 1;
    ROW2 = 1;
    ROW3 = 1;
    ROW4 = 1;
    
    COL1 = 1;
    COL2 = 1;
    COL3 = 1;
    
    
   
    
}

/* This function will be called AFTER you have determined that someone pressed
 * SOME key. This function is to figure out WHICH key has been pressed.
 * This function should return -1 if more than one key is pressed or if
 * no key is pressed at all. Otherwise, it should return the ASCII character of
 * the key that is pressed. The ascii character c programmatically is just 'c'
 */
char scanKeypad(void)
{
    char key = -1;
    
    

    disableKeyboardInterrupt();
    

    
    
    //row1
    ROW1 =0;
    ROW2 =1;
    ROW3 =1;
    ROW4 =1;
    
    
    delayUs(1000);
    
    
    //check col    
        if (COL1 == 0 ) key = '1';
         if (COL2 == 0 ) key = '2';
        if (COL3 == 0) key = '3';
  
    
    
        //row 2 
    ROW1 =1;
    ROW2 =0;
    ROW3 =1;
    ROW4 =1;
    
    delayUs(1000);
    
    //check col
  
      if (COL1 == 0 ) key = '4';
       if (COL2 == 0) key = '5';
      if (COL3 == 0 ) key = '6';
    
        //row 3 
    ROW1 =1;
    ROW2 =1;
    ROW3 =0;
    ROW4 =1;
    
    delayUs(1000);
    
    //check col 
  
    
       if (COL1 == 0 ) key = '7';
        if (COL2 == 0) key = '8';
        if (COL3 == 0) key = '9';

 // row 4 
    ROW1 =1;
    ROW2 =1;
    ROW3 =1;
    ROW4 =0;
    
        delayUs(1000);
    
    //check col 
  
    
      
       if (COL1 == 0) key = '*';//star
        if (COL2 == 0 ) key = '0';
        if (COL3 == 0 ) key = '#'; //#
    

        ROW1=0;
        ROW2=0;
        ROW3=0;
        ROW4=0;
        
    
  enableKeyboardInterrupt(); 

    
    return key;
}


void    enableKeyboardInterrupt()
{
    CNCONDbits.ON = 1; //ENABLE INTERRUPT
    CNENEbits.CNIEE5 = 1; //col 1
    CNENEbits.CNIEE7 = 1; //col 2
    CNENEbits.CNIEE6 = 1; //col 3 
    IEC1bits.CNEIE = 1; //enable allover interrupt    
    IFS1bits.CNEIF = 0; //put flag down
    
    
    IPC8bits.CNIP = 7;
}

void disableKeyboardInterrupt()
{
    CNCONDbits.ON = 0; //ENABLE INTERRUPT
    CNENEbits.CNIEE5 = 1; //col 1
    CNENEbits.CNIEE7 = 1; //col 2
    CNENEbits.CNIEE6 = 1; //col 3 
    IEC1bits.CNEIE = 0; //enable allover interrupt    
    IFS1bits.CNEIF = 0; //put flag down
   
    
    IPC8bits.CNIP = 7;

}