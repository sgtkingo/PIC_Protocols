#include "PICF18LF46K22_ConfigSFR.h"

#define LED LATDbits.LATD0

//declared init fce
void InitDevice(){
    OSCCON=0b01111100; //osc setting, 16 MHz, internal by FOSCH
    OSCTUNE=0x00; //PLLx4 OFF
    ANSELD=0x00;
    TRISD=0x00;
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

    while(1){
        NOP();
    }
    return;
}

void interrupt IRS(void){
    return;
}
