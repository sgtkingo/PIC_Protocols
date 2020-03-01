/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LCD_CHARSET
#define	LCD_CHARSET

#include <xc.h> // include processor files - each processor file is guarded.

void convertNumber(unsigned int number, char* buffer, char buffersize);
char getNumber(char number);
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
    while ( number > 0){
        number/=10;
        i++;
    }
    return i;
}

void convertNumber(unsigned int number, char* buffer, char buffersize){
    char size=getSize(number);
    if( buffersize-1 < size) 
        size=buffersize-1;
    
    char i=size-1;
    
    while(number > 0 && i > -1){
        buffer[i]=getNumber(number%10);
        number/=10;
        i--;
    }
    buffer[buffersize-1]='\0';
} 
#endif	/* LCD_CHARSET */

