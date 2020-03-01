#ifndef LCD_ENGINE
#define	LCD_ENGINE

#include "LCD_ConfigLib.h"
void LCD_Initialize()
{
    LCD_Config();
    // required by display controller to allow power to stabilize
    LCD_pause(LCD_StartupTime);
    LCD_RS=LCD_RW=0;

    LCD_pause(LCD_shortDelayTime);
    
    LCDPutCmd(0x3);
    LCD_pause(5000);
    LCDPutCmd(0x3);
    LCD_pause(300);
    LCDPutCmd(0x3);
    LCDPutCmd(0x2);

    // set interface size, # of lines and font
    LCDPutCmd(FUNCTION_SET8bit);

    // turn off display and sets up cursor
    LCDPutCmd(DISPLAY_SETUP);
    
    //set display clear
    LCDPutCmd(LCD_CLEAR);
    
    // set cursor movement direction
    LCDPutCmd(ENTRY_MODE);
}

void LCD_Clear(){
    LCDPutCmd(LCD_CLEAR);
    LCDPutCmd(LCD_HOME);
    LCD_pause(LCD_longDelayTime);//Wait to clear display
}

void LCD_Config(){
    ANSELD=0;
    TRISD=0x00; //RD0-RD7 to DB0-DB7
    PORTD=0x00;
    
    ANSELCbits.ANSC6=0;
    ANSELCbits.ANSC7=0;
    ANSELEbits.ANSE2=0;
    TRISCbits.TRISC6=0;
    TRISCbits.TRISC7=0;
    TRISEbits.TRISE2=0;
    NOP();
    
    LCD_RW=0;  //mode WRITE
    LCD_RS=0; //ROM select
    LCD_EN=0; //Not enable    
}

void LCD_Enable(){
	LCD_EN  = 1; 
    LCD_pause(LCD_enableDelayTime);
    LCD_EN  = 0; 
}


void LCDWriteByte(char data_byte,char rs)
{
    // set data/instr bit to 0 = insructions; 1 = data
    LCD_RS = rs;
    // RW - set write mode
    LCD_RW = 0;
    //EN ready
    LCD_EN  = 1; 
    PORTD=(data_byte);
    LCD_Enable();  
}

void LCD_pause(unsigned int t){
    while(t--)__delay_us(1);
}

void LCDPutChar(char ch)
{
    LCD_pause(LCD_shortDelayTime);//wait to print char
    //Send byte data
    LCDWriteByte(ch,DAT);
}

void LCDPutCmd(char cmd)
{
    LCD_pause(LCD_shortDelayTime); //wait to execute cmd
    //Send cmd byte
    LCDWriteByte(cmd,ISR);
    
}

void LCDPutStr(const char *str)
{
    char i=0;
    
    // While string has not been fully traveresed
    while (str[i])
    {
        // Go display current char
        LCDPutChar(str[i++]);
    }
    
}

void LCDGoto(char pos,char ln)
{
    // if incorrect line or column
    if ((ln > (NB_LINES-1)) || (pos > (NB_COL-1)))
    {
        // Just do nothing
        return;
    }

    // LCD_Goto command
    LCDPutCmd((ln == 1) ? (0xC0 | pos) : (0x80 | pos));

    // Wait for the LCD to finish
    LCD_pause(LCD_shortDelayTime);
}

#endif	/* LCD_ENGINE */

