/*  
 * Author: Jiri Konecny 
 * Version: 070420
 * Comments: MAX7219 8x8 dot matrix config header for PIC
 */

// This is a guard condition so that contents of this file are not included, using 2to4 demuxer
// more than once.
#ifndef MAX_7219_MDS_CONFIG_H
#define	MAX_7219_MDS_CONFIG_H

#include "SPI_PICLIB.h"
#include "MAX7219_Charset.h"
#include "PIC16F18323_DEMUXER.h"


#define REG_DECODE_MAX7219 0x09
#define REG_INTENSITY_MAX7219 0x0A
#define REG_SCANLIMIT_MAX7219 0x0B
#define REG_SHUTDOWN_MAX7219 0x0C
#define REG_TEST_MAX7219 0x0F

#define CMD_TEST 0x01
#define CMD_NORMAL_MODE 0x01
#define CMD_SHUTDOWN_MODE 0x00

#define CMD_NO_DECODE_MODE 0x00
#define CMD_ALL_DECODE_MODE 0xFF

#define CMD_INTENSITY_100P 0x0F
#define CMD_INTENSITY_50P 0x08
#define CMD_INTENSITY_25P 0x04
#define CMD_INTENSITY_1P 0x00

#define CMD_SCAN_ALL 0x07
#define CMD_NO_OP 0x00

#define N_MATRIX 4
#define NON_MATRIX 0xFF

unsigned char ACTUAL_MATRIX=0;

/*
*********************************************************************************************************
* Inits_MAX7219_MDS()
*
* Description: Init MAX7219 in multidevice simple mode
* Arguments  : none
* Returns    : none
*********************************************************************************************************
*/
void Inits_MAX7219_MDS();
/*
*********************************************************************************************************
* selectMatrix(unsigned char matrix)
*
* Description: Select matrix
* Arguments  : matrix - number of matrix 0-3
* Returns    : none
*********************************************************************************************************
*/
void selectMatrix(unsigned char matrix);

/*
*********************************************************************************************************
* Write_Data_MAX7219_MD(unsigned char cmd,unsigned char data)
*
* Description: Write data to bus
* Arguments  : cmd - command for device, data - data for device
* Returns    : none
*********************************************************************************************************
*/
void Write_Data_MAX7219_MD(unsigned char cmd,unsigned char data);
/*
*********************************************************************************************************
* Send_Data_MAX7219_MD(unsigned char cmd,unsigned char data, unsigned char device)
*
* Description: Send data to device
* Arguments  : cmd - command for device, data - data for device, device - device number 
* Returns    : none
*********************************************************************************************************
*/
void Send_Data_MAX7219_MD(unsigned char cmd,unsigned char data, unsigned char device);
/*
*********************************************************************************************************
* Shine_LEDs_MAX7219_MD(unsigned char display, unsigned char data, unsigned char device)
*
* Description: Shine LEDs on selected matrix
* Arguments  : display - matrix display number 0-7, data - data for display, device - device number 
* Returns    : none
*********************************************************************************************************
*/
void Shine_LEDs_MAX7219_MD(unsigned char display, unsigned char data, unsigned char device);

/*
*********************************************************************************************************
* Test_MAX7219_MD(unsigned char device)
*
* Description: Test all leds on selected matrix
* Arguments  : device - device number 
* Returns    : none
*********************************************************************************************************
*/
void Test_MAX7219_MD(unsigned char device);
/*
*********************************************************************************************************
* Test_ALL_MAX7219_MD
*
* Description: Test all  matrix
* Arguments  : none 
* Returns    : none
*********************************************************************************************************
*/
void Test_ALL_MAX7219_MD();
/*
*********************************************************************************************************
* Clear_ALL_MD
*
* Description: Clear all  matrix
* Arguments  : none 
* Returns    : none
*********************************************************************************************************
*/
void Clear_ALL_MD();
/*
*********************************************************************************************************
* ClearDisplay_MAX7219_MD(unsigned char display, unsigned char device)
*
* Description: Clear selected display on selected matrix
* Arguments  : display - matrix display number 0-7, device - device number
* Returns    : none
*********************************************************************************************************
*/
void ClearDisplay_MAX7219_MD(unsigned char display, unsigned char device);
/*
*********************************************************************************************************
* Clear_Matrix_MAX7219_MD(unsigned char device)
*
* Description: Clear selected matrix
* Arguments  : device - device number
* Returns    : none
*********************************************************************************************************
*/
void Clear_Matrix_MAX7219_MD(unsigned char device);

/*
*********************************************************************************************************
* Shutdown_MAX7219_MD(unsigned char mode, unsigned char device)
*
* Description: Set shutdown mode on selected matrix
* Arguments  : mode - shutdown mode, device - device number
* Returns    : none
*********************************************************************************************************
*/
void Shutdown_MAX7219_MD(unsigned char mode, unsigned char device);

/*
*********************************************************************************************************
* Print_Matrix_MAX7219_MD(unsigned char *data, unsigned char device)
*
* Description: Print data to matrix
* Arguments  : data - data array for matrix, device - device number
* Returns    : none
*********************************************************************************************************
*/
void Print_Matrix_MAX7219_MD(unsigned char *data, unsigned char device);

#endif	/* MAX_7219_MDS_CONFIG_H */

