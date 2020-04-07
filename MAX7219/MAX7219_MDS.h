/*  
 * Author: Jiri Konecny 
 * Version: 070420
 * Comments: MAX7219 8x8 dot matrix header for PIC
 */


// This is a guard condition so that contents of this file are not included, using 2to4 demuxer
// more than once.
#ifndef PIC_MAX_7219_MDS_H
#define	PIC_MAX_7219_MDS_H

void Inits_MAX7219_MDS(){
    ACTUAL_MATRIX=0;
    Init_DEMUXER();

    SPI_INIT();
    
    for(unsigned char i=0;i<N_MATRIX;i++){
        Test_MAX7219_MD(i);
        Send_Data_MAX7219_MD(REG_INTENSITY_MAX7219,CMD_INTENSITY_50P,i);
        Send_Data_MAX7219_MD(REG_SCANLIMIT_MAX7219,CMD_SCAN_ALL,i);
        Send_Data_MAX7219_MD(REG_DECODE_MAX7219,CMD_NO_DECODE_MODE,i);
        Send_Data_MAX7219_MD(REG_SHUTDOWN_MAX7219,CMD_NORMAL_MODE,i);
        SPI_PAUSE(5000);
    }
}

void selectMatrix(unsigned char matrix){
    if( matrix < 0 || matrix >= N_MATRIX )return;
    ACTUAL_MATRIX=matrix;
}

void Send_Data_MAX7219_MD(unsigned char cmd,unsigned char data, unsigned char device){
    selectMatrix(device);
    Write_Data_MAX7219_MD(cmd,data);
}

void Write_Data_MAX7219_MD(unsigned char cmd,unsigned char data){
    Set_DEMUX(ACTUAL_MATRIX);
    SPI_WRITE(cmd);
    SPI_WRITE(data);
    Set_DEMUX(NON_MATRIX);
    
    SPI_PAUSE(500);    
}

//Set DATA to display 1-7, 0 == NO_OP
void Shine_LEDs_MAX7219_MD(unsigned char display, unsigned char data, unsigned char device){
    if( display > MAX7219_MAX_DISPLAYs ) display=0x08; 
    Send_Data_MAX7219_MD(display,data,device);  
}

void Print_Matrix_MAX7219_MD(unsigned char *data, unsigned char device){
    for(unsigned char i=0;i<MAX7219_MAX_DISPLAYs;i++){
        Shine_LEDs_MAX7219_MD(i+1, data[i],device);
    }
}

void Test_ALL_MAX7219_MD(){
    for(unsigned char i=0;i<N_MATRIX;i++){
        Test_MAX7219_MD(i);
    }    
}

void Test_MAX7219_MD(unsigned char device){
    Send_Data_MAX7219_MD(REG_TEST_MAX7219,CMD_TEST,device);
    delay_ms(500);
    Send_Data_MAX7219_MD(REG_TEST_MAX7219,0,device);
}

void Shutdown_MAX7219_MD(unsigned char mode, unsigned char device){
    Send_Data_MAX7219_MD(REG_SHUTDOWN_MAX7219,mode,device);
}

void ClearDisplay_MAX7219_MD(unsigned char display, unsigned char device){
    Shine_LEDs_MAX7219_MD(display,0x00,device);
}
void Clear_Matrix_MAX7219_MD(unsigned char device){
    for(unsigned char i=0;i<MAX7219_MAX_DISPLAYs;i++){
        ClearDisplay_MAX7219_MD(i+1,device);
    }
}
void Clear_ALL_MD(){
    for(unsigned char i=0;i<N_MATRIX;i++){
        Clear_Matrix_MAX7219_MD(i);
    }
}
#endif	/* PIC_MAX_7219_MDS_H */

