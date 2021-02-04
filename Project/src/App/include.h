#ifndef INCLUDE_H_
#define INCLUDE_H_

/*
 * 包含Cortex-M内核的通用头文件
 */
#include    <stdio.h>                       //printf
#include    <string.h>                      //memcpy
#include    <stdlib.h>                      //malloc
#include    "math.h"
#include    "common.h" 
#include    "MK66F18.h"   /* 寄存器映像头文件 */
#include    "core_cm4.h"         /* 内核文件用于设置中断优先级 */

/*----------------------------------------------------------------
                   底层驱动文件
------------------------------------------------------------------*/
#include "MK60_ADC.h"              /* 用于ADC模块 */
#include "MK60_GPIO.h"             /* 用于GPIO模块 */
#include "MK60_GPIO_Cfg.h"         /* 用于GPIO模块 类51的IO操作 */
#include "MK60_PLL.h"              /* 用于时钟频率设置 */
#include "MK60_UART.h"             /* 用于串口模块 */
#include "MK60_PIT.h"              /* 用于PIT定时器模块 */
#include "MK60_FTM.h"              /* 用于FTM定时器模块 */
#include "MK60_CMT.h"              /* 用于CMT定时器某块 */
#include "MK60_IIC.h"              /* 用于IIC模块 */
#include "MK60_DMA.h"              /* 用于DMA模块 */
#include "MK60_LPTMR.h"            /* 用于LPTMR定时器模块 */
#include "MK60_WDOG.h"             /* 用于看门狗 */
#include "MK60_SYSTICK.h"          /* systick 内核定时器 */
#include "MK60_FLASH.h"            /* Flash 读写 */
#include "MK60_SPI.h"
/*----------------------------------------------------------------
                  用户函数
------------------------------------------------------------------*/
//#include "pid.h"
#include "init.h"
#include "12864.h"
#include "SGP18T.h"
#include "MT9V034.h"
#include "constant.h"
#include "steer.h"
#include "motor.h"
#include "user_common.h"
#include "fuzzy_steer.h"
#include "ImageRecognition.h"
#include "electromagnetics.h"
#include "VL53.h"
#include "ICM20602.h"
#include "ccd.h"
#include "func.h"
//#include "ANO_DT.h"
//#include "DataScope_DP.h"
//#include "fsl_edma.h"
//#include "Uart_DMA.h"
  
/* 在OLED 上显示或者在TFT1.8显示 */
#define LQ_OLED     
//#define LQ_TFT1_8  
//#define GRAY
//常量类型的定义
#define uint8_t  u8
#define uint16_t u16
#define uint32_t u32

#define Pi        3.14159265358979f            //圆周率

//#define uint64_t u64


extern u8 s_flag;

struct PID                          //结构体定义不能初始化变量，以为结构体类似于一个共有得类型，不能搞特殊化“初始化”
{
  float P;
  float I;
  float D;
  float Pout;
  float Iout;
  float Dout;
  float Out;
};

extern int L_encoder;
extern int R_encoder;
extern float  car_encoder_speed;           //小车编码器速度
extern float car_speed;                  //小车实际速度
extern float Target_Turn_Angle_Speed;     //目标转向角速度
extern float Rel_Turn_Angle_Speed;        //实际转向角速度
extern int target_encoder_speed;
extern int Speed_New_Out;
extern float Speed_Out;
extern int  Speed_Control_Count;
extern int  Turn_Control_Count;
extern int  Angle_Speed_Control_Count;
extern int  Turn_Angle_Speed_Control_Count;
extern float curr_elect_val;
extern int stopflag;
extern float elect_L;
extern float elect_R;
extern float elect_M;
extern float elect_L_offset;
extern float elect_R_offset;
extern float elect_val_delta;
extern int Target_Turn_Angle;
extern float Turn_Ang_Speed_bias;
extern int Speed_New_Out;
extern int Speed_Old_Out;
extern uint8_t Speed_Set;
extern float up_angle_acce;
extern float up_angle;
extern uint8 Ramp_flag;
extern short aacx,aacy,aacz;	        //加速度传感器原始数据
extern short gyrox,gyroy,gyroz;        //陀螺仪原始数据
extern short gyro_temp;             //陀螺仪温度
extern u8 ch1;
extern u8 ch2;
extern u8 ch3;
extern u8 ch4;
extern u8 ch5;
extern u8 ch6;
extern u8 ch7;
extern u8 ch8;
extern u8 start_car;
extern int Left_MOTOR_Speed;           //电机速度
extern int Right_MOTOR_Speed;
extern uint8_t BM_1;
extern uint8_t BM_2;
extern uint8_t BM_3;
extern uint8_t BM_4;
extern uint8_t BM_NUM;
extern uint16_t dis;
extern int duty_motor;
extern int receive_data;
extern float In_Out_Roundabout_I;
extern int duty_motor;
extern uint8_t normal_track_line_in_ring;
extern uint8_t Speed_Set;
extern int speed_duty[10];
//extern float cs_p[10];
extern float cs_d[10];
extern int Left_Motor_Duty;
extern int Right_Motor_Duty;
extern int Target_Left_Motor_Duty;
extern int Target_Right_Motor_Duty;
extern uint8_t Lose_Line_Flag;
extern short gyrosensor;
extern int encoder_cnt;
extern int encoder[8];
extern int speed_max[10];
extern int speed_min[10];
extern float cs_d[10];
//extern float cs_p[10];
extern int speed_encoder[10];
extern float cross_p ;
extern float cross_d ;
extern int s_cnt ;
extern int cross_remain;
extern int out_ring_cnt;
extern int out_ring_flag;
extern short accsenser;
extern int out_garbage;
extern int gryo_circle;
extern  float  up_elec_l ;
extern  float  up_elec_r ;
extern int gyro_garage;
extern int garage_cnt;
extern int into_garage_flag;
extern uint8_t ramp_flag;
extern uint8_t test_startline;
extern uint8_t test_roadpart;
extern uint8_t test_circle ;

#endif
