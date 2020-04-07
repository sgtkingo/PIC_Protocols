/*  
 * Author: Jiri Konecny 
 * Version: 060420
 * Comments: PIC16F18323 us as 2to4 demux function header for PIC
 */

// This is a guard condition so that contents of this file are not included
// more than once.  

#ifndef PIC_PIC16F18323_FUNCTIONLIB_H
#define	PIC_PIC16F18323_FUNCTIONLIB_H

#include <xc.h>
#define _XTAL_FREQ 16000000

#define DMX_A LATEbits.RE0
#define DMX_B LATEbits.RE1

#define DMX_OV LATAbits.LA0

#define DMX_OVERDRIVE 0xFF

#define W8_TO_EXECUTE __delay_us(50)

/*
*********************************************************************************************************
* Init_DEMUXER()
*
* Description: Init and config demuxer control pins
* Arguments  : none
* Returns    : none
*********************************************************************************************************
*/
void Init_DEMUXER();
/*
*********************************************************************************************************
* config_DEMUXER()
*
* Description: Config demuxer control pins
* Arguments  : none
* Returns    : none
*********************************************************************************************************
*/
void config_DEMUXER();

/*
*********************************************************************************************************
* Overdrive_DEMUX(unsigned char mode)
*
* Description: Set ovedride signal for demuxer
* Arguments  : mode - log. value 1/0
* Returns    : none
*********************************************************************************************************
*/
void Overdrive_DEMUX(unsigned char mode);
/*
*********************************************************************************************************
* Set_DEMUX(unsigned char device)
*
* Description: Select active device (demux control output)
* Arguments  : device - value 0x0-0x03
* Returns    : none
*********************************************************************************************************
*/
void Set_DEMUX(unsigned char device);

void Init_DEMUXER(){
    config_DEMUXER();
    Overdrive_DEMUX(0);
}

void config_DEMUXER(){
    ANSELAbits.ANSA0=0;
    ANSELEbits.ANSE0=0;
    ANSELEbits.ANSE1=0;
    
    TRISAbits.RA0=0;
    TRISEbits.RE0=0;
    TRISEbits.RE1=0;
    
    //Default UNBLOCK DEMUX
    DMX_OV=0;
}

void Overdrive_DEMUX(unsigned char mode){
    DMX_OV=mode;
}

void Set_DEMUX(unsigned char device){
    if(device == DMX_OVERDRIVE){
        Overdrive_DEMUX(1);
    }else Overdrive_DEMUX(0);
    
    if(device > 3 || device < 0)device=0;
    LATE&=0xFC; //Clear demux
    LATE|=device; //Set demux
    W8_TO_EXECUTE;
}

#endif	/* PIC_PIC16F18323_FUNCTIONLIB_H */

