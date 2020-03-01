
// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LCD_CONFIGLIB
#define	LCD_CONFIGLIB

#include <xc.h> // include processor files - each processor file is guarded.  
#define _XTAL_FREQ 16000000

// set up the timing for the LCD delays
#define LCD_enableDelayTime    100     // ~1ms
#define LCD_shortDelayTime     2000     // ~2ms
#define LCD_longDelayTime      5000     // ~5ms
#define LCD_StartupTime        15000    // ~15ms

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
#define DAT         1

//define LCD primal port
#define LCD_PORT PORTD 
//#define LCD_PWR      PORTBbits.RB0                      // LCD power pin
#define LCD_EN       PORTCbits.RC6                     // LCD enable
#define LCD_RW       PORTEbits.RE2                      // LCD read/write line
#define LCD_RS       PORTCbits.RC7                      // LCD register select line

#define NB_LINES    2                                   // Number of display lines
#define NB_COL      16                                  // Number of characters per line


void LCD_Config();
void LCD_Initialize();
void LCD_Clear();
void LCD_Config();
void LCD_Enable();
void LCDWriteByte(char data_byte,char rs);
void LCDPutCmd(char cmd);
void LCDPutChar(char ch);
void LCD_pause(unsigned int t);
void LCDGoto(char pos,char ln);
void LCDPutStr(const char *str);

#endif	/* LCD_CONFIGLIB */

