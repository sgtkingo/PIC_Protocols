/*  
 * Author: Jiri Konecny 
 * Version: 060420
 * Comments: SPI bus function header for PIC
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PIC_SPI_BUS_H
#define	PIC_SPI_BUS_H

#include <xc.h> // include processor files - each processor file is guarded. 

#define SCK PORTCbits.RC3;
#define SDI PORTCbits.RC4;
#define SDO PORTCbits.RC5;

#define SSPIF PIR1bits.SSP1IF

#define RESET_TIME 1000

#define BF SSP1STATbits.BF
#define COLIF SSP1CON1bits.WCOL

bit SPI_Error=false;

/*
*********************************************************************************************************
* SPI_INIT()
*
* Description: Init SPI bus
* Arguments  : none
* Returns    : none
*********************************************************************************************************
*/
void SPI_INIT();
/*
*********************************************************************************************************
* SPI_CONFIG()
*
* Description: Config SPI bus
* Arguments  : none
* Returns    : none
*********************************************************************************************************
*/
void SPI_CONFIG(); 
/*
*********************************************************************************************************
* SPI_SET_CS(unsigned char *port, unsigned char pin, unsigned char cs)
*
* Description: Set CS signal
* Arguments  : port - port for CS, pin - pin on port, cs - log. value 1/0
* Returns    : none
*********************************************************************************************************
*/
void SPI_SET_CS(unsigned char *port, unsigned char pin, unsigned char cs);
/*
*********************************************************************************************************
* SPI_WRITE(unsigned char data)
*
* Description: Write byte to SPI bus
* Arguments  : data - byte to write
* Returns    : none
*********************************************************************************************************
*/
void SPI_WRITE(unsigned char data);
/*
*********************************************************************************************************
* SPI_MSSP()
*
* Description: Wait util MSSP buffer flush
* Arguments  : none
* Returns    : none
*********************************************************************************************************
*/
void SPI_MSSP();
/*
*********************************************************************************************************
* SPI_COLISION()
*
* Description: Chceck for SPI bus collision
* Arguments  : none
* Returns    : none
*********************************************************************************************************
*/
void SPI_COLISION();

/*
*********************************************************************************************************
* SPI_PAUSE(unsigned int t))
*
* Description: Tick pause in us
* Arguments  : t - number of tick in us
* Returns    : none
*********************************************************************************************************
*/
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

#endif	/* PIC_SPI_BUS_H */

