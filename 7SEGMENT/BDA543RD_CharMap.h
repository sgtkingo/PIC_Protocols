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
 * Author: Jiri Konecny
 * Comments: Charmaps header for BDA543RD 7 segments display
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef BDA543RD_CHARMAP
#define	BDA543RD_CHARMAP

const char numbersData[]={ 
0b11000000 /*0*/ ,
0b11111001 /*1*/, 
0b10100100  /*2*/,
0b10110000 /*3*/,
0b10011001 /*4*/,
0b10010010 /*5*/,
0b10000010 /*6*/,
0b11111000 /*7*/,
0b10000000 /*8*/,
0b10010000 /*9*/};

const char charData[]={
0b10111111 /*-*/};

const char charDataAlias[]={
'-'};

char getChar(char ch){
    char returnchar=0xFF;
    for(int i=0;i<sizeof(charDataAlias)/sizeof(char);i++){
        if(charDataAlias[i]==ch){
            returnchar=charData[i];
            break;
        }
    }
    return returnchar;
}


#endif	/* BDA543RD_CHARMAP */

