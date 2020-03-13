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
 * Comments: EEPROM 22LC64 config header for PIC
 */

// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef MCP4726_DA_PIC
#define MCP4726_DA_PIC

#include "I2C_PICLIB.h"

#define CMD_DAC 0b000
#define CMD_COMMAND 0b010
#define CMD_ALL 0b011
#define CMD_CONF 0b100

#define PD_NORMAL 0b00
#define PD_1k 0b01
#define PD_125k 0b10
#define PD_640k 0b11

#define VR_VDD 0b00
#define VR_VREF_UBF 0b10
#define VR_VREF_BF 0b11

#define Gx1 0
#define Gx2 1

#define EEPROM_MEM_SIZE_4726 6
#define DA_MAX 0x0FFF

#define TIME_DA_WRITE 10
#define TIME_EEPROM_WRITE 10000
#define TIME_EEPROM_READ  7500
#define TIME_INIT  15000

char EEPROM_MEM_4726[EEPROM_MEM_SIZE_4726];

#define ADR_4726 0b11000010
// address_of_24LC64_in the BUS I2C, W
#define	ADR_4726_W (ADR_4726 MASK_W)
// address_of_24LC64_in the BUS I2C, R
#define	ADR_4726_R (ADR_4726 MASK_R)

//Intit EEPROM, enter address or set to default
void Init_MCP4726();
void InitClear_MCP4726();

void Default_Setting_MCP4726();
void Custom_Setting_MCP4726(char cmd, char vr, char pd,char g);

int ReadFrom_MCP47264();

void CloseStream_MCP4726();
void OpenWriteStream_MCP4726();
void WriteStream_MCP4726(char data);

void Write_DAC12b_MCP4726(char pd, unsigned int data);
void Write_DAC8b_MCP4726(char pd, char data);

void Write_ALL_MCP4726(char cmd, unsigned int data);
void Write_CMD_MCP4726(char cmd);

char CreateCommand_MCP4726(char CMD, char VR, char PD, char G);
void Set_VoltageAsData_MCP4726(unsigned int voltage_data);
void Clear_EEPROM_MEM();


void Init_MCP4726(){
    I2C_SPEED(SPEED_N_100kHz);
    I2C_INIT();
    InitClear_MCP4726();
    I2C_PAUSE(TIME_INIT);
}

void Default_Setting_MCP4726(){
    char command=CreateCommand_MCP4726(CMD_ALL,VR_VDD,PD_NORMAL,Gx1);
    Write_ALL_MCP4726(command,0);
}

void Custom_Setting_MCP4726(char cmd, char vr, char pd,char g){
    char command=CreateCommand_MCP4726(cmd,vr,pd,g);
    Write_CMD_MCP4726(command);
}

void InitClear_MCP4726(){
    Clear_EEPROM_MEM();
    Default_Setting_MCP4726();
}

void Clear_EEPROM_MEM(){
    for(char i=0;i<EEPROM_MEM_SIZE_4726;i++){
        EEPROM_MEM_4726[i]=0x00;
    }
}

char CreateCommand_MCP4726(char CMD, char VR, char PD, char G){
    char command=0x00;

    command|=CMD<<5;
    command|=VR<<3;
    command|=PD<<1;
    command|=G;

    return command;
}

void Set_VoltageAsData_MCP4726(unsigned int voltage_data){
    voltage_data&=DA_MAX;

    Write_DAC12b_MCP4726(PD_NORMAL,voltage_data);
}

void CloseStream_MCP4726(){
    I2C_STP();
}

void OpenWriteStream_MCP4726(){
    I2C_STR();
    WriteI2C(ADR_4726_W);
}

void WriteStream_MCP4726(char data){
    WriteI2C(data);
}

void Write_DAC12b_MCP4726(char pd, unsigned int data){
    char firstB=0x00;char secondB=0x00;
    data&=DA_MAX;

    firstB=pd<<4;
    firstB|=(data & 0x0F00)>>8;
    secondB=(data & 0x00FF);

    OpenWriteStream_MCP4726();
    WriteStream_MCP4726(firstB);
    WriteStream_MCP4726(secondB);
    CloseStream_MCP4726();
    
    I2C_PAUSE(TIME_DA_WRITE);
}

void Write_DAC8b_MCP4726(char pd, char data){
    char firstB=0x00;char secondB=0x00;
    firstB=pd<<4;
    secondB=data;

    OpenWriteStream_MCP4726();
    WriteStream_MCP4726(firstB);
    WriteStream_MCP4726(secondB);
    CloseStream_MCP4726();
    
    I2C_PAUSE(TIME_DA_WRITE);
}

void Write_ALL_MCP4726(char cmd, unsigned int data){
    char FistB=0;char SecondB=0;
    data&=DA_MAX;

    FistB=((data>>4) & 0xFF);
    SecondB=(data & 0x000F)<<4;

    OpenWriteStream_MCP4726();
    WriteStream_MCP4726(cmd);
    WriteStream_MCP4726(FistB);
    WriteStream_MCP4726(SecondB);
    CloseStream_MCP4726();
    
    I2C_PAUSE(TIME_EEPROM_WRITE);
}

void Write_CMD_MCP4726(char cmd){
    I2C_STR();
    WriteI2C(ADR_4726_W);
    WriteI2C(cmd);
    I2C_STP();
    
    I2C_PAUSE(TIME_DA_WRITE);
}

void ReadFrom_MCP4726(){
    Clear_EEPROM_MEM();

    I2C_STR();
    WriteI2C(ADR_4726_R);

    for(int i=0;i<EEPROM_MEM_SIZE_4726-1;i++){
        EEPROM_MEM_4726[i]=I2CRead(true);
        I2C_PAUSE(TIME_EEPROM_READ);
    }
    EEPROM_MEM_4726[EEPROM_MEM_SIZE_4726-1]=I2CRead(false);
    I2C_PAUSE(TIME_EEPROM_READ);
    I2C_STP();
}

#endif	/* MCP4726_DA_PIC */

