/*  
 * Author: Jiri Konecny 
 * Version: 070420
 * Comments: EEPROM function header for PIC
 */
// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PIC_EEPROM_FUNCTIONLIB_H
#define	PIC_EEPROM_FUNCTIONLIB_H
#include <xc.h> // include processor files - each processor file is guarded. 

#define WRITE EECON1bits.WR
#define READ EECON1bits.RD

#define WRITE_ENABLE EECON1bits.WREN
#define WRITE_ERROR EECON1bits.WRERR

/*
*********************************************************************************************************
* Init_EEPROM()
*
* Description: Init internal uC EEPROM
* Arguments  : none
* Returns    : none
*********************************************************************************************************
*/
void Init_EEPROM();
/*
*********************************************************************************************************
* Write_EEPROM(unsigned int addr,unsigned char data)
*
* Description: Write to internal uC EEPROM
* Arguments  : addr - EEADR low byte address, data - data to write
* Returns    : none
*********************************************************************************************************
*/
void Write_EEPROM(unsigned int addr,unsigned char data);
/*
*********************************************************************************************************
* Read_EEPROM(unsigned int addr)
*
* Description: Read from internal uC EEPROM
* Arguments  : addr - EEADR low byte address
* Returns    : data from eeprom address
*********************************************************************************************************
*/
unsigned char Read_EEPROM(unsigned int addr);

void Init_EEPROM(){
    //EEDATA eeprom data register
    //EECON2 catch register
    //EEADR + EEADRH = 10bit eemprom address (0-1023)
    EECON1=0b00000000; //Access EEPROM, deny Write
    EEADRH=0x00; //will use only down 8bit of addr (EEADR)
    //PIE2bits.EEIE=1; //intterupt enable
}

void Write_EEPROM(unsigned int addr,unsigned char data){
    EEADR=(addr & 0xFF); //Set EEPROM address
    EEADRH=((addr >> 8)&0x03); //Set EEPROM H address
    EEDATA=data; //save data to eeprom register
    WRITE_ENABLE=1;
    
    ///NEED by proceser - INIT WRITE operation
    EECON2=0x55; 
    EECON2=0x0AA;
    ///
    WRITE=1; //Set WRITE operation
    while(WRITE && !WRITE_ERROR)NOP(); //till to complete
    WRITE_ENABLE=0;    
    
    delay_ms(5);
}

unsigned char Read_EEPROM(unsigned int addr){
    unsigned char data=0;
    EEADR=(addr & 0xFF); //Set EEPROM address
    EEADRH=((addr >> 8)&0x03); //Set EEPROM H address
    READ=1; //Set READ operation
    
    while(READ)NOP(); //till to complete
    data=EEDATA; //store data  
    
    return data;
}

#endif	/* PIC_EEPROM_FUNCTIONLIB_H */

