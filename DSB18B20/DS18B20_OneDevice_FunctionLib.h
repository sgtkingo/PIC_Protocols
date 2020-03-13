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
 * Comments: DS18B20 one device function lib
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef DS18B20_ONEDEVICE
#define	DS18B20_ONEDEVICE

#define N_DS18B20 8

//All CMD must be send in direct communication order: 
//**** 1: INIT (Reset)
//**** 2: ROM CMD (can be SKIP)
//**** 3: FCE CMD (call some internal function)

enum ROM_commands { ROM_searchROM=0xF0, ROM_skipROM=0xCC, ROM_readROM=0x33, 
ROM_matchROM=0x55, ROM_alarmSearch=0xEC};

enum FCE_commands { FCE_convertT=0x44, FCE_writeToSCR=0x4E, FCE_readFromSCR=0xBE,
FCE_copytoSCRtoEEPROM=0x48, FCE_reloadEEPROMDataToSCR=0xB8, FCE_readPowerSupply=0xB4,
FCE_8bitFamilyCode=0x28 };

//FCE_writetoSCR=0x4E command send 3 BYTE! All 3 must be send, in LSB mode! 
//*** FIRST BYTE - > 2 BYTE of SCR (TH)
//*** SECOND BYTE - > 3 BYTE of SCR (TL)
//*** THIRT BYTE - > 4 BYTE of SCR (CONFIG)

//FCE_readFromSCR=0xBE command read all 9 BYTES prom SCR, in LSB mode! 
//*** 0 BYTE - > Temp LSB (0x50) 
//*** 1 BYTE - > Temp MSB (0x05) 
//*** 2 BYTE - > TH or GP (Generel Purpose) Byte 1
//*** 3 BYTE - > TL or GP (Generel Purpose) Byte 2
//*** 4 BYTE - > Config Register
//*** 5 BYTE - > Reserved! (R-O)
//*** 6 BYTE - > Reserved! (R-O)
//*** 7 BYTE - > Reserved! (R-O)
//*** 8 BYTE - > CRC (Generated by ROM 64bit mem and SCR 8BYTE)
//This CMD may be termined by RESET sequence if only some bytes needed

//FCE_copytoSCRtoEEPROM=0x48 command copy 3 defined byte from SCR to EEPROM 
//*** FIRST BYTE - > 2 BYTE of SCR (TH)
//*** SECOND BYTE - > 3 BYTE of SCR (TL)
//*** THIRT BYTE - > 4 BYTE of SCR (CONFIG)

//FCE_readPowerSupply=0xB4 command return information about powering device
//RETURN 0 - Parasite Power
//RETURN 1 - External Power

enum resolutions { res_9b=0b00011111, res_10b=0b00111111, res_11b=0b01011111 , res_12b=0b01111111 };
bit senzorPwrSupply;
bit tempPolarity;
char temperatureDataLSB,temperatureDataMSB;
char tempCelsia, tempMiliCelsia ;
unsigned int temperatureData=0;
    
unsigned char senzorROMasArray[N_DS18B20]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char familyName, CRC;
unsigned long long int SN;

#include "OW_PICLIB.h"

void Init_DS18B20();
void Clear_DS18B20();

char configTemperatureSenzor(unsigned char resolution);
void setResultResolution(unsigned char resolution);

void readROM();
void parseROMData();
void readPowerSuply();

void readTemperature();
void parseTemeperatureData(char lsb, char msb);

char getTempCelsia();
char getTempMiliCelsia();
unsigned int getTemperatureData();
bit getTempPolarity();

void Init_DS18B20(){
    OW_Init();
    Clear_DS18B20();
}

void Clear_DS18B20(){
    senzorPwrSupply=0;
    tempPolarity=0;
    temperatureDataLSB=0;
    temperatureDataMSB=0;
    tempCelsia=tempMiliCelsia=0;
    temperatureData=0;
    familyName=0;
    CRC=0;
    SN=0;
}

char configTemperatureSenzor(unsigned char resolution){
    if(OWTouchReset())return 1;
    asm("NOP");
    readROM();
    readPowerSuply();
    setResultResolution(resolution); //Resolution to 12b
    return 0;
}

void setResultResolution(unsigned char resolution){
    if(OWTouchReset())return;
    
    OWWriteByte(ROM_skipROM); //Skip-EEPROM
    OWWriteByte(FCE_writeToSCR); //Write to SRAM memory 
    OWWriteByte(0x00); //TH byte 
    OWWriteByte(0x00); //TL byte
    OWWriteByte(resolution); //Config resolution 
}

void readROM(){ 
    if(OWTouchReset())return;
    
    char lastReadByte=0;
    OWWriteByte(ROM_readROM); //Read ROM 
    for(int i=0;i<N_DS18B20;i++){
        lastReadByte=OWReadByte();
        senzorROMasArray[i]=lastReadByte;
    }
    parseROMData();
}

void parseROMData(){
    familyName=senzorROMasArray[N_DS18B20-1];
    CRC=senzorROMasArray[0];
    
    for(int i=0;i<N_DS18B20-2;i++){
        SN|=senzorROMasArray[i];
        SN<<=8;
    }
}

void readPowerSuply(){
    if(OWTouchReset())return;
    OWWriteByte(ROM_skipROM); //Skip-EEPROM
    OWWriteByte(FCE_readPowerSupply); //Read power supply
    
    senzorPwrSupply=OWReadBit();
}

void readTemperature(){
    if(OWTouchReset())return;
        
    temperatureDataLSB=0;
    temperatureDataMSB=0;
    
    OWWriteByte(ROM_skipROM);  
    OWWriteByte(FCE_convertT); //Conver cmd
    while(!OWReadBit()){
        __delay_ms(1);
    }
    
    if(OWTouchReset())return;
    OWWriteByte(ROM_skipROM); 
    
    OWWriteByte(FCE_readFromSCR); //Read memory
    
    temperatureDataLSB=OWReadByte(); //LSB
    temperatureDataMSB=OWReadByte(); //MSB
    OWTouchReset();
    asm("NOP");
    parseTemeperatureData(temperatureDataLSB,temperatureDataMSB);
}

void parseTemeperatureData(char lsb, char msb){
    unsigned int tmp=(msb & 0b00000111);
    tmp<<=8;
    
    tempPolarity=(msb & 0xF000);
    temperatureData=(tmp|lsb);
    
    tempCelsia=(temperatureData >> 4);
    tempMiliCelsia=(temperatureData & 0x000F);
}

char getTempCelsia(){
    return tempCelsia;
}
char getTempMiliCelsia(){
    float result=(float)tempMiliCelsia/16;
    result*=100;
    
    return (char)result;
}
unsigned int getTemperatureData(){
    return temperatureData;
}
bit getTempPolarity(){
    return tempPolarity;
}

unsigned char *dataROMAsArray(){
    return senzorROMasArray;
}

#endif	/* DS18B20_ONEDEVICE */

