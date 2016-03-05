// ******************************************************************************************* //
//
// File:        lab2part2   yuchaoliao
// Date:         
// Authors:      
//
// Description: Part2 for lab 2
// ******************************************************************************************* //

#include <xc.h>
#include <sys/attribs.h>
#include "interrupt.h"
#include "timer.h"
#include "config.h"
#include "lcd.h"
#include "keypad.h"



#define press 0
#define release 1

#define COL1 PORTEbits.RE5
#define COL2 PORTEbits.RE7
#define COL3 PORTEbits.RE6



typedef enum stateTypeEnum {
    waitpress,justwait, Scanandhold,waitrelease, check,setmode,savepassword,passwordInvalid, checkpassword,passwordgood,passwordbad
} stateType;

volatile stateType state =waitpress;
/* Please note that the configuration file has changed from lab 0.
 * the oscillator is now of a different frequency.
 */


int main(void)
{
    SYSTEMConfigPerformance(10000000); //
    
    char password[]={'p','p','p','p'};
    char database[][4] = { {'1','1','1','1'}, {'p','p','p','p'}, {'p','p','p','p'}, {'p','p','p','p'} };
    int position=0;     // set database position
    int passcount = 0;  // count password long
    int savemode=0;     // check whether in set mode 
    int i=0,k=0;       // i, k for loop
    char key= 'p';     // use for scan keypad
    int countright =0; // check for password valid for local 
    int bo=0;          //check for password valid for overall
    
	
  
    initTimer2();
    initLCD();
    initKeypad();

    enableInterrupts();
    enableKeyboardInterrupt();

             
     while (1) 
    {
       
        switch (state) {
            case waitpress:                
                clearLCD();
                moveCursorLCD(0,0);
                printStringLCD("Enter");
                moveCursorLCD(1,0);
                state = justwait;
                break;
                
            case justwait:
            break;
                
            case Scanandhold:
                key = scanKeypad();
                password [passcount] = key;
               
                if (savemode ==1 && (key== '#' ||key =='*')){    // check if set mode and first is * or #, password invalid
                    state = passwordInvalid;
                    savemode=0;
                    break;
                }
                moveCursorLCD(1,passcount);
                printCharLCD(password[passcount]);
                passcount++;
                 
                if (passcount ==4 ){               //when password full
                    passcount =0;
                   
                    if (savemode ==1 ){            // if in set mode 
                        state = savepassword;
                        break;
                    }
                    else if(password[3]=='#'||password[3]=='*'){    // if last word invalid 
                        state = passwordInvalid;
                        break;
                    }
                    else{
                        state = checkpassword;              // otherwise go to check password 
                        break;
                    }
                }
                     
                    if (position ==4){             //if database  full clear 
                        position =0;
                    }
                   
                    state = waitrelease;
            break;
                
            case waitrelease :
            break;
             
            case check:
                if (password [0]== '*'){
                    if ( password [1] == '*') {  // first * second * set mode 
                        state = setmode; 
                        break;
                    }
                    else if (password[1]!='p' && password [1]!= '*'){  //first * second not * invalid password 
                        state = passwordInvalid;
                        break;
                    }
                }
                if(savemode == 0 && password[0]!='*'){         //check for all invalid 
                    if(password[2] == '*' || password[3] == '*' || ((password[0] != '*') && (password[1] == '*'||password[0] == '#' || password[1] == '#' || password[2] == '#' || password[3] == '#' ))){
                        state = passwordInvalid;
                        break;
                    }
                }
                state =justwait;
                
            break ;
                
            case setmode:
                clearLCD();
                moveCursorLCD(0,0);
                printStringLCD("Set Mode");         
                passcount=0;
                password[0]='p';
                password[1]='p';
                password[2]='p';
                password[3]='p';
                moveCursorLCD(1,0);
                savemode=1;          // label in set mode 
                state = justwait;    // go back to scan
            break;
             
            case passwordInvalid:
                
                clearLCD();
                moveCursorLCD(0,0);
                printStringLCD("Invalid");    
                passcount = 0;
                password[0]='p';
                password[1]='p';
                password[2]='p';
                password[3]='p';
                for(i = 0; i < 250; i++) delayUs(1000);   //delay
                state = waitpress ; 
            break;
            
            case checkpassword:
                for (i=0; i<4 ; i++){
                    for (k=0; k<4 ;k++){
                        if (password[k]== database[i][k]){      //check password one char by one char start from first position
                            countright++;
                        }
                        if (countright ==4){                //if all four word correct to go good 
                            state = passwordgood;
                            bo=1;
                            countright=0;
                            break;
                        }
                        if ( countright !=4 && k==3 && i==3  ){ // otherwise go to bad 
                            state = passwordbad;
                            countright=0;
                            break;
                        }
                    }
                    if (bo==1){       //for double break 
                        bo=0;
                        break;
                    }
                      
                }
            break ; 
               
            case passwordgood:
                clearLCD();
                moveCursorLCD(0,0);
                printStringLCD("GOOD"); 
                password[0]='p';
                password[1]='p';
                password[2]='p';
                password[3]='p';
                passcount=0;
                for(i = 0; i < 250; i++) delayUs(1000);
                state = waitpress;
            break;
            
            case passwordbad:
                clearLCD ();
                moveCursorLCD(0,0);
                printStringLCD("BAD");
                password[0]='p';
                password[1]='p';
                password[2]='p';
                password[3]='p';
                passcount=0;
                for(i = 0; i < 250; i++) delayUs(1000);
                state = waitpress;
            break;
                
            case savepassword:
                for (i=0; i<4 ; i++){
                    database[position][i]= password[i];      //save to database 
                }
                position++;   //update position 
                clearLCD();
                moveCursorLCD(0, 0);
                printStringLCD("Valid");
                password[0]='p';
                password[1]='p';
                password[2]='p';
                password[3]='p';
                passcount=0;
                savemode=0;
                for(i = 0; i < 250; i++) delayUs(1000);
                state= waitpress;       
            break;
        }
    }
    
     return 0;
}
 



void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt(void) {
   int dummy1;
   int dummy2;
   int dummy3;
    
   IFS1bits.CNEIF = 0;
   dummy1 = COL1;
   dummy2 = COL2;
   dummy3 = COL3;
    
    
    
    if(state == justwait && COL1 == 0 || COL2 == 0|| COL3 == 0){
         state = Scanandhold;
    }
   
    else if (state == waitrelease) {
        state = check;
    }
}
