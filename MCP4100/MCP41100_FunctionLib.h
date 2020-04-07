/*  
 * Author: Jiri Konecny 
 * Version: 070420
 * Comments: Digital potenciometr MCP41100 function header for PIC
 */


// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PIC_MCP41100_FUNCTIONLIB_H
#define	PIC_MCP41100_FUNCTIONLIB_H

#include "SPI_PICLIB.h"
#define CS_PIN 0b00000010 //RA1

#define CMD_WRITE_ALL_MCP41xxx 0b00010011
#define CMD_WRITE_PT0_MCP41xxx 0b00010001
#define CMD_WRITE_PT1_MCP41xxx 0b00010010

#define CMD_SHUTDOWN_MCP41xxx 0b00100000

unsigned char *PORT_CS=0x00; //PORT for CS
unsigned char PIN_CS=0x00; //PIN for CS 

/*
*********************************************************************************************************
* Init_MCP41100(unsigned char const *port_cs, unsigned char const cs_pin_value)
*
* Description: Init MCP41100, set SPI bus and CS signal
* Arguments  : port_cs - pointer to port with CS, cs_pin_value - specific CS pin
* Returns    : none
*********************************************************************************************************
*/
void Init_MCP41100(unsigned char const *port_cs, unsigned char const cs_pin_value);
/*
*********************************************************************************************************
* Send_Data_MCP41100(unsigned char cmd,unsigned char data)
*
* Description: Send data to MCP41100
* Arguments  : cmd - command type, data - data to send
* Returns    : none
*********************************************************************************************************
*/
void Send_Data_MCP41100(unsigned char cmd,unsigned char data);
/*
*********************************************************************************************************
* Set_Resistence_MCP41100(unsigned long int resistence)
*
* Description: Set resistance of MCP41100
* Arguments  : resistence - set resistance in Ohms
* Returns    : none
*********************************************************************************************************
*/
void Set_Resistence_MCP41100(unsigned long int resistence);
/*
*********************************************************************************************************
* ShutDown_MCP41100(unsigned char mode)
*
* Description: Set shutdown mode of MCP41100
* Arguments  : mode - log. value 1/0 (0=shutdown) 
* Returns    : none
*********************************************************************************************************
*/
void ShutDown_MCP41100(unsigned char mode);

void Init_MCP41100(unsigned char const *port_cs, unsigned char const cs_pin_value){
    PORT_CS=port_cs;
    PIN_CS=cs_pin_value;
    
    //SPI_SET_CS(PORT_CS,PIN_CS,0);
    SPI_SET_CS(PORT_CS,PIN_CS,1);
    
    SPI_INIT();
    
    Send_Data_MCP41100(CMD_WRITE_PT0_MCP41xxx,0x00);
}

void Send_Data_MCP41100(unsigned char cmd,unsigned char data){
    SPI_SET_CS(PORT_CS,PIN_CS,0);
    SPI_WRITE(cmd);
    SPI_WRITE(data);
    SPI_SET_CS(PORT_CS,PIN_CS,1);
    
    SPI_PAUSE(1000);
}

void Set_Resistence_MCP41100(unsigned long int resistence){
    float proc=0;
    unsigned char bit8_result=0;
    proc=((float)(resistence)/100000);
    bit8_result=(proc*0xFF);
    Send_Data_MCP41100(CMD_WRITE_PT0_MCP41xxx,bit8_result);
}

void ShutDown_MCP41100(unsigned char mode){
    Send_Data_MCP41100(CMD_SHUTDOWN_MCP41xxx,mode & 0x01);
}

#endif	/* PIC_MCP41100_FUNCTIONLIB_H */

