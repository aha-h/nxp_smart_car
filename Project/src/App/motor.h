#ifndef __MOTOR_H
#define __MOTOR_H

#include "include.h"

extern int16 motor_duty;
extern uint8 BangBang_Flag;
extern float NC_Speed_New_Out;
extern float Error_Delta;
extern float cs_p;
extern float delt_elect;
extern int getinto_speed;

void PWM_Out();
int range_protect(int duty, int min, int max);
void Motor_Out();
void Adjust_Speed(void);
void Control_Speed(void);
void CSControl();
int NC_Speed_Control(int target_speed,int car_speed);

#endif






























//#include "user_common.h"

////定义电机端口
//#define MOTOR_FTM   FTM0
//#define Left_MOTOR1_PWM  FTM_CH0
//#define Left_MOTOR2_PWM  FTM_CH1
//#define Right_MOTOR1_PWM FTM_CH2
//#define Right_MOTOR2_PWM FTM_CH3
//#define MOTOR_HZ    15000                     //C车电机频率建议在17k左右
//
//////***************************//
//extern int32   Left_MOTOR_Speed;         //FTM1  正值
//extern int32   Right_MOTOR_Speed;         //FTM2 负值
//extern int32   Pulse_count;
//
//extern int16 Max_Speed;
//extern int16 Min_Speed;
//extern int16 High_Speed;
//extern int16 Left_High_Speed,Right_High_Speed;
///*************************   赛道检测变量    ******************************************/
//extern int16  Fictitious_Beeline_Num;
//extern uint8  Track_Line_Num,Track_Line_Time;
///*************************   其他控制变量    ******************************************/
//extern char Set,Speed_Set;
//extern uint8 Run_Flag;
//int16 Fictitious_Beeline(void);
//
///*******************挂挡变量声明**************/
//extern float Set_CS_P_Right[2];
//extern float Set_CS_P_Left[2];
//extern char Set_Set[2];
//
//
//void Start_Control(void);
//void Control_Speed(void);
//void Adjust_Speed(void);
//void Track_Line(void);
//void speed_measure(void);
//void MOTOR_Control(void);

