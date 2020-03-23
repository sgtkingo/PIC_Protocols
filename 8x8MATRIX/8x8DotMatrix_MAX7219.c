#include "PICF18LF46K22_ConfigSFR.h"
#include "MAX7219_FunctionLib.h"

const unsigned char bit_image[MAX7219_MAX_DISPLAYs]={
    0b00011000,
    0b00100100,
    0b01000010,
    0b10100101,
    0b10011001,
    0b01011010,
    0b00111100,
    0b00011001,
};
//declared init fce
void InitDevice(){
    OSCCON=0b01111100; //osc setting, 16 MHz, internal by FOSCH
    OSCTUNE=0x00; //PLLx4 OFF
    ANSELD=0x00;
    TRISD=0x00;
    
    //CS for 8x8LEDMATRIX
    ANSELAbits.ANSA0=0;
    TRISAbits.RA0=0;
    PORTAbits.RA0=1;
    
    //CS for MCP41100
    ANSELAbits.ANSA1=0;
    TRISAbits.RA1=0;
    PORTAbits.RA1=1;
}
//declared clear fce
void ClearDevice(){
    PORTD=0;
    LATD=0;
}
//declared test fce
void TestDevice(){
    delay_ms(1000);
    LATD=0xFF;
    delay_ms(1000);
    LATD=0x00;
}

void main(void) {
    InitDevice();
    ClearDevice();
    TestDevice();
    
    SPI_SET_CS(&PORTA,0b00000001,1); //RA0, 8x8LEDMATRIX
    SPI_SET_CS(&PORTA,0b00000010,1); //RA1, MCP41100
 
    Init_MAX7219(&PORTA,CS_PIN);
    ClearALL_MAX7219();
    delay_ms(1000);
    
    for(unsigned char disp=1;disp<=MAX7219_MAX_DISPLAYs;disp++){
        if(disp%2)Shine_LEDs_MAX7219(disp,0xAA);
        else      Shine_LEDs_MAX7219(disp,0x55);
        delay_ms(100);
    }
    delay_ms(5000);
    Print_Matrix_MAX7219(bit_image);
    //Shutdown_MAX7219(CMD_SHUTDOWN_MODE);
    
    LATD=0xFF;
    while(1){
        NOP();
    }
    return;
}
