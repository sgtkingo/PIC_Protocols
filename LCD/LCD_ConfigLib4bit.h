/*  
 * Author: Jiri Konecny 
 * Version: 070420
 * Comments: LCD config header
 */
// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef LCD_CONFIGLIB
#define	LCD_CONFIGLIB

#include <xc.h> // include processor files - each processor file is guarded.
#define _XTAL_FREQ 16000000

// set up the timing for the LCD delays
#define TIME_Enable    5        // 5 us
#define TIME_Short     100     // 100 us
#define TIME_Long      3000     // ~3ms
#define TIME_Startup       20000    // ~20ms

// Command set for Hitachi 44780U LCD display controller
#define LCD_CLEAR           0x01
#define LCD_HOME            0x02
#define LCD_CURSOR_BACK     0x10
#define LCD_CURSOR_FWD      0x14
#define LCD_PAN_LEFT        0x18
#define LCD_PAN_RIGHT       0x1C
#define LCD_CURSOR_OFF      0x0C
#define LCD_CURSOR_ON       0x0E
#define LCD_CURSOR_BLINK    0x0F
#define LCD_CURSOR_LINE2    0xC0

// display controller setup commands from page 46 of Hitachi datasheet
#define FUNCTION_SET4bit    0x28          // 4 bit interface, 2 lines, 5x8 font
#define FUNCTION_SET8bit    0x38          // 8 bit interface, 2 lines, 5x8 font
#define ENTRY_MODE          0x06          // increment mode
#define DISPLAY_SETUP       0x0C          // display on, cursor off, blink offd
#define DISPLAY_OFF         0x08          // display off, cursor off, blink offd

//----------------------------------------------------------------------
// Definitions specific to the PICDEM 2 Plus
// These apply to the Black (2011) version.
//----------------------------------------------------------------------

// single bit for selecting command register or data register
#define ISR        0
#define DAT        1

//define LCD primal port
#define LCD_PORT PORTD
//#define LCD_PWR      PORTBbits.RB0                      // LCD power pin
#define LCD_EN       PORTDbits.RD3                    // LCD enable
#define LCD_RW       PORTDbits.RD2                      // LCD read/write line
#define LCD_RS       PORTDbits.RD1                      // LCD register select line
#define LCD_ON       PORTDbits.RD0                      // LCD ON pin

#define NB_LINES    2                                   // Number of display lines
#define NB_COL      16                                  // Number of characters per line


void LCD_Initialize();
/*
*********************************************************************************************************
* LCD_Clear()
*
* Description: Clear LCD, goto 0,0 
* Arguments  : none
* Returns    : none
*********************************************************************************************************
*/
void LCD_Clear();
/*
*********************************************************************************************************
* LCD_Config()
*
* Description: Config LCD
* Arguments  : none
* Returns    : none
*********************************************************************************************************
*/
void LCD_Config();
/*
*********************************************************************************************************
* LCD_Enable()
*
* Description: LCD enable routine
* Arguments  : none
* Returns    : none
*********************************************************************************************************
*/
void LCD_Enable();
/*
*********************************************************************************************************
* LCD_Welcome()
*
* Description: LCD Welcome routine
* Arguments  : none
* Returns    : none
*********************************************************************************************************
*/
void LCD_Welcome(); 
/*
*********************************************************************************************************
* LCDWriteNibble(char data_nibble)
*
* Description: Write nibble to LCD
* Arguments  : data_nibble - nibble to write
* Returns    : none
*********************************************************************************************************
*/
void LCDWriteNibble(char data_nibble);
/*
*********************************************************************************************************
* LCDWriteByte(char data_byte,char rs)
*
* Description: Write byte to LCD, specific mode
* Arguments  : data_byte - byte to write, rs - RS signal set
* Returns    : none
*********************************************************************************************************
*/
void LCDWriteByte(char data_byte,char rs);
/*
*********************************************************************************************************
* LCDPutCmd(char cmd)
*
* Description: Send command to LCD
* Arguments  : cmd - LCD command
* Returns    : none
*********************************************************************************************************
*/
void LCDPutCmd(char cmd);
/*
*********************************************************************************************************
* LCDPutChar(char ch)
*
* Description: Send character to LCD
* Arguments  : ch - ASCII character
* Returns    : none
*********************************************************************************************************
*/
void LCDPutChar(char ch);
/*
*********************************************************************************************************
* LCD_pause(unsigned int t)
*
* Description: Wait routine in ms
* Arguments  : t - time in ms
* Returns    : none
*********************************************************************************************************
*/
void LCD_pause(unsigned int t);
/*
*********************************************************************************************************
* LCDGoto(char pos,char ln)
*
* Description: GoTo command for LCD
* Arguments  : pos - position in line(from 0), ln - line number (from 0)
* Returns    : none
*********************************************************************************************************
*/
void LCDGoto(char pos,char ln);
/*
*********************************************************************************************************
* LCDPutStr(const char *str)
*
* Description: Put string to LCD
* Arguments  : str - string pointer
* Returns    : none
*********************************************************************************************************
*/
void LCDPutStr(const char *str);
/*
*********************************************************************************************************
* LCDClearLine(char ln)
*
* Description: Clear specific LCD line
* Arguments  : ln - line number (from 0)
* Returns    : none
*********************************************************************************************************
*/
void LCDClearLine(char ln);

#endif	/* LCD_CONFIGLIB */

