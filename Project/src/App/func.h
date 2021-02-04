#ifndef _FUNC_H
#define _FUNC_H

#include "include.h"

#define     float_conversion_uint32(float_data)     (*(uint32 *)&float_data)

#define     uint32_conversion_float(uint32_data)    (*(float *)&uint32_data)

extern uint8_t start_car;
extern unsigned char KeySta[4][4];
extern int dir_pwm;
extern int max_normal_elect;
extern float mpu_gyro_z_offset;
extern float mpu_gyro_y_offset;
extern float cross_d;
extern float cross_i;
extern float cross_p;
extern float mpu_gyro_x_offset;
extern int start_line_thres;
extern uint8_t out_death;
extern int temp;
extern u8 start_car;

void ShowMenu(void);
void Check_BottonPress();
void Push_And_Pull(float *buff,int len,float newdata);
void Int_Push_And_Pull(int *buff,int len,int newdata);
float Slope_Calculate(uint8 begin,uint8 end,float *p);
void LED();
void SendImageData(void) ;
unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT);
void OutPut_Data();
void sendufuzzymain();
void ParaAdjust();
void KeyScan();
void Read_Flash();

#endif
