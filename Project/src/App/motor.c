#include "motor.h"

#define MOTOR_MAX   8000
float DirectionErr[15];
int left_duty = 0;
int right_duty = 0;
float delt_elect;
int getinto_speed = 250;
  
//float cs_p = 0.5;

int range_protect(int duty, int min, int max)//限幅保护
{
	if (duty >= max)
	{
		return max;
	}
	if (duty <= min)
	{
		return min;
	}
	else
	{
		return duty;
	}
}

/*------------------------------------------------------------------------------------------------------
【函    数】Test_Motor
【功    能】FTM0输出PWM信号控制电机例子
【参    数】num  :  电机标号  用PTC1 和 PTC2控制电机1  用PTC3 和 PTC4控制电机2
【返 回 值】motor_duty ： 占空比    范围 -FTM0_PRECISON  到 +FTM0_PRECISON
【实    例】Test_Motor(); //测试电机
【注意事项】
--------------------------------------------------------------------------------------------------------*/
void MOTOR_Ctrl(uint8_t num, short duty)
{    
    if(duty > MOTOR_MAX)     //防止占空比给错
    {
        duty = MOTOR_MAX;
    }else if(duty < -MOTOR_MAX){
        duty = -MOTOR_MAX;
    }
    
    switch(num)
    {
      case 1:
        if(duty > 0)
        {
            FTM_PwmDuty(FTM3, FTM_CH1, duty);
            FTM_PwmDuty(FTM3, FTM_CH2, 0);
        }
        else
        {
            FTM_PwmDuty(FTM3, FTM_CH1, 0);
            FTM_PwmDuty(FTM3, FTM_CH2, -duty);
        }
      case 2:
        if(duty > 0)
        {
            FTM_PwmDuty(FTM3, FTM_CH3, duty);
            FTM_PwmDuty(FTM3, FTM_CH4, 0);
        }
        else
        {
            FTM_PwmDuty(FTM3, FTM_CH3, 0);
            FTM_PwmDuty(FTM3, FTM_CH4, -duty);
        }
        
    }

}

float Pout = 0.0;
float Iout = 0.0;
float curr_err_1 = 0.0;
float curr_err_2 = 0.0;
float last_err_1 = 0.0;
float last_err_2 = 0.0;
float curr_out_1 = 0;
float curr_out_2 = 0;
float Speed_Control(int target_speed,int encoder_speed,uint8_t type)
{
    float P = 0.0;//16.8
    float I = 0.0;      //0.56
  
    P = cross_p;
    I = cross_i;

//    if(into_garage_enable){
//        P = 65;
//        I = 1.5;
//    }
    
//    if(stopflag || getinto_garage_flag){
//        P = 70;
//        I = 0.6;
//    }

    if(type == 1){
        last_err_1 = curr_err_1;
        curr_err_1 = (target_speed - encoder_speed); 
        Pout = P * (curr_err_1 - last_err_1);
        Iout = I * curr_err_1;
        curr_out_1 += (Pout + Iout);    
        return curr_out_1;
    }
    if(type == 2){
        last_err_2 = curr_err_2;
        curr_err_2 = (target_speed - encoder_speed); 
        Pout = P * (curr_err_2 - last_err_2);
        Iout = I * curr_err_2;
        curr_out_2 += (Pout + Iout);
        return curr_out_2;
    }

}

//先算出2个轮子一样的速度
void Adjust_Speed(void)
{
  if(!into_garage_enable){
    
//      if(fabs(curr_elect_val) < 0.03){
//        Speed_Set = 3;
//      }else if(fabs(curr_elect_val) < 0.35){
//        Speed_Set = 2;
//      }else{
//        Speed_Set = 1;
//      } 
      
//      if(Circle_Flag == 1){
//         Speed_Set = 4;
//      }
//      
//      if(lose_line || (out_ring_flag == 1) || Circle_Pre_flag == 1){
//        Speed_Set = 1;
//      }
//      
//      if(stopflag == 1){
//        Speed_Set = 0; 
//      }
//      
      
  }


}

//在2个轮子速度基础上进行差速变化
void CSControl(void)
{
    float cs_p =  0;
    
    
    cs_p = cs[Speed_Set];
    
    if(lose_line){
      cs_p = 0.7;
    }

    
  if(!stopflag){
        if(direction_pwm >= 0){//right
                left_duty  = (speed_duty[Speed_Set]);
                right_duty = - cs_p * (direction_pwm * 0.3 ) + (speed_duty[Speed_Set]);
               
        }else if(direction_pwm < 0){//left
                left_duty  = cs_p * (direction_pwm * 0.3) + (speed_duty[Speed_Set]);
                right_duty = (speed_duty[Speed_Set]);

        }
        
        if(getinto_garage_flag){
           if(direction_into_garage == 1){//left
                    left_duty  = 0;
                    right_duty = getinto_speed;
          }else if(direction_into_garage == 2){//right
                    left_duty  = getinto_speed;
                    right_duty = 0;
          }
        }
        
        
        if(start_car == 0 && test_roadpart == 0){
             left_duty  = 30;
             right_duty = 30;
        }
        
        if(Circle_Flag == 1 && test_circle == 1){
            left_duty = 0;
            right_duty = 0;
            last_err_2 = 0;
            last_err_1 = 0;
            curr_err_2 = 0;
            curr_err_1 = 0;
            curr_out_1 = 0;
            curr_out_2 = 0;
        }
  

  }else{
        left_duty = 0;
        right_duty = 0;
        last_err_2 = 0;
        last_err_1 = 0;
        curr_err_2 = 0;
        curr_err_1 = 0;
        curr_out_1 = 0;
        curr_out_2 = 0;
  }
  
}

void Motor_Out(){
  
  

    if(left_duty <= 0){
        left_duty = 0;
    }
    
    if(right_duty <= 0){
        right_duty = 0;
    }
    
    left_duty = Speed_Control(left_duty, L_encoder,1);
    right_duty  = Speed_Control(right_duty, R_encoder,2);

    MOTOR_Ctrl(1,left_duty);           //修改电机占空比
    MOTOR_Ctrl(2,right_duty);

    if(out_death){
        MOTOR_Ctrl(1,4000);           //修改电机占空比
        MOTOR_Ctrl(2,4000);
    }
    
}
