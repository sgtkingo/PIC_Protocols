/*  
 * Author: Jiri Konecny 
 * Version: 070420
 * Comments:Keyboard charmap header
 */

#ifndef KEYBOARD4x4_CHARMAP_H
#define	KEYBOARD4x4_CHARMAP_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define N_KEYBOARD 16

#define CODE_NOKEY 0xFF
#define CODE_OVERFLOW 0x11

/*
*********************************************************************************************************
* getChar(unsigned char value)
*
* Description: Get character by decimal code from charSet map
* Arguments  : value - decimal code 
* Returns    : character
*********************************************************************************************************
*/
char getChar(unsigned char value);
/*
*********************************************************************************************************
* getValue(unsigned char key)
*
* Description: Get character decimal code
* Arguments  : key - decimal code of character
* Returns    : decimal code 
*********************************************************************************************************
*/
unsigned char getValue(unsigned char key);

char charSet[N_KEYBOARD]={
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
    'A',
    'B',
    'C',
    'D',
    '#',
    '*',
};

char getChar(unsigned char value){
    if(value < 0 || value > N_KEYBOARD){
        return 0;
    }
    else return charSet[value];
}

unsigned char getValue(unsigned char key){
    if(key < 0 || key > 9){
        return CODE_OVERFLOW;
    }
    else return key;
}

#endif	/* KEYBOARD4x4_CHARMAP_H */

