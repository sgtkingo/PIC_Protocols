/*  
 * Author: Jiri Konecny 
 * Version: 060420
 * Comments: 1-Wire function header for PIC
 */

// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef PIC_OW_BUS_H
#define	PIC_OW_BUS_H

#include <xc.h> // include processor files - each processor file is guarded.

//Pin INPUT and OUTPUT for OW
#define P_W PORTAbits.RA0
#define P_R PORTAbits.RA1

// 'tick' values
static unsigned int A,B,C,D,E,F,G,H,I,J;

///Declarations of functions
/*
*********************************************************************************************************
* tickDelay(int tick)
*
* Description: Simple delay us funciton
* Arguments  : tick - amount of tick in us
* Returns    : none
*********************************************************************************************************
*/
void tickDelay(int tick);
/*
*********************************************************************************************************
* OW_Init()
*
* Description: Init 1-Wire bus
* Arguments  : none
* Returns    : none
*********************************************************************************************************
*/
void OW_Init();

/*
*********************************************************************************************************
* OW_SetSpeed()
*
* Description: Set tick values for 1-wire bus
* Arguments  : none
* Returns    : none
*********************************************************************************************************
*/
void OW_SetSpeed();

/*
*********************************************************************************************************
* OWTouchReset()
*
* Description: Reset 1-Wire devices on bus
* Arguments  : none
* Returns    : return log. value 1/0
*********************************************************************************************************
*/
char OWTouchReset(void);
/*
*********************************************************************************************************
* OWWriteBit(char myBit)
*
* Description: Send 1 bit to 1-Wire bus
* Arguments  : myBit - logic value 1/0
* Returns    : none
*********************************************************************************************************
*/
void OWWriteBit(char myBit);
/*
*********************************************************************************************************
* OWReadBit()
*
* Description: Read 1 bit on 1-Wire bus
* Arguments  : none
* Returns    : readed bit
*********************************************************************************************************
*/
char OWReadBit(void);

/*
*********************************************************************************************************
* OWWriteByte(char data)
*
* Description: Write 1 byte on 1-Wire bus
* Arguments  : data - 1 byte to write
* Returns    : none
*********************************************************************************************************
*/
void OWWriteByte(char data);
/*
*********************************************************************************************************
* OWReadByte()
*
* Description: Read 1 byte on 1-Wire bus
* Arguments  : none
* Returns    : readed byte
*********************************************************************************************************
*/
char OWReadByte(void);
/*
*********************************************************************************************************
* OWTouchByte(char data)
*
* Description: Touch 1 byte on 1-Wire bus
* Arguments  : data - byte to touch
* Returns    : ack value
*********************************************************************************************************
*/
char OWTouchByte(char data);

/*
*********************************************************************************************************
* OWBlock(unsigned char *data, int data_len)
*
* Description: send array of bytes to bus
* Arguments  : data - bytes data array, data_len - size of "data"
* Returns    : none
*********************************************************************************************************
*/
void OWBlock(unsigned char *data, int data_len);

///Definitions of functions

//Simple delay fce
void tickDelay(int tick){
    while(tick-->0);
}

//Initialize OW
void OW_Init(){
    ANSELAbits.ANSA0=0; //Set I/O OW port as Din
    ANSELAbits.ANSA1=0; //Set I/O OW port as Din
    TRISAbits.RA1=1; //OW IN
    TRISAbits.RA0=0; //OW OUT
    OW_SetSpeed();
}
// Set the 1-Wire timing to 'standard'
void OW_SetSpeed()
{
    // Standard Speed
    A = 1;
    B = 12;
    C = 12;
    D = 1;
    E = 1;
    F = 10;
    G = 0;
    H = 111;
    I = 14;
    J = 91;
}

//-----------------------------------------------------------------------------
// Generate a 1-Wire reset, return 1 if no presence detect was found,
// return 0 otherwise.
// (NOTE: Does not handle alarm presence from DS2404/DS1994)
//
char OWTouchReset(void)
{
        char result;

        tickDelay(G);
        P_W=0x00; // Drives DQ low
        tickDelay(H);
        P_W=0x01; // Releases the bus
        tickDelay(I);
        result = P_R & 0x01; // Sample for presence pulse from slave
        tickDelay(J); // Complete the reset sequence recovery
        return result; // Return sample presence pulse result
}

//-----------------------------------------------------------------------------
// Send a 1-Wire write bit. Provide 10us recovery time.
//
void OWWriteBit(char myBit)
{
        if (myBit)
        {
                // Write '1' bit
                P_W=0x00; // Drives DQ low
                tickDelay(A);
                P_W=0x01; // Releases the bus
                tickDelay(B); // Complete the time slot and 10us recovery
        }
        else
        {
                // Write '0' bit
                P_W=0x00; // Drives DQ low
                tickDelay(C);
                P_W=0x01; // Releases the bus
                tickDelay(D);
        }
}

//-----------------------------------------------------------------------------
// Read a bit from the 1-Wire bus and return it. Provide 10us recovery time.
//
char OWReadBit(void)
{
        int result;

        P_W=0x00; // Drives DQ low
        tickDelay(A);
        P_W=0x01; // Releases the bus
        tickDelay(E);
        result = P_R & 0x01; // Sample the bit value from the slave
        tickDelay(F); // Complete the time slot and 10us recovery

        return result;
}

//-----------------------------------------------------------------------------
// Write 1-Wire data byte
//
void OWWriteByte(char data)
{
        char loop;

        // Loop to write each bit in the byte, LS-bit first
        for (loop = 0; loop < 8; loop++)
        {
                OWWriteBit(data & 0x01);

                // shift the data byte for the next bit
                data >>= 1;
        }
}

//-----------------------------------------------------------------------------
// Read 1-Wire data byte and return it
//
char OWReadByte(void)
{
        char loop, result=0;

        for (loop = 0; loop < 8; loop++)
        {
                // shift the result to get it ready for the next bit
                result >>= 1;

                // if result is one, then set MS bit
                if (OWReadBit())
                        result |= 0x80;
        }
        return result;
}

//-----------------------------------------------------------------------------
// Write a 1-Wire data byte and return the sampled result.
//
char OWTouchByte(char data)
{
        char loop, result=0;

        for (loop = 0; loop < 8; loop++)
        {
                // shift the result to get it ready for the next bit
                result >>= 1;

                // If sending a '1' then read a bit else write a '0'
                if (data & 0x01)
                {
                        if (OWReadBit())
                                result |= 0x80;
                }
                else
                        OWWriteBit(0);

                // shift the data byte for the next bit
                data >>= 1;
        }
        return result;
}

//-----------------------------------------------------------------------------
// Write a block 1-Wire data bytes and return the sampled result in the same
// buffer.
//
void OWBlock(unsigned char *data, int data_len)
{
        char loop;

        for (loop = 0; loop < data_len; loop++)
        {
                data[loop] = OWTouchByte(data[loop]);
        }
}

//-----------------------------------------------------------------------------


#endif	/* PIC_OW_BUS_H */

