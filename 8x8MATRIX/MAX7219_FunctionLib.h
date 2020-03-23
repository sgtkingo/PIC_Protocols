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
 * Comments: Liblary for MAX7219
 * Revision history: 1.0 / 070320
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MAX_7219_PIC_H
#define	MAX_7219_PIC_H

#include "SPI_PICLIB.h"
#include "MAX7219_Charset.h"

#define CS_PIN 0b00000001 //RA0

#define REG_DECODE_MAX7219 0x09
#define REG_INTENSITY_MAX7219 0x0A
#define REG_SCANLIMIT_MAX7219 0x0B
#define REG_SHUTDOWN_MAX7219 0x0C
#define REG_TEST_MAX7219 0x0F

#define CMD_TEST 0x01
#define CMD_NORMAL_MODE 0x01
#define CMD_SHUTDOWN_MODE 0x00

#define CMD_NO_DECODE_MODE 0x00
#define CMD_ALL_DECODE_MODE 0xFF

#define CMD_INTENSITY_100P 0x0F
#define CMD_INTENSITY_50P 0x08
#define CMD_INTENSITY_25P 0x04
#define CMD_INTENSITY_1P 0x00

#define CMD_SCAN_ALL 0x07
#define CMD_NO_OP 0x00

unsigned char *PORT_CS=0x00; //PORT for CS
unsigned char PIN_CS=0x00; //PIN for CS 

void Init_MAX7219(unsigned char *port_cs, unsigned char cs_pin_value);
void Send_Data_MAX7219(unsigned char cmd,unsigned char data);
void Shine_LEDs_MAX7219(unsigned char display, unsigned char data);

void Test_MAX7219();
void ClearDisplay_MAX7219(unsigned char display);
void ClearALL_MAX7219();

void Shutdown_MAX7219(unsigned char mode);

void Print_Matrix_MAX7219(unsigned char *data);

void Init_MAX7219(unsigned char *port_cs, unsigned char cs_pin_value){
    PORT_CS=port_cs;
    PIN_CS=cs_pin_value;
    
    SPI_SET_CS(PORT_CS,PIN_CS,1);
    SPI_INIT();
    
    Test_MAX7219();
    Send_Data_MAX7219(REG_INTENSITY_MAX7219,CMD_INTENSITY_50P);
    Send_Data_MAX7219(REG_SCANLIMIT_MAX7219,CMD_SCAN_ALL);
    Send_Data_MAX7219(REG_DECODE_MAX7219,CMD_NO_DECODE_MODE);
    Send_Data_MAX7219(REG_SHUTDOWN_MAX7219,CMD_NORMAL_MODE);
}

void Send_Data_MAX7219(unsigned char cmd,unsigned char data){
    SPI_SET_CS(PORT_CS,PIN_CS,0);
    SPI_WRITE(cmd);
    SPI_WRITE(data);
    SPI_SET_CS(PORT_CS,PIN_CS,1);
    
    SPI_PAUSE(1000);
}

//Set DATA to display 1-7, 0 == NO_OP
void Shine_LEDs_MAX7219(unsigned char display, unsigned char data){
    if( display > MAX7219_MAX_DISPLAYs ) display=0x08;
    
    SPI_SET_CS(PORT_CS,PIN_CS,0);
    SPI_WRITE(display);
    SPI_WRITE(data);
    SPI_SET_CS(PORT_CS,PIN_CS,1);  
    SPI_PAUSE(1000);   
}

void Send_NO_OP(){
    SPI_SET_CS(PORT_CS,PIN_CS,0);
    SPI_WRITE(CMD_NO_OP);
    SPI_WRITE(0x00);
    SPI_SET_CS(PORT_CS,PIN_CS,1);
    
    SPI_PAUSE(1000);    
}

void Print_Matrix_MAX7219(unsigned char *data){
    for(unsigned char i=0;i<MAX7219_MAX_DISPLAYs;i++){
        Shine_LEDs_MAX7219(i+1, data[i]);
    }
}

void Test_MAX7219(){
    Send_Data_MAX7219(REG_TEST_MAX7219,CMD_TEST);
    __delay_ms(500);
    Send_Data_MAX7219(REG_TEST_MAX7219,0);
}

void Shutdown_MAX7219(unsigned char mode){
    Send_Data_MAX7219(REG_SHUTDOWN_MAX7219,mode);
}

void ClearDisplay_MAX7219(unsigned char display){
    Shine_LEDs_MAX7219(display,0x00);
}
void ClearALL_MAX7219(){
    for(unsigned char i=1;i<=MAX7219_MAX_DISPLAYs;i++){
        ClearDisplay_MAX7219(i);
    }
}
#endif	/* MAX_7219_PIC_H */

