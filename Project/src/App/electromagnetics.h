#ifndef __ELECTRO_H
#define __ELECTRO_H

#include "include.h"

#define S3010_RIGHT   7560      //7460
#define S3010_MID     6660     //6620
#define S3010_LEFT    5760     //5780


extern float error;
extern uint8_t lose_line;
extern float angle_save[50];
extern uint8_t lose_line_dir;
extern int AD_max[6];
extern int AD_min[6];       //电感极值 
extern int direction_pwm;
extern float rel_d;
extern int lose_line_t;
extern short  gyro_z_add;
extern uint8_t dir_pwm_enable ; 
extern uint8_t s_s_flag ;
extern uint8_t S_flag ;
extern uint8_t straight_flag ;
extern uint8_t max_curve_flag ;
extern uint8_t ramp_flag ;
extern int time_stamp;
extern float elect_Up_L ;
extern float elect_Up_R ;
extern int AD_data[6];
extern int rule[7][7];
extern int fuzzy_txt[10];
extern float up_curr_elect_val;           //电磁偏差,差和比运算的结果
extern float up_elect_val_delta; 
/*****环岛相关变量***/
extern uint8_t Go_Out_Circle;   //环岛状态区分
extern int Circle_Flag_Turn;    //入环标志位
extern uint8_t Circle_Flag;       //环岛标志位
extern uint8_t LeftCircle_Flag;   //左环
extern uint8_t RightCircle_Flag;  //右环
extern uint8_t Circle_Pre_flag;   //预判标志位
extern float angle_sum;
extern uint8_t crosstype ;
extern int cross_remain;
extern uint8_t cross_out;
extern int OutRing ;
extern int Circle_elect_thres;
extern int circle_up_thres;
extern int risingadd;
extern int losingadd;
extern float upl_elect[3],upr_elect[3];
extern float leftDecline_amplitude,leftRising_amplitude,leftSensorMax;
extern float rightDecline_amplitude,rightRising_amplitude,rightSensorMax;
extern float first_leftDecline_amplitude,first_rightDecline_amplitude;
extern uint8_t left_down_flag,right_down_flag;
extern uint8_t second_left_down_flag,second_right_down_flag;
extern int lose_in_circle;
extern int up_thres;
extern int in_circle_thres ;
extern int goto_circle_gyro;
extern int getinto_circle_thres;
extern int dir_pwm_max ;
extern int dir_pwm_min  ;
/*****环岛相关变量***/
/********变速相关变量********/
extern float speed_seg ;
/********变速相关变量********/
extern int Lose_Line_Thres ;
extern int REFIND_LINE ;
extern float cs[10];
/********丢线相关变量********/

/********丢线相关变量********/
/********舵机打角相关变量********/
extern float gyroy_d;
extern int kd_elc;
extern int l_r_sum;
/********舵机打角相关变量********/
extern float elec_k;
extern float elec_k_r;


void Steer_Scan(void);
void Foresight_Data( int * AD_data);
void Get_Speed();                                               //获得小车速度
void Set_Car_Speed(float rel_speed);                            //设置小车速度         单位：cm/s  
void Smooth_Speed();
void NC_Turn_Control();                                         //单神经元PD--第二次转向PID
float Get_Target_Turn_Angle_Speed(float curv_R);                //获取目标转向角速度
void Get_Rel_Turn_Angle_Speed();                                //获取实际转向角速度
void Turn_Control();                                            //转向控制
void Get_Elect_Data();                                          //读取电磁
void Elect_Control_Out();                                       //电磁控制输出
void Prective_Elect_Control_Out();
void Bell();                                                    //蜂鸣器
void Fuzzy_Dir_Adjust(float e, float ec);                     //方向环自适应模糊调整参数
void Stop_Car();                                                //停车控制
void LED();                                                     //LED灯
void OLED_Display_Elect();                                      //显示电磁值
void OLED_Display_Debug();
void Get_Car_Angle();                                           //获取车身角度
void KalmanFilter_Up_Angle();
float KalmanFilter_Elect(float curr_elect_val,float last_elect_val);
void Direction();

#endif