/*  
 * Author: Jiri Konecny 
 * Version: 070420
 * Comments: 4x4 keyboard via MCP26017 function header for PIC
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PIC_KEYABOARD4x4_VIA_MCP23017_H
#define	PIC_KEYABOARD4x4_VIA_MCP23017_H
#include "MCP23017_FunctionLib.h"
#include "KEYBOARD4x4_Charmap.h"

#define INTB PORTBbits.RB4 
#define INTA PORTBbits.RB5

#define IN_MASK 0xF0
#define OUT_MASK 0x0F

#define ALL_OFF 0x0F

#define COL_A 0x01
#define COL_B 0x02
#define COL_C 0x04
#define COL_D 0x08

#define ROW_A 0x01
#define ROW_B 0x02
#define ROW_C 0x04
#define ROW_D 0x08

unsigned char IN_DATA,OUT_DATA,RESULT;

/*
*********************************************************************************************************
* Init_Keyboard()
*
* Description: Init keyboard and MCP23017
* Arguments  : none
* Returns    : none
*********************************************************************************************************
*/
void Init_Keyboard();
/*
*********************************************************************************************************
* Read_Keyboard()
*
* Description: Read keyboard
* Arguments  : none
* Returns    : readed value
*********************************************************************************************************
*/
unsigned char Read_Keyboard();
/*
*********************************************************************************************************
* Resolve_Keyboard(unsigned char in, unsigned char out)
*
* Description: Resolve keyboard input
* Arguments  : in - keyboard get value, out - keyboard test value
* Returns    : keyboard code key
*********************************************************************************************************
*/
unsigned char Resolve_Keyboard(unsigned char in, unsigned char out);
/*
*********************************************************************************************************
* Test_Keyboard(unsigned char out)
*
* Description: Test keyboard by parametr
* Arguments  : out - keyboard test value
* Returns    : none
*********************************************************************************************************
*/
void Test_Keyboard(unsigned char out);

void Init_Keyboard(){
    //Input for INTA and INTB
    ANSELBbits.ANSB4=0;
    ANSELBbits.ANSB5=0;
    TRISBbits.RB4=1;
    TRISBbits.RB5=1;
    
    RESULT=0;
    IN_DATA=0;
    OUT_DATA=COL_A;
    
    Init_MCP23017();
    Set_DirectionOfPort(IODIRA, 0xF0); //1-4 ROWN (IN), 5-8 COLS (OUT)
    Set_PullUp(PORT_A_MASK, 0xF0); //pull-up port A
    Set_PortPol(PORT_A_MASK, 0xF0); //Swap polarization of port A (0 -> 1)
    
    Set_IOC(PORT_A_MASK,0xF0,true,0x0F); //pin 0-3 are IOC, compare to 0 (0 trigger int)
    Get_IOC_Cap(PORT_A_MASK); //clear interrupt
    
    Send_DataToPort(GPIOA, ALL_OFF); 
}

unsigned char Read_Keyboard(){
    RESULT=0;
    OUT_DATA=COL_A;
    
    while( OUT_DATA <= COL_D){
        Test_Keyboard(OUT_DATA);
        delay_ms(10);
        while( INTA ){
            IN_DATA=(Read_DataFromPort(GPIOA) & IN_MASK);
            IN_DATA >>=4;
            if(IN_DATA == 0)break;

            RESULT=Resolve_Keyboard(IN_DATA,OUT_DATA);
            return RESULT;
        }
        OUT_DATA*=2;
        IN_DATA=0x00;   
    }
    return CODE_NOKEY;
}

void Test_Keyboard(unsigned char out){
    out =  ~out; 
    NOP();
    Send_DataToPort(GPIOA, out & OUT_MASK); 
}

unsigned char Resolve_Keyboard(unsigned char in, unsigned char out){
   if( in == 0 || out == 0) return CODE_NOKEY;
   unsigned char result=0;
   if( out == COL_A){
       if( in == ROW_A){
           result=0xD;
           //16
       }
       if( in == ROW_B){
           result=0xC;
           //15
       }
       if( in == ROW_C){
           result=0xB;
           //14
       }
       if( in == ROW_D){
           result=0xA;
           //13
       }       
   }
   if( out == COL_B){
       if( in == ROW_A){
           result=0xE;
           //12
       }
       if( in == ROW_B){
           result=0x9;
           //11
       }
       if( in == ROW_C){
           result=0x6;
           //10
       }
       if( in == ROW_D){
           result=0x3;
           //9
       }       
   }
   if( out == COL_C){
       if( in == ROW_A){
           result=0x0;
           //8
       }
       if( in == ROW_B){
           result=0x8;
           //7
       }
       if( in == ROW_C){
           result=0x5;
           //6
       }
       if( in == ROW_D){
           result=0x2;
           //5
       }       
   }
   if( out == COL_D){
       if( in == ROW_A){
           result=0xF;
           //4
       }
       if( in == ROW_B){
           result=0x7;
           //3
       }
       if( in == ROW_C){
           result=0x4;
           //2
       }
       if( in == ROW_D){
           result=0x1;
           //1
       }       
   }   
   return result;
}

#endif	/* PIC_KEYABOARD4x4_VIA_MCP23017_H */