/*  
 * Author: Jiri Konecny 
 * Version: 060420
 * Comments: DA MCP4726 function header for PIC
 */

// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef PIC_MCP4726_FUNCTIONLIB_H
#define PIC_MCP4726_FUNCTIONLIB_H

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

//main I2C address of MCP4726
#define ADR_4726 0b11000010
// address_of_24LC64_in the BUS I2C, W
#define	ADR_4726_W (ADR_4726 MASK_W)
// address_of_24LC64_in the BUS I2C, R
#define	ADR_4726_R (ADR_4726 MASK_R)

/*
*********************************************************************************************************
* Init_MCP4726()
*
* Description: Init MCP4726
* Arguments  : none
* Returns    : none
*********************************************************************************************************
*/
void Init_MCP4726();
/*
*********************************************************************************************************
* InitClear_MCP4726()
*
* Description: Default auto init MCP4726
* Arguments  : none
* Returns    : none
*********************************************************************************************************
*/
void InitClear_MCP4726();

/*
*********************************************************************************************************
* Default_Setting_MCP4726()
*
* Description: Default auto setting MCP4726
* Arguments  : none
* Returns    : none
*********************************************************************************************************
*/
void Default_Setting_MCP4726();
/*
*********************************************************************************************************
* Custom_Setting_MCP4726(char cmd, char vr, char pd,char g)
*
* Description: Custom setting setting MCP4726
* Arguments  : cmd - type of command, vr - VR(Voltage Reference) value, pd(Power Down) - PD value, g(Gain)  - G value
* Returns    : none
*********************************************************************************************************
*/
void Custom_Setting_MCP4726(char cmd, char vr, char pd,char g);

/*
*********************************************************************************************************
* ReadFrom_MCP47264()
*
* Description: Read data from MCP4726, stored to "EEPROM_MEM_4726"
* Arguments  : none
* Returns    : none
*********************************************************************************************************
*/
void ReadFrom_MCP47264();


/*
*********************************************************************************************************
* CloseStream_MCP4726()
*
* Description: Close R/W stream
* Arguments  : none
* Returns    : none
*********************************************************************************************************
*/
void CloseStream_MCP4726();
/*
*********************************************************************************************************
* OpenWriteStream_MCP4726()
*
* Description: Open write stream for write stream od data
* Arguments  : none
* Returns    : none
*********************************************************************************************************
*/
void OpenWriteStream_MCP4726();
/*
*********************************************************************************************************
* WriteStream_MCP4726(char data)
*
* Description: Write single data to MCP4726
* Arguments  : data - data to write
* Returns    : none
*********************************************************************************************************
*/
void WriteStream_MCP4726(char data);

/*
*********************************************************************************************************
* Write_DAC12b_MCP4726(char pd, unsigned int data)
*
* Description: Write value for DA, 12bit
* Arguments  : pd(Power Down) - PD value, data - data for DA 
* Returns    : none
*********************************************************************************************************
*/
void Write_DAC12b_MCP4726(char pd, unsigned int data);
/*
*********************************************************************************************************
* Write_DAC8b_MCP4726(char pd, char data)
*
* Description: Write value for DA, 8bit
* Arguments  : pd(Power Down) - PD value, data - data for DA 
* Returns    : none
*********************************************************************************************************
*/
void Write_DAC8b_MCP4726(char pd, char data);

/*
*********************************************************************************************************
* Write_ALL_MCP4726(char cmd, unsigned int data)
*
* Description: Write data to ALL memory
* Arguments  : cmd - type of command, data - data for write
* Returns    : none
*********************************************************************************************************
*/
void Write_ALL_MCP4726(char cmd, unsigned int data);
/*
*********************************************************************************************************
* Write_CMD_MCP4726(char cmd))
*
* Description: Send command to MCP4726
* Arguments  : cmd - type of command
* Returns    : none
*********************************************************************************************************
*/
void Write_CMD_MCP4726(char cmd);

/*
*********************************************************************************************************
* CreateCommand_MCP4726(char CMD, char VR, char PD, char G)
*
* Description: Create custom command for MCP4726
* Arguments  : CMD - type of command, VR - VR(Voltage Reference) value, PD(Power Down) - PD value, G(Gain)  - G value
* Returns    : created command
*********************************************************************************************************
*/
char CreateCommand_MCP4726(char CMD, char VR, char PD, char G);
/*
*********************************************************************************************************
* Set_VoltageAsData_MCP4726(unsigned int voltage_data)
*
* Description: Set voltage as 12bit data
* Arguments  : voltage_data - data for DA
* Returns    : none
*********************************************************************************************************
*/
void Set_VoltageAsData_MCP4726(unsigned int voltage_data);
/*
*********************************************************************************************************
* Clear_EEPROM_MEM()
*
* Description: Clear "EEPROM_MEM_4726" array
* Arguments  : none
* Returns    : none
*********************************************************************************************************
*/
void Clear_EEPROM_MEM();
/*
*********************************************************************************************************
* getEEPROM_MCP4726()
*
* Description: Get EEPROM data from MCP4726
* Arguments  : none
* Returns    : EEPROM data array pointer
*********************************************************************************************************
*/
char* getEEPROM_MCP4726();

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

char* getEEPROM_MCP4726(){
    return EEPROM_MEM_4726;
}

#endif	/* PIC_MCP4726_FUNCTIONLIB_H */

