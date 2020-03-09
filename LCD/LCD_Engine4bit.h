#ifndef LCD_ENGINE
#define	LCD_ENGINE

#include "LCD_ConfigLib4bit.h"
#include "LCD_Charset.h"

void LCD_Initialize()
{
    LCD_Config();
    // required by display controller to allow power to stabilize
    LCD_ON=1; //On
    LCD_pause(TIME_Startup);

    LCDWriteNibble(0x03<<4);
    LCD_pause(10000);
    LCDWriteNibble(0x03<<4);
    LCD_pause(1000);
    LCDWriteNibble(0x03<<4);
    LCD_pause(1000);
    LCDWriteNibble(0x02<<4);
    LCD_pause(500);

    // set interface size, # of lines and font
    LCDPutCmd(FUNCTION_SET4bit);

    //set display clear
    LCDPutCmd(LCD_CLEAR);
    LCD_pause(2500);

    // turn off display and sets up cursor
    LCDPutCmd(DISPLAY_SETUP);

    // set cursor movement direction
    LCDPutCmd(ENTRY_MODE);
}

void LCD_Clear(){
    LCDPutCmd(LCD_CLEAR);
    LCD_pause(TIME_Long);//Wait to clear display
}

void LCD_Config(){
    ANSELD=0;
    TRISD=0x00; //RB0-RB7 to DB0-DB7
    PORTD=0x00;
    NOP();

    LCD_RW=0;  //mode WRITE
    LCD_RS=0; //ROM select
    LCD_EN=0; //Not enable
    LCD_ON=0; //Off
}

void LCD_Enable(){
    NOP();
	LCD_EN  = 1;
    LCD_pause(TIME_Enable);
    LCD_EN  = 0;
    NOP();
}

void LCDWriteNibble(char data_nibble)
{
    LCD_PORT&=0x0F;
    char data=(data_nibble & 0xF0);
    LCD_pause(1);
 
    LCD_PORT|=data;
    LCD_Enable();
    NOP();
}


void LCDWriteByte(char data_byte,char rs)
{
    LCD_RW = 0;
    LCD_RS = rs;
    LCD_pause(40);//wait 40us
    
    LCDWriteNibble(data_byte & 0xF0);
    NOP();
    LCDWriteNibble(data_byte<<4 & 0xF0);
    NOP();
}

void LCD_pause(unsigned int t){
    while(t--)__delay_us(1);
}

void LCDPutChar(char ch)
{
    //Send byte data
    LCDWriteByte(ch,DAT);
    LCD_pause(TIME_Short);//wait to print char
}

void LCDPutCmd(char cmd)
{
    //Send cmd byte
    LCDWriteByte(cmd,ISR);
    LCD_pause(TIME_Short); //wait to execute cmd
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
    LCD_pause(TIME_Short);
}

void LCDClearLine(char ln){
    if (ln > (NB_LINES-1))
    {
        // Just do nothing
        return;
    }
    LCDGoto(0,ln);
    LCDPutStr(LINE);
}

void LCD_Welcome(){
    LCD_Clear();
    LCDPutStr("Greetins :)");
    delay_ms(2500);
    LCD_Clear();    
}

#endif	/* LCD_ENGINE */

