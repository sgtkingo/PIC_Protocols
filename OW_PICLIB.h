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
 * Comments: OneWire function header for PIC18LF46K22
 */

// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef OW_SINGLE_DEVICE_H
#define	OW_SINGLE_DEVICE_H

#include <xc.h> // include processor files - each processor file is guarded.

//Pin INPUT and OUTPUT for OW
#define P_W PORTAbits.RA0
#define P_R PORTAbits.RA1

// 'tick' values
static unsigned int A,B,C,D,E,F,G,H,I,J;

///Declarations of functions
void tickDelay(int tick);
void OW_Init();

void OW_SetSpeed();

int OWTouchReset(void);
void OWWriteBit(int myBit);
int OWReadBit(void);

void OWWriteByte(int data);
int OWReadByte(void);
int OWTouchByte(int data);

void OWBlock(unsigned char *data, int data_len);
//int OWOverdriveSkip(unsigned char *data, int data_len);

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
int OWTouchReset(void)
{
        int result;

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
void OWWriteBit(int myBit)
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
int OWReadBit(void)
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
void OWWriteByte(int data)
{
        int loop;

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
int OWReadByte(void)
{
        int loop, result=0;

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
int OWTouchByte(int data)
{
        int loop, result=0;

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
        int loop;

        for (loop = 0; loop < data_len; loop++)
        {
                data[loop] = OWTouchByte(data[loop]);
        }
}

//-----------------------------------------------------------------------------


#endif	/* OW_SINGLE_DEVICE_H */

