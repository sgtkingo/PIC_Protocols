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
 * Comments: EEPROM 22LC64 function header for PIC
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PIC_24LC64
#define PIC_24LC64
#include "I2C_PICLIB.h"

#define TIME_WRITE 5500
#define TIME_READ  1000

#define PAGE_SIZE 32

#define H_MAX 0x1F
#define L_MAX 0xFF

#define WP LATCbits.LC6

#define ADR_24LC64 0b10100000
// address_of_24LC64_in the BUS I2C, W	
#define	ADR_24LC64_W (ADR_24LC64 MASK_W)	
// address_of_24LC64_in the BUS I2C, R	
#define	ADR_24LC64_R (ADR_24LC64 MASK_R)


void Init_24LC64();
void WP_Mode(unsigned char mode);
void WriteTo_24LC64(char mem_H, char mem_L, char data);
int ReadFrom_24LC64(char mem_H, char mem_L);

void WritePage_24LC64(char mem_H, char mem_L,char* data,char N);
void OpenWriteStream_24LC64(char mem_H, char mem_L);
void WriteStream_24LC64(char data);

void OpenReadStream_24LC64(char mem_H, char mem_L);
char ReadStream_24LC64(char next);
void SequentialRead_24LC64(char mem_H, char mem_L,char *data,char N);

void CloseStream_24LC64();

void Init_24LC64(){
    //Set WP pin
    ANSELCbits.ANSC6=0;
    TRISCbits.TRISC6=0;
    WP_Mode(0);
    
    I2C_SPEED(SPEED_N_100kHz);
    I2C_INIT();
}

void WP_Mode(unsigned char mode){
    WP=(mode & 0x01);
}

void WriteTo_24LC64(char mem_H, char mem_L, char data){
    I2C_STR();
    WriteI2C(ADR_24LC64_W);
    WriteI2C(mem_H);
    WriteI2C(mem_L);
    WriteI2C(data);
    I2C_STP();
    I2C_PAUSE(TIME_WRITE);
}

int ReadFrom_24LC64(char mem_H, char mem_L){
    char data=0;
    
    I2C_STR();
    WriteI2C(ADR_24LC64_W);
    WriteI2C(mem_H);
    WriteI2C(mem_L);
    
    I2C_STR();
    WriteI2C(ADR_24LC64_R);
    data=I2CRead(false);
    I2C_STP();
    
    I2C_PAUSE(TIME_READ);
    return data;
}

void CloseStream_24LC64(){
    I2C_STP();
    I2C_PAUSE(TIME_WRITE);
}

void OpenWriteStream_24LC64(char mem_H, char mem_L){
    I2C_STR();
    WriteI2C(ADR_24LC64_W);
    WriteI2C(mem_H);
    WriteI2C(mem_L);
}

void WriteStream_24LC64(char data){
    WriteI2C(data);
    I2C_PAUSE(1);
}

void WritePage_24LC64(char mem_H, char mem_L,char* data,char N){
    if(N>PAGE_SIZE)return;
    
    OpenWriteStream_24LC64(mem_H,mem_L);
    for(char i=0;i<N;i++){
         WriteStream_24LC64(data[i]);
         if(I2CError)break;
    }
    CloseStream_24LC64();
}

char ReadStream_24LC64(char next){
    char data=0x00;
    
    data=I2CRead(next);
    I2C_PAUSE(1);
    return data;
}


void SequentialRead_24LC64(char mem_H, char mem_L,char *data, char N){
    char i=0;
    if(N>PAGE_SIZE)return;
    
    OpenReadStream_24LC64(mem_H,mem_L);
    for(char i=0;i<N-1;i++){
        data[i]=ReadStream_24LC64(true);
        if(I2CError)break;
    }
    data[N-1]=ReadStream_24LC64(false);
    CloseStream_24LC64();
}

void OpenReadStream_24LC64(char mem_H, char mem_L){
    I2C_STR();
    WriteI2C(ADR_24LC64_W);
    WriteI2C(mem_H);
    WriteI2C(mem_L);
    I2C_STP();
    
    I2C_PAUSE(1000);
    
    I2C_STR();
    WriteI2C(ADR_24LC64_R);
}
#endif	/* PIC_24LC64 */

