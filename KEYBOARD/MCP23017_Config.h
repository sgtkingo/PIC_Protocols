/*  
 * Author: Jiri Konecny 
 * Version: 070420
 * Comments: Config header for MCP23017
 */

/*IOCON.BANK = 1
Address
IOCON.BANK = 0 
        Access to:
00h 00h IODIRA
10h 01h IODIRB
01h 02h IPOLA
11h 03h IPOLB
02h 04h GPINTENA
12h 05h GPINTENB
03h 06h DEFVALA
13h 07h DEFVALB
04h 08h INTCONA
14h 09h INTCONB
05h 0Ah IOCON
15h 0Bh IOCON
06h 0Ch GPPUA
16h 0Dh GPPUB
07h 0Eh INTFA
17h 0Fh INTFB
08h 10h INTCAPA
18h 11h INTCAPB
09h 12h GPIOA
19h 13h GPIOB
0Ah 14h OLATA
1Ah 15h OLATB*/

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MCP23017_CONFIG
#define	MCP23017_CONFIG

#include "I2C_PICLIB.h"

#define ADR_MCP23017 0b01000000
// address_of_24LC64_in the BUS I2C, W	
#define	ADR_MCP23017_W (ADR_MCP23017 MASK_W)	
// address_of_24LC64_in the BUS I2C, R	
#define	ADR_MCP23017_R (ADR_MCP23017 MASK_R)

#define MCP2017_MEM_SIZE 22

#define PORT_A_MASK 0x00
#define PORT_B_MASK 0x10

#define IOCON_NS 0x05  //Config IOCON, no seq BANK=1
#define IOCON_S 0x0B  //Config IOCON, seq BANK=0

#define IOCON_NOSEQ_8bit 0b10111010  //Disable Sequential mode, BANK=1, Slew Rate=0, no MIRROR, HW en
#define IOCON_SEQ_16bit 0b00011010 //Enable Sequential mode, BANK=0, Slew Rate=0, no MIRROR, HW en

#define GPPU 0x06   //Config GPPU, pullup
#define INTF 0x07   //Config INTF, interrrupt flag
#define INTCAP 0x08 //Config INTCAP, interrrupt lach reg ReadOnly
#define GPINTEN 0x02 //Config GPINTEN, IOC pins ON/OFF
#define INTCON_ 0x04 //Config INTCON, IOC swap DEFVAL/previous value
#define DEFVAL 0x03 //Config defalut value for interrupt-on-change on port
#define IPOL 0x01 //Config reflex polarization of PORT (for read))

#define GPIOA 0x09   //Config GPIO, general purpose register A, for READ/W
#define GPIOB 0x19   //Config GPIO, general purpose register B, for READ/W

#define IODIRA 0x00 //Config IODIR, direction IN/OUT A
#define IODIRB 0x10 //Config IODIR, direction IN/OUT B

#define OLATA 0x0A   //Config OLATA, output latch register
#define OLATB 0x1A   //Config OLATB, output latch register

/*
*********************************************************************************************************
* Init_MCP23017()
*
* Description: Init MCP23017
* Arguments  : none
* Returns    : none
*********************************************************************************************************
*/
void Init_MCP23017();
/*
*********************************************************************************************************
* Config_MCP23017_Fast()
*
* Description: Fast auto config MCP23017
* Arguments  : none
* Returns    : none
*********************************************************************************************************
*/
void Config_MCP23017_Fast();

/*
*********************************************************************************************************
* Send_ConfigToRegister(unsigned char reg, unsigned char cmd)
*
* Description: Config device register
* Arguments  : reg - register code, cmd - data for register
* Returns    : none
*********************************************************************************************************
*/
void Send_ConfigToRegister(unsigned char reg, unsigned char cmd);
/*
*********************************************************************************************************
* ChangeIOCONMode(unsigned char mode)
*
* Description: Change memory list mode - 8/16 bit
* Arguments  : mode - IOCON mode code
* Returns    : none
*********************************************************************************************************
*/
void ChangeIOCONMode(unsigned char mode);
/*
*********************************************************************************************************
* GoToAddress(unsigned char addr)
*
* Description: Go to device memory address
* Arguments  : addr - address number
* Returns    : none
*********************************************************************************************************
*/
void GoToAddress(unsigned char addr);


/*
*********************************************************************************************************
* Read_DataFromPort(unsigned char port)
*
* Description: Read data from specific device port
* Arguments  : port - port code
* Returns    : data from port
*********************************************************************************************************
*/
unsigned char Read_DataFromPort(unsigned char port);
/*
*********************************************************************************************************
* Send_DataToPort(unsigned char port, unsigned char data)
*
* Description: Send data to specific device port
* Arguments  : port - port code, data - data to write
* Returns    : none
*********************************************************************************************************
*/
void Send_DataToPort(unsigned char port, unsigned char data);
/*
*********************************************************************************************************
* Set_DirectionOfPort(unsigned char port, unsigned char dir)
*
* Description: Send I/O direction of specific device port
* Arguments  : port - port code, dir - direction value
* Returns    : none
*********************************************************************************************************
*/
void Set_DirectionOfPort(unsigned char port, unsigned char dir);

/*
*********************************************************************************************************
* Set_IOC(unsigned char port, unsigned char pins, unsigned char mode, unsigned char defval)
*
* Description: Set IOC for specific device port
* Arguments  : port - port code, pins - specific pins, mode - IOC mode, defval - IOC default value
* Returns    : none
*********************************************************************************************************
*/
void Set_IOC(unsigned char port, unsigned char pins, unsigned char mode, unsigned char defval);
/*
*********************************************************************************************************
* Get_IOC_Flag(unsigned char port)
*
* Description: Get IOC flags of specific device port
* Arguments  : port - port code
* Returns    : IOC flags value
*********************************************************************************************************
*/
unsigned char Get_IOC_Flag(unsigned char port);
/*
*********************************************************************************************************
* Get_IOC_Cap(unsigned char port)
*
* Description: Get IOC last captured flags of specific device port
* Arguments  : port - port code
* Returns    : IOC last captured flags value
*********************************************************************************************************
*/
unsigned char Get_IOC_Cap(unsigned char port);

/*
*********************************************************************************************************
* Set_PullUp(unsigned char port, unsigned char pins)
*
* Description: Set pull-up resistors for specific device port
* Arguments  : port - port code, pins - pins for pull-up
* Returns    : none
*********************************************************************************************************
*/
void Set_PullUp(unsigned char port, unsigned char pins);
/*
*********************************************************************************************************
* Set_PortPol(unsigned char port, unsigned char pins)
*
* Description: Set logical polarity for specific device port
* Arguments  : port - port code, pins - pins for set polarity
* Returns    : none
*********************************************************************************************************
*/
void Set_PortPol(unsigned char port, unsigned char pins);

#endif	/* MCP23017_CONFIG */

