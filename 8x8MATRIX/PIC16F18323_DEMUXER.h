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
 * Comments: DEMUX driver 
 * Revision history: 1.0/220320
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PIC16F18323_DEMUXER_H
#define	PIC16F18323_DEMUXER_H

#include <xc.h>

#define DMX_A LATEbits.RE0
#define DMX_B LATEbits.RE1

#define DMX_OV LATAbits.LA0

#define DMX_OVERDRIVE 0xFF

void Init_DEMUXER();
void config_DEMUXER();

void Overdrive_DEMUX(unsigned char mode);

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
    __delay_us(25);
}

#endif	/* PIC16F18323_DEMUXER_H */

