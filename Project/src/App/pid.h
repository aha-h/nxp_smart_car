//#ifndef __PID_H
//#define __PID_H
//
//#include "include.h"
//
//#define P 0
//#define I 1
//#define D_ 2
//#define T 3
//
//
//typedef struct PID
//{
//	long SumError;	//误差累计
//	int32 LastError;	//Error[-1]
//	int32 PrevError;	//Error[-2]
//	int32 LastSpeed;	//Speed[-1]
//	float KP;                              //比例常数 Proportional Const
//	float KI;                                //积分常数 Integral Const
//	float KD;                              //微分常数 Derivative Const
//} PID;
//
//extern float   Left_MOTOR[3];	//电机PID
//extern float   Right_MOTOR[3];	//电机PID
//
//extern PID S3010_PID, Left_MOTOR_PID, Right_MOTOR_PID, CS_PID;	//定义舵机和电机的PID参数结构体
//
////位置式PID参数初始化
//void PlacePID_Init(PID* sptr);
//int32 PlacePID_CS(PID* sprt, int32 NowPiont, int32 SetPoint);
//int32 PlacePID_CS2(PID* sprt, int32 NowPiont, int32 SetPoint);
//int32 PlacePID_Control3(PID* sprt, int32 NowPiont, int32 SetPoint);
//int32 PlacePID_Control4(PID* sprt, int32 NowPiont, int32 SetPoint);
////增量式PID参数初始化
//void IncPID_Init(PID* sptr, float* MOTOR_PID);
//
////位置式PID舵机控制
//int32 PlacePID_Control(PID* sprt, int32 NowPiont, int32 SetPoint);
//int32 PlacePID_Control2(PID* sprt, int32 NowPiont, int32 SetPoint);
//
//int32 PID_Realize(PID* sptr, int32 ActualSpeed, int32 SetSpeed);
//
//#endif