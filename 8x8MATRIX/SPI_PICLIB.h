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
 * File:   SPI_PICLIB.h
 * Author: Jiri Konecny
 * Comments: SPI lib for PIC MIC
 * Revision history: 1.0
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SPI_PICLIB_H
#define	SPI_PICLIB_H

#include <xc.h> // include processor files - each processor file is guarded. 

#define SCK PORTCbits.RC3;
#define SDI PORTCbits.RC4;
#define SDO PORTCbits.RC5;

#define SSPIF PIR1bits.SSP1IF

#define RESET_TIME 1000

#define BF SSP1STATbits.BF
#define COLIF SSP1CON1bits.WCOL

bit SPI_Error=false;

void SPI_INIT();
void SPI_CONFIG(); 
void SPI_SET_CS(unsigned char *port, unsigned char pin, unsigned char cs);
void SPI_WRITE(unsigned char data);
void SPI_MSSP();
void SPI_COLISION();

void SPI_PAUSE(unsigned int t);

void SPI_PAUSE(unsigned int t){
    while(t--)__delay_us(1);
}

void SPI_INIT(){
    SPI_CONFIG();
    
    SSP1CON1 = 0b00100010; //SPI SCK Fost/64, CKP=0
    SSP1STAT = 0x40; //CKE=1
    SSP1BUF = 0x00;
    BF=COLIF=0;
    
    SPI_Error=false;
    SPI_PAUSE(1000);
}

void SPI_CONFIG(){
    //Setting pins as digital
    ANSELCbits.ANSC3=0;
    ANSELCbits.ANSC4=0;
    ANSELCbits.ANSC5=0;
    //Setting pins I/O
    TRISCbits.RC3=0; //SCK as OUT
    TRISCbits.RC4=1; //SDI as IN
    TRISCbits.RC5=0; //SDO as OUT
}

//Pointer to PORT with CS, spec the pin and value
void SPI_SET_CS(unsigned char *port, unsigned char pin, unsigned char cs){
    if(cs==0)*port&=(~pin);
    else *port|=(pin);
    SPI_PAUSE(1);
}

void SPI_WRITE(unsigned char data){
    SPI_COLISION();
    
    SSP1BUF=data;
    SPI_MSSP();
}

void SPI_MSSP(){
    SPI_Error=false;
    int t=RESET_TIME;
    
    while(!SSPIF && t)t--;
    SSPIF=0;
    
    if(t == 0 )SPI_Error=true;
}

void SPI_COLISION(){
    while(COLIF);
    SSPIF=0;
}

#endif	/* SPI_PICLIB_H */

