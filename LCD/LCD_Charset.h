/*  
 * Author: Jiri Konecny 
 * Version: 070420
 * Comments: Charset header for LCD
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LCD_CHARSET
#define	LCD_CHARSET

#include <xc.h> // include processor files - each processor file is guarded.

/*
*********************************************************************************************************
* convertNumberAsArray(unsigned int number)
*
* Description: Convert number to string
* Arguments  : number - decimal number
* Returns    : number as string
*********************************************************************************************************
*/
char* convertNumberAsArray(unsigned int number);
/*
*********************************************************************************************************
* getNumber(char number)
*
* Description: Convert one decimal number 0-9 to character
* Arguments  : number - decimal number 0-9
* Returns    : number as character
*********************************************************************************************************
*/
char getNumber(char number);
/*
*********************************************************************************************************
* getSize(unsigned int number)
*
* Description: Get quantity of digits in number
* Arguments  : number - decimal number
* Returns    : quantity of digits
*********************************************************************************************************
*/
char getSize(unsigned int number);

const char* LINE="                ";

char NumbersMap[]={
'0',
'1',
'2',
'3',
'4',
'5',
'6',
'7',
'8',
'9',
};
char getNumber(char number){
    if(number>0 && number<10)
        return NumbersMap[number];
    else return NumbersMap[0];
}

char getSize(unsigned int number){
    char i=0;
    if(number == 0)return 1;
    
    while ( number > 0){
        number/=10;
        i++;
    }
    return i;
}

char* convertNumberAsArray(unsigned int number){
    if(number < 0)number*=-1;
    
    char size=(getSize(number)+1);
    if(size > 16)size=16;
    static char buffer[16];
    
    char i=(size-2);
    if(number == 0){
        buffer[i]=getNumber(number);
    }
    while(number > 0 && i >= 0){
        buffer[i]=getNumber(number%10);
        number/=10;
        i--;
    }
    buffer[size-1]='\0';
    return buffer;
} 
#endif	/* LCD_CHARSET */

