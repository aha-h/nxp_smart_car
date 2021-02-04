//#include "pid.h"
//
//
//PID S3010_PID, CS_PID, Left_MOTOR_PID, Right_MOTOR_PID;	//定义舵机和电机的PID参数结构体
//////////普通元素////////
//int32 S3010[8][4] = { {  0,  0,  0,  0},
///*Mode_Set = 1(200)*/ { 12,  0,  9, 20},     //13  9
///*Mode_Set = 2(220)*/ { 14,  0,  12, 20},   // 12   10
///*Mode_Set = 3(240)*/ { 15,  0,  13, 20},
///*Mode_Set = 4(350)*/ { 11,  0,   6,  0},
///*Mode_Set = 5(400)*/ { 18,  0,  10, 20},
///*Mode_Set = 6(420)*/ { 20,  0,  10, 20},
///*Mode_Set = 7(450)*/ { 13,  0,  23, 20} };
//
//int32 CS[8][4] = { {  0,  0,  0,  0},
///*Mode_Set = 1(200)*/ { 12,  0,  9, 20},      //13 9
///*Mode_Set = 2(220)*/ { 14,  0,  12, 20},
///*Mode_Set = 3(240)*/ { 15,  0,  13, 20},
///*Mode_Set = 4(350)*/ { 11,  0,   6,  0},
///*Mode_Set = 5(400)*/ { 18,  0,  10, 20},
///*Mode_Set = 6(420)*/ { 20,  0,  10, 20},
///*Mode_Set = 7(450)*/ { 13,  0,  23, 20} };
//////////环岛专属////////
//int32 S3010_Ring[8][4] = { {  0,  0,  0,  0},
///*Mode_Set = 1(200)*/ { 6,  0,   5, 20},
///*Mode_Set = 2(220)*/ { 7,  0,  5, 20},                 //10   0  8   20      此PID参数对应第一次使用的加权参数
///*Mode_Set = 3(240)*/ { 8,  0,  6, 20},
///*Mode_Set = 4(350)*/ { 11,  0,   6,  0},
///*Mode_Set = 5(400)*/ { 18,  0,  10, 20},
///*Mode_Set = 6(420)*/ { 20,  0,  10, 20},
///*Mode_Set = 7(450)*/ { 13,  0,  23, 20} };
//int32 CS_Ring[8][4] = { {  0,  0,  0,  0},
///*Mode_Set = 1(200)*/ { 6,  0,   5, 20},
///*Mode_Set = 2(220)*/ { 7,  0,  5, 20},
///*Mode_Set = 3(240)*/ { 8,  0,  6, 20},
///*Mode_Set = 4(350)*/ { 11,  0,   6,  0},
///*Mode_Set = 5(400)*/ { 18,  0,  10, 20},
///*Mode_Set = 6(420)*/ { 20,  0,  10, 20},
///*Mode_Set = 7(450)*/ { 13,  0,  23, 20} };
///* PID 的 I 项没有使用，此处 I 仅作为速度模式的标号，对PID无影响 */
//int32 S3010_Electromagnetic[8][4] = { {  0,  0,  0,  0},
///*Mode_Set = 1(200)*/ { 12,  0,  6, 20},     //1,4
///*Mode_Set = 2(220)*/ { 12,  0,  6, 20},
///*Mode_Set = 3(240)*/ { 11,  0,  6, 20},
///*Mode_Set = 4(350)*/ { 11,  0,   6,  0},
///*Mode_Set = 5(400)*/ { 18,  0,  10, 20},
///*Mode_Set = 6(420)*/ { 20,  0,  10, 20},
///*Mode_Set = 7(450)*/ { 13,  0,  23, 20} };
//
//
//float   Left_MOTOR[3] = { 7.8, 2.96, 3.6 };	//电机PID
//float   Right_MOTOR[3] = { 7.8, 2.96, 3.6 };	//电机PID
////位置式PID参数初始化
//void PlacePID_Init(PID* sptr)
//{
//    sptr->SumError = 0;
//    sptr->LastError = 0;	//Error[-1]
//    sptr->PrevError = 0;	//Error[-2]
//}
////增量式PID参数初始化
//void IncPID_Init(PID* sptr, float* MOTOR_PID)
//{
//    sptr->SumError = 0;
//    sptr->LastError = 0;	//Error[-1]
//    sptr->PrevError = 0;	//Error[-2]
//    sptr->LastSpeed = 0;
//    sptr->KP = *MOTOR_PID;
//    sptr->KI = *(MOTOR_PID + 1);
//    sptr->KD = *(MOTOR_PID + 2);
//
//}
////位置式PID舵机控制
//int32 PlacePID_Control(PID* sprt, int32 NowPiont, int32 SetPoint)
//{
//    //定义为寄存器变量，只能用于整型和字符型变量，提高运算速度
//    int32 iError, Delat_iError,	//当前误差
//            Actual;	//最后得出的实际输出值
//    float	Kp, Kd = 0;		//动态P
//
//
//    iError = SetPoint - NowPiont;	//计算当前误差
//    Delat_iError = iError - sprt->LastError;
//    Kp = Fuzzy(iError, Delat_iError);
//    Kd = Fuzzy_D(iError, Delat_iError);
//    if (island_flag == 0)
//            Actual = (S3010[Speed_Set][P] + Kp) * iError + (S3010[Speed_Set][D_] + Kd) * (iError - sprt->LastError);//只用PD
//    else
//            Actual = (S3010_Ring[Speed_Set][P] + Kp) * iError + (S3010_Ring[Speed_Set][D_] + Kd) * (iError - sprt->LastError);//只用PD
//
//    sprt->LastError = iError;		//更新上次误差
//
//
//    return (S3010_MID - Actual);
//}
//
////位置式PID舵机控制    右转弯时的PID
//int32 PlacePID_Control3(PID* sprt, int32 NowPiont, int32 SetPoint)
//{
//    //定义为寄存器变量，只能用于整型和字符型变量，提高运算速度
//    int32 iError, Delat_iError,	//当前误差
//            Actual;	//最后得出的实际输出值
//    float	Kp, Kd = 0;		//动态P
//
//
//    iError = SetPoint - NowPiont;	//计算当前误差
//    Delat_iError = iError - sprt->LastError;
//    Kp = Fuzzy(iError, Delat_iError);
//    Kd = Fuzzy_D(iError, Delat_iError);
//    if (island_flag == 0)
//            Actual = (S3010_Electromagnetic[Speed_Set][P] + Kp) * iError + (S3010_Electromagnetic[Speed_Set][D_] + Kd) * (iError - sprt->LastError);//只用PD
//
//    sprt->LastError = iError;		//更新上次误差
//
//
//    return S3010_MID + Actual;
//}
//
//#define Qp      1
//#define Qd      1
//
////位置式差速控制
//int32 PlacePID_CS(PID* sprt, int32 NowPiont, int32 SetPoint)
//{
//    //定义为寄存器变量，只能用于整型和字符型变量，提高运算速度
//    int32 iError, Delat_iError,	//当前误差
//            Actual;	//最后得出的实际输出值
//    float	Kp, Kd = 0;		//动态P
//
//
//    iError = SetPoint - NowPiont;	//计算当前误差
//    Delat_iError = iError - sprt->LastError;
//    Kp = Fuzzy(iError, Delat_iError);
//    if (island_flag == 0)
//            Kd = Fuzzy_D(iError, Delat_iError);
//    if (island_flag == 0)
//            Actual = (CS[Speed_Set][P] + Kp) * iError + (CS[Speed_Set][D_] + Kd) * (iError - sprt->LastError);//只用PD
//    else
//            Actual = (CS_Ring[Speed_Set][P] + Kp) * iError + (CS_Ring[Speed_Set][D_] + Kd) * (iError - sprt->LastError);//只用PD
//
//    sprt->LastError = iError;		//更新上次误差
//
//
//    return S3010_MID - Actual;
//}
//
////增量式PID电机控制    速度控制
//uint8 BangBang_Flag = 0;
//int32 PID_Realize(PID* sptr, int32 ActualSpeed, int32 SetSpeed)
//{
//    //当前误差，定义为寄存器变量，只能用于整型和字符型变量，提高运算速度
//
//    int32 iError,	    //当前误差
//            Increase;	//最后得出的实际增量
//
//    iError = SetSpeed - ActualSpeed;//计算当前误差
//
//    Increase = sptr->KP * (iError - sptr->LastError)
//            + sptr->KI * iError
//            + sptr->KD * (iError - 2 * sptr->LastError + sptr->PrevError);
//    sptr->PrevError = sptr->LastError;	//更新前次误差
//    sptr->LastError = iError;		  	//更新上次误差
//
//    if (iError < -50)
//    {
//            BangBang_Flag = 1;
//    }
//    else if (iError < -50 && BangBang_Flag)
//    {
//            BangBang_Flag = 0;
//            Increase -= 400;
//    }
//    if (iError > 100)
//    {
//            Increase += 800;
//    }
//    else if (iError > 50)
//    {
//            Increase += 400;
//    }
//    else if (iError < -40)
//    {
//            BangBang_Flag = 1;
//            Increase -= 200;
//    }
//
//
//    if (iError > 100)
//    {
//            Increase += 600;
//    }
//    else if ((iError > 40) && (BangBang_Flag == 1))
//    {
//            BangBang_Flag = 0;
//            Increase += 400;
//    }
//    else if (iError > 50)
//    {
//            Increase += 300;
//            BangBang_Flag = 1;
//    }
//
//    return Increase;
//}
//
