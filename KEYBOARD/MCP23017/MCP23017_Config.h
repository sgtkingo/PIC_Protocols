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


#endif	/* XC_HEADER_TEMPLATE_H */

