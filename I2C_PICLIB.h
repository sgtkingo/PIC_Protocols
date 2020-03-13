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
 * Comments: Library for PIC I2C
 * Revision history: 1.0 / 070320
 */

// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef PIC_I2CLIB
#define	PIC_I2CLIB

#include <xc.h> // include processor files - each processor file is guarded.

#define SCL PORTCbits.RC3 //SCL init as 0
#define SDA PORTCbits.RC4 //SDA init as 0

#define true 1
#define false 0

#define MASK_W &0xFE
#define MASK_R |0x01

#define RESET_TIME 1000

#define SEN SSP1CON2bits.SEN
#define PEN SSP1CON2bits.PEN
#define RW SSP1STATbits.READ_WRITE
#define BF SSP1STATbits.BF
#define COLIF SSP1CON1bits.WCOL

#define SSPIF PIR1bits.SSP1IF

#define SPEED_N_100kHz 100000
#define SPEED_HS_400kHz 400000
#define SPEED_UHS_1MHz 1000000

unsigned char BITRATE=0;
/* find bit rate to assign this value to SSPADD register, _XTAL_FREQ=FOSH */
//#define BITRATE ((_XTAL_FREQ/(4*I2C_CLOCK))-1)

bit I2CError;

void I2C_INIT();
void I2C_SPEED(unsigned long int speed);

void I2C_STR();
void I2C_STP();

void I2C_ON();
void I2C_OFF();

void I2C_COLISION();
void I2C_IDLE();
void I2C_MSSP();

bit I2C_BUS_STATUS();
void I2C_CLEAR();

void I2C_ACK();
void I2C_NACK();

void WriteI2C( unsigned char data_out );
unsigned char I2CRead(char mode);

void I2C_PAUSE(unsigned int t);

void I2C_PAUSE(unsigned int t){
    while(t--)__delay_us(1);
}

void I2C_INIT()
{
        ANSELCbits.ANSC3=0; //SCL as dig
        ANSELCbits.ANSC4=0; //SDA as dig
        TRISCbits.RC3=1; //SCL as IN
        TRISCbits.RC4=1; //SDA as IN
        SDA=SCL=0;
        BITRATE=0;

        I2C_SPEED(SPEED_N_100kHz);
        SSP1CON1 = 0x08;     /* not enable SSP port for I2C Master mode,
                                         * clock = FOSC / (4 * (SSPADD+1)) */
        SSP1CON2 = 0x00;

        INTCONbits.GIE=1;
        INTCONbits.PEIE=1;
        PIE1bits.SSPIE = 1;             /* enable SSPIF interrupt */
        I2C_CLEAR();

        I2C_ON();

        I2C_PAUSE(1000);
}

void I2C_SPEED(unsigned long int speed){
    //BITRATE ((_XTAL_FREQ/(4*I2C_CLOCK))-1)
    I2C_OFF();

    BITRATE=((_XTAL_FREQ/(4*speed))-1);
    SSP1ADD  =(char)BITRATE;

    if(speed == SPEED_N_100kHz){
        SSP1STAT = 0x80;      /* slew rate disabled, other bits are cleared*/
    }
    else {
        SSP1STAT = 0x00;      /* slew rate enable, other bits are cleared*/
    }

    I2C_ON();
}

void I2C_ON(){
    SSP1CON1bits.SSPEN = 1;   // Enable I2C operation
    while(!SSP1CON1bits.SSPEN)NOP();
}

void I2C_OFF(){
    SSP1CON1bits.SSPEN = 0;   // Enable I2C operation
    while(SSP1CON1bits.SSPEN)NOP();
}

void I2C_COLISION(){
    I2C_IDLE();
    while(COLIF || BF || RW);
    SSPIF=0;
}

void I2C_IDLE(){
    while (SEN | PEN | RW); //W8 for S or P or RW
}
void I2C_MSSP(){
    I2CError=false;
    char t=RESET_TIME;

    while(!SSPIF && t)t--;
    SSPIF=0;

    if(t == 0 )I2CError=true;
}
bit I2C_BUS_STATUS(){
    if ((SCL ^ SDA)){
       I2CError=true;
    }
    I2CError=false;

    return I2CError;
}

void I2C_CLEAR(){
    SSP1BUF = 0x00;
    SSPIF=0;
    I2CError=0;
}

void I2C_STR(){
    I2C_COLISION();
    SEN=1;

    I2C_IDLE();
    I2C_BUS_STATUS();
}

void I2C_STP(){
    I2C_COLISION();
    PEN=1;

    I2C_IDLE();
    I2C_BUS_STATUS();
}

void WriteI2C( unsigned char data_out ){
    I2C_COLISION();

    SSP1BUF = data_out; // write single byte to SSPBUF
    I2C_MSSP();

    I2CError=SSP1CON2bits.ACKSTAT;
}

unsigned char I2CRead(char mode){
    I2C_COLISION();

    char data=0;
    SSP1CON2bits.RCEN=1; //recieve
    NOP();

    I2C_MSSP();
    data=SSP1BUF; //read data

    (mode & 0x01) ? I2C_ACK() : I2C_NACK();

    return data;        // Data from eeprom is now in the SSPBUF so return that value
}

void I2C_ACK(){
    SSP1CON2bits.ACKDT=0; //MODE ACK
    SSP1CON2bits.ACKEN=1;//ack sequence
    while(SSP1CON2bits.ACKEN); //Wait to finish
}
void I2C_NACK(){
    SSP1CON2bits.ACKDT=1; //MODE NACK
    SSP1CON2bits.ACKEN=1;//ack sequence
    while(SSP1CON2bits.ACKEN); //Wait to finish
}
#endif	/* PIC_I2CLIB */

