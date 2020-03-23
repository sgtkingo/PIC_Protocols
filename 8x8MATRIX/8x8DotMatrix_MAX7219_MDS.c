#include "PICF18LF46K22_ConfigSFR.h"
#include "MAX7219_MultiDeviceSimple.h"

const unsigned char bit_image_D1[MAX7219_MAX_DISPLAYs]={
    0b00011000,
    0b00100100,
    0b01000010,
    0b10100101,
    0b10011001,
    0b01011010,
    0b00111100,
    0b00011001,
};
const unsigned char bit_image_D2[MAX7219_MAX_DISPLAYs]={
    0b00000000,
    0b00100100,
    0b01000010,
    0b10100101,
    0b10011001,
    0b01011010,
    0b00111100,
    0b00000001,
};
const unsigned char bit_image_D3[MAX7219_MAX_DISPLAYs]={
    0b00011000,
    0b00100100,
    0b01000010,
    0b10100101,
    0b10011001,
    0b01011010,
    0b00111100,
    0b11111111,
};
const unsigned char bit_image_D4[MAX7219_MAX_DISPLAYs]={
    0b11111111,
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
    
    SPI_SET_CS(&PORTA,0b00000010,1); //RA1, MCP41100
 
    Inits_MAX7219_MDS();
    Clear_ALL_MD();
    delay_ms(1000);
    
    Print_Matrix_MAX7219_MD(getNumber(0),0);
    Print_Matrix_MAX7219_MD(getNumber(1),1);
    Print_Matrix_MAX7219_MD(getNumber(2),2);
    Print_Matrix_MAX7219_MD(getNumber(3),3);
    
    LATD=0xFF;
    while(1){
        NOP();
    }
    return;
}
