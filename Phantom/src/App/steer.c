#include "steer.h"










































///****************************    舵机控制变量    ****************************/
//int32    S3010_Duty = 0;      //       //4320  4740 5180
//
//int8  Last_Error[5],Point_Error;
//uint8 Point;
//
//uint16 Time_Left=290;
//uint16 Time_Right=420;
//
//int8     Point_Num=2;  //提前系数
///****************************    差速控制变量    ****************************/
//int16    angle;//阿克曼转向角
//int32    Angle;//舵机目标角度
//uint8    CS_Left=11,CS_Right=11;
//uint8    CS_Point;
//int32    CS_Duty;      //       //4320  4740 5180
//float   Left_Speed = 0;
//float   Right_Speed = 0;
//float   Temp_Orr;
//float   Temp_Add;
//
//int8     CS_Point_Num=0;
//float   CS_P_Right = 0.50,CS_P_Left = 0.50;//差速系数
//
//
//void S3010_Control(void)
//{
//
//    if(TurnRight_Flag == 1)
//    {
//        Electro_Error=-33.0;
//        TurnRight_Flag=0;
//    }
//    else if(TurnLeft_Flag == 1)
//    {
//        Electro_Error=33.0;
//        TurnLeft_Flag=0;
//    }
//    S3010_Duty=PlacePID_Control3(&S3010_PID,0,Electro_Error);
//    S3010_Duty = range_protect(S3010_Duty, S3010_RIGHT, S3010_LEFT);          //舵机占空比限幅保护
//    FTM_PwmDuty(S3010_FTM, S3010_CH, S3010_Duty);			//修改舵机占空比
//
//}
//
//
//#define ANG_NUM    45
//void CS_Control(void)
//{
//        CS_Point = Motor_Point_Weight(CS_Point_Num);		// 正值 图像向下平移 提前转向	必须为偶数
//
////        CS_Duty = PlacePID_CS(&CS_PID,Point_Mid,CS_Point);	//使用位置式PID解算
//
//        CS_Duty = range_protect(CS_Duty, S3010_RIGHT, S3010_LEFT);          //舵机占空比限幅保护
//        Angle = S3010_MID - CS_Duty; //目标角度
//        /****************************************************************************/
//        /****************************    舵机角度差速    ****************************/
//        /****************************************************************************/
//
//        if(Angle>0)         //右拐
//        {
//           angle=ANG_NUM*(Angle)*1.0/(S3010_LEFT - S3010_MID) ;  //angle：阿克曼转向角    angle_max   45
//           if(angle>ANG_NUM)  angle=ANG_NUM;
//           if(angle<0)   angle=0;
//           Right_Speed = High_Speed - (High_Speed*angle*CS_P_Right)/ANG_NUM;
//           Left_Speed = High_Speed;
//           Left_High_Speed = Left_Speed;
//           Right_High_Speed = Right_Speed;
//        }
//        else              //左拐
//        {
//           angle=ANG_NUM*(-Angle)*1.0/(S3010_MID - S3010_RIGHT) ; //angle：阿克曼转向角    angle_max   45
//           if(angle>ANG_NUM)  angle=ANG_NUM;
//           if(angle<0)   angle=0;
//           Right_Speed = High_Speed;
//           Left_Speed = High_Speed - (High_Speed*angle*CS_P_Left)/ANG_NUM;
//           Left_High_Speed = Left_Speed;
//           Right_High_Speed = Right_Speed;
//
//        }
//
//}


/******** 限幅保护 *********/

  

