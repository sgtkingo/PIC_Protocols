
#ifndef KEYBOARD4x4_CHARMAP_H
#define	KEYBOARD4x4_CHARMAP_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define N_KEYBOARD 16

#define CODE_NOKEY 0xFF
#define CODE_OVERFLOW 0x11

char getChar(unsigned char value);
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

