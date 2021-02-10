#ifndef _IMAGERECOGNITION_H
#define _IMAGERECOGNITION_H

#include "include.h"

extern uint16_t ccd_buf[128];
extern uint8_t into_garage_enable;
extern uint8_t goto_garage_cnt;
extern uint8_t getinto_garage_flag;

void GetImageArray();
void OutGarage();

//#define IMAGEH  MT9V034_IMAGEH
//#define IMAGEW  MT9V034_IMAGEW
//#define ZIPH   60
//#define ZIPW   12
//#define LCDH   60
//#define LCDW   94
//
//extern short gyro_z_add;
//
//extern uint8_t pixle[LCDH][LCDW];
//extern uint8_t Image_Use[LCDH][LCDW];
//extern uint8_t zip_pixle[ZIPH][ZIPW];
//extern float  BlackThres;   //黑白阈值
//
//void CameraInit(void);
//void UARTSendPicture(void);
//void GetImageArray();
//void GetThreshold(void);
//void Get_01_Value(u8 image[LCDH][LCDW]);
//void sobel(uint8_t imag[LCDH][LCDW],uint8_t imag1[LCDH][LCDW]);
//void StartLineCheck();
//void GarageCheck();

#endif


