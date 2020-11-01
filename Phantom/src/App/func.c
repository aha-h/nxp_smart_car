#include "func.h"

u16 battery = 0;
u16 temperature  = 0;
u16 dis = 0;
int OutData[4] = {0}; 
uint8_t  Speed_Set=1;//速度档位选择
extern float Turn_R;

/***********************************/
/****************PID****************/
int  L_encoder = 0;               //小车速度
int  R_encoder = 0;
float  car_encoder_speed;           //小车编码器速度
float  car_speed = 0;            //小车的实际速度
int  Speed_Control_Count;
int  Turn_Control_Count;
int  Angle_Speed_Control_Count;
int  Turn_Angle_Speed_Control_Count;
float Speed_Out;
int  target_encoder_speed = 0;
int stopflag = 1;//1表示停车，0表示前进
int Speed_New_Out;
int Speed_Old_Out;
float mpu_gyro_z_offset;
float mpu_gyro_x_offset;
float mpu_gyro_y_offset;
/***********************************/
u8 start_car  = 0;
u8 data_adjust_flag = 0;
int duty_motor;
uint8_t showflag = 0;
uint8_t sendflag = 3;
uint8_t s_flag = 0;
uint8_t send_fuzzy_flag = 0;
float In_Out_Roundabout_I = 0;
uint8_t normal_track_line_in_ring = 0;
uint8_t OLED_Refresh = 0;
/***********************参数数组的定义************************/
int  speed_duty[10]  =  {0,210,230,240,250,260,270,280,290,300};//只用面前几个速度  350  400
//int speed_encoder[10] = {214,275,334,389,445,504,564,644,698,755 };//不同占空比对应的编码器数值
//float cs_p[10] = {0,0,0,0,0,0,0,0,0,0};//差速的p
float cs[10] = {0,0.5,0.63,0.55,0.68,0.65,0.69,0.80,0.8,0.8};//差速
int speed_min[10] = { 0,15,50,70,0,0,0,0,0,0 };//差速的最小值
int speed_max[10] = { 0,20,60,90,0,0,0,0 };//差速的最大值
uint8_t Lose_Line_Flag = 0;
int encoder_cnt = 0;
float cross_p = 50;
float cross_i = 1.0;
float cross_d = 0;
/**********起跑线相关变量********/
//uint8_t start_car = 0;
int start_line_thres = 300;
uint8_t left_right_parameter = 0;
/**********起跑线相关变量********/
/**********按键扫描相关变量********/
unsigned char KeySta[4][4] = {  
    {1, 1, 1, 1},  {1, 1, 1, 1},  {1, 1, 1, 1},  {1, 1, 1, 1}
};

unsigned char backup[4][4] = {  
    {1, 1, 1, 1},  {1, 1, 1, 1},  {1, 1, 1, 1},  {1, 1, 1, 1}
};

uint8_t press_key = 0;
uint8_t last_send = 0;
short plus_minus = 1;
uint8_t last_show = 1;
int dir_pwm = 0;
int temp;
/**********按键扫描相关变量********/
/**********flash相关变量********/

uint32 write_buf[60];
uint32 read_buf[60];

float write_float[60];
float read_float[60];

/**********flash相关变量********/
/**********赛道信息采集相关变量********/
int max_normal_elect = 0;
uint8_t out_death = 0;
uint8_t read_flash_type = 1;

/**********赛道信息采集相关变量********/

void WriteFlash()
{
    FLASH_EraseSector(1);
    write_buf[0] = AD_max[0];
    write_buf[1] = AD_max[1];
    write_buf[2] = AD_max[2];
    write_buf[3] = AD_max[3];
    write_buf[4] = AD_max[4];
    write_buf[5] = last_send;
    write_buf[6] = REFIND_LINE;
    write_buf[7]  = sendflag;
    write_buf[8]  = Lose_Line_Thres;
    write_buf[9]  = Circle_elect_thres;
    write_buf[10] = goto_circle_gyro;
    write_buf[11] = lose_in_circle;
    write_buf[12] = losingadd ;
    write_buf[13] = gyro_garage;
    write_buf[14] = start_line_thres ;
    write_buf[15] = in_circle_thres;
    write_buf[16]  = gryo_circle;
    write_buf[17] = speed_duty[1];
    write_buf[18] = speed_duty[2];
    write_buf[19] = speed_duty[3];
    write_buf[20]  = speed_duty[4];
    write_buf[21]  = speed_duty[5];
    write_buf[22]  = speed_duty[6];
    write_buf[23] = risingadd;
    write_buf[24] = AD_min[0];
    write_buf[25] = AD_min[1];
    write_buf[26] = AD_min[2];
    write_buf[27] = AD_min[3];
    write_buf[28] = AD_min[4];
    write_buf[29] = up_thres;
    write_buf[30] = garage_cnt;
    write_buf[31] = getinto_circle_thres;
    write_buf[32] = Speed_Set;
    write_buf[33] = dir_pwm_max;
    write_buf[34] = dir_pwm_min;
    write_buf[35] = speed_duty[7];
    write_buf[36] = speed_duty[8];
    write_buf[37] = speed_duty[9];
    write_buf[38] = getinto_speed;
    write_buf[39] = test_startline;
    write_buf[40] = test_roadpart;
    write_buf[41] = test_circle;
    write_buf[42] = direction_into_garage;
      
    FLASH_WriteBuf(1, (uint8_t *)write_buf, sizeof(write_buf), 0);
    
    FLASH_EraseSector(2);
//    write_float[0] = cs_p;
    write_float[1] = gyroy_d;
//    write_float[2] = cross_p;
//    write_float[3] = cross_i;
//    write_float[4] = cross_d;
    write_float[5] = cs[1];
    write_float[6] = cs[2];
    write_float[7] = cs[3];
    write_float[8] = cs[4];
    write_float[9] = cs[5];
    write_float[10] = cs[6];
    write_float[11] = cs[7];
    write_float[12] = cs[8];
    write_float[13] = cs[9];
    
    
    FLASH_WriteBuf(2, (uint8_t *)write_float, sizeof(write_float), 0);
    
}

void Read_Flash()
{
    FLASH_ReadBuff(1, 0, sizeof(write_buf), (char *)read_buf);
    if(read_flash_type == 0){
        AD_max[0] = read_buf[0];
        AD_max[1] = read_buf[1];
        AD_max[2] = read_buf[2];
        AD_max[3] = read_buf[3];
        AD_max[4] = read_buf[4];
        last_send = read_buf[5];
        REFIND_LINE = read_buf[6];
        sendflag = read_buf[7];
        Lose_Line_Thres = read_buf[8];
        Circle_elect_thres = read_buf[9];
        goto_circle_gyro = read_buf[10];
        lose_in_circle = read_buf[11];
        losingadd  = read_buf[12];
        gyro_garage = read_buf[13];
        start_line_thres =  read_buf[14];
        in_circle_thres = read_buf[15];
        gryo_circle = read_buf[16];
        speed_duty[1] = read_buf[17];
        speed_duty[2] = read_buf[18];
        speed_duty[3] = read_buf[19];
        speed_duty[4] = read_buf[20];
        speed_duty[5] = read_buf[21];
        speed_duty[6]  = read_buf[22];
        risingadd = read_buf[23];
        AD_min[0] = read_buf[24];
        AD_min[1] = read_buf[25];
        AD_min[2] = read_buf[26];
        AD_min[3] = read_buf[27];
        AD_min[4] = read_buf[28];
        up_thres = read_buf[29];
        garage_cnt = read_buf[30];
        getinto_circle_thres = read_buf[31];
        Speed_Set = read_buf[32];
        dir_pwm_max = read_buf[33];
        dir_pwm_min = read_buf[34];
        speed_duty[7] = read_buf[35];
        speed_duty[8] = read_buf[36];
        speed_duty[9] = read_buf[37];
        getinto_speed = read_buf[38];
        test_startline = read_buf[39];
        test_roadpart = read_buf[40];
        test_circle = read_buf[41];
        direction_into_garage = read_buf[42];
    }else if(read_flash_type == 1){
        last_send = read_buf[5];
        REFIND_LINE = read_buf[6];
        sendflag = read_buf[7];
        Lose_Line_Thres = read_buf[8];
        Circle_elect_thres = read_buf[9];
        goto_circle_gyro = read_buf[10];
        lose_in_circle = read_buf[11];
        losingadd  = read_buf[12];
        gyro_garage = read_buf[13];
        start_line_thres =  read_buf[14];
        in_circle_thres = read_buf[15];
        gryo_circle = read_buf[16];
        speed_duty[1] = read_buf[17];
        speed_duty[2] = read_buf[18];
        speed_duty[3] = read_buf[19];
        speed_duty[4] = read_buf[20];
        speed_duty[5] = read_buf[21];
        speed_duty[6]  = read_buf[22];
        risingadd = read_buf[23];
        up_thres = read_buf[29];
        garage_cnt = read_buf[30];
        getinto_circle_thres = read_buf[31];
        Speed_Set = read_buf[32];
        dir_pwm_max = read_buf[33];
        dir_pwm_min = read_buf[34];
        speed_duty[7] = read_buf[35];
        speed_duty[8] = read_buf[36];
        speed_duty[9] = read_buf[37];
        getinto_speed = read_buf[38];
        test_startline = read_buf[39];
        test_roadpart = read_buf[40];
        test_circle = read_buf[41];
        direction_into_garage = read_buf[42];
    }
      
    FLASH_ReadBuff(2, 0, sizeof(write_float), (char *)read_float);
//    cs_p = read_float[0];
    gyroy_d = read_float[1];
//    cross_p = read_float[2];
//    cross_i = read_float[3];
//    cross_d = read_float[4];
    cs[1] = read_float[5];
    cs[2] = read_float[6];
    cs[3] = read_float[7];
    cs[4] = read_float[8];
    cs[5] = read_float[9]; 
    cs[6] = read_float[10];
    cs[7] = read_float[11];
    cs[8] = read_float[12];
    cs[9] = read_float[13];
   
}

void ShowMenu(void)
{ 
    char txt[20];
    
    if(s_flag == 1){
          OLED_CLS();  
          s_flag = 0;
    
          
        switch(last_show){
            case 1://电感相关信息
              {
                sprintf(txt, "%d", AD_max[0]);
                OLED_P6x8Str(0, 0, (uint8_t*)txt);
                
                sprintf(txt,"%d", AD_max[1]);
                OLED_P6x8Str(0, 1, (uint8_t*)txt);
                
                sprintf(txt,"%d",AD_max[2]);
                OLED_P6x8Str(0,2,(uint8_t*)txt);
                
                sprintf(txt,"%d",AD_max[3]);
                OLED_P6x8Str(0,3,(uint8_t*)txt);      
           
                sprintf(txt,"%d",AD_max[4]);
                OLED_P6x8Str(0,4,(uint8_t*)txt);
                
            
                sprintf(txt, "%d", AD_min[0]);
                OLED_P6x8Str(25, 0, (uint8_t*)txt);
                
                sprintf(txt,"%d", AD_min[1]);
                OLED_P6x8Str(25, 1, (uint8_t*)txt);
                
                sprintf(txt,"%d",AD_min[2]);
                OLED_P6x8Str(25,2,(uint8_t*)txt);
                
                sprintf(txt,"%d",AD_min[3]);
                OLED_P6x8Str(25,3,(uint8_t*)txt);      
           
                sprintf(txt,"%d",AD_min[4]);
                OLED_P6x8Str(25,4,(uint8_t*)txt);
                
        
                sprintf(txt, "L:%.2f", elect_L);
                OLED_P6x8Str(65, 0, (uint8_t*)txt);
        
                sprintf(txt, "M:%.2f", elect_M);
                OLED_P6x8Str(65, 1, (uint8_t*)txt);
        
                sprintf(txt, "R:%.2f", elect_R);
                OLED_P6x8Str(65, 2, (uint8_t*)txt);
                
                sprintf(txt, "UL:%.2f", elect_Up_L);
                OLED_P6x8Str(65, 3, (uint8_t*)txt);
                
                sprintf(txt, "UR:%.2f", elect_Up_R);
                OLED_P6x8Str(65, 4, (uint8_t*)txt);
                
                sprintf(txt, "%d", AD_data[0]);
                OLED_P6x8Str(45, 0, (uint8_t*)txt);
        
                sprintf(txt, "%d", AD_data[1]);
                OLED_P6x8Str(45, 1, (uint8_t*)txt);
        
                sprintf(txt, "%d", AD_data[2]);
                OLED_P6x8Str(45, 2, (uint8_t*)txt);
                
                sprintf(txt, "%d", AD_data[3]);
                OLED_P6x8Str(45, 3, (uint8_t*)txt);
                
                sprintf(txt, "%d", AD_data[4]);
                OLED_P6x8Str(45, 4, (uint8_t*)txt);

                
                sprintf(txt, "read_flash_type:%d", read_flash_type);
                OLED_P6x8Str(0, 5, (uint8_t*)txt);
                
                sprintf(txt, "elect_val:%.3f", curr_elect_val);
                OLED_P6x8Str(0, 6, (uint8_t*)txt);
                
                sprintf(txt, "up_elect_val:%.3f", up_curr_elect_val);
                OLED_P6x8Str(0, 7, (uint8_t*)txt);
                    
              }
              break;
            case 2:       
              {
                  sprintf(txt, "loseThres:%d", Lose_Line_Thres);
                  OLED_P6x8Str(0, 0, (uint8_t*)txt);
                  
                  sprintf(txt, "refindLine:%d", REFIND_LINE);
                  OLED_P6x8Str(0, 1, (uint8_t*)txt);
                  
                  sprintf(txt, "getinto_speed:%d", getinto_speed);
                  OLED_P6x8Str(0, 2, (uint8_t*)txt);  

                  
                  sprintf(txt, "garage_cnt:%d", garage_cnt);
                  OLED_P6x8Str(0, 3, (uint8_t*)txt);  
                    
                  
                   sprintf(txt, "gyro_garage:%d", gyro_garage);
                   OLED_P6x8Str(0, 4, (uint8_t*)txt);

                    
                  sprintf(txt, "start_line_thres:%d", start_line_thres);
                  OLED_P6x8Str(0, 5, (uint8_t*)txt);
                    

     
                    for(uint8_t i=0; i<128; i++)
                    {
                      OLED_Set_Pos(i , 7); 
                      if(ccd_buf[i] > start_line_thres)	      			
                        OLED_WrDat(0xFF);
                      else
                        OLED_WrDat(0x00);	
                    }
              }
              break;
            case 3:
              {
                sprintf(txt, "sendflag:%d", sendflag);
                OLED_P6x8Str(0, 0, (uint8_t*)txt);
                
                sprintf(txt, "dir_pwm_max:%d", dir_pwm_max);
                OLED_P6x8Str(0, 1, (uint8_t*)txt);
                
                sprintf(txt, "dir_pwm_min:%d", dir_pwm_min);
                OLED_P6x8Str(0, 2, (uint8_t*)txt);
                
             
                
                                
             
              }
              break;
            case 4:
              {
                   sprintf(txt, "getinto_circle:%d", getinto_circle_thres);
                   OLED_P6x8Str(0, 0, (uint8_t*)txt);
                   
                   sprintf(txt, "goto_circle_gyro:%d", goto_circle_gyro);
                   OLED_P6x8Str(0, 1, (uint8_t*)txt);
               
                   sprintf(txt, "CircleThres:%d", Circle_elect_thres);
                   OLED_P6x8Str(0, 2, (uint8_t*)txt);
                     
                   sprintf(txt, "in_circle_thres:%d", in_circle_thres);
                   OLED_P6x8Str(0, 3, (uint8_t*)txt); 
                     
                   sprintf(txt, "gryo_circle:%d", gryo_circle);
                   OLED_P6x8Str(0, 4, (uint8_t*)txt);
                     
                   if(RightCircle_Flag == 1){//左出车库，右进环岛
                      sprintf(txt, "RightCircle:%d", RightCircle_Flag);
                      OLED_P6x8Str(0, 5, (uint8_t*)txt);

                  }else if(LeftCircle_Flag == 1){
                      
                      sprintf(txt, "LeftCircle:%d", LeftCircle_Flag);
                      OLED_P6x8Str(0, 5, (uint8_t*)txt);
                      

                  }


              }
              break;
            case 5:
              {


               
               
               
               sprintf(txt, "accsenser:%d", accsenser);
               OLED_P6x8Str(0, 6, (uint8_t*)txt);
                     
               sprintf(txt, "gyrosensor:%d", gyrosensor);
               OLED_P6x8Str(0, 7, (uint8_t*)txt);
               

            }
               
              break;
            case 6:
              {
                   sprintf(txt, "speed1:%d", speed_duty[1]);
                   OLED_P6x8Str(0, 0, (uint8_t*)txt);
                   
                   sprintf(txt, "cs1:%.2f", cs[1]);
                   OLED_P6x8Str(80, 0, (uint8_t*)txt);
               
                   sprintf(txt, "speed2:%d", speed_duty[2]);
                   OLED_P6x8Str(0, 1, (uint8_t*)txt);
                   
                   sprintf(txt, "cs2:%.2f", cs[2]);
                   OLED_P6x8Str(80, 1, (uint8_t*)txt);
                     
                   sprintf(txt, "speed3:%d", speed_duty[3]);
                   OLED_P6x8Str(0, 2, (uint8_t*)txt); 
                   
                   sprintf(txt, "cs3:%.2f", cs[3]);
                   OLED_P6x8Str(80, 2, (uint8_t*)txt);
                   
                   sprintf(txt, "speed4:%d", speed_duty[4]);
                   OLED_P6x8Str(0, 3, (uint8_t*)txt);
                   
                   sprintf(txt, "cs4:%.2f", cs[4]);
                   OLED_P6x8Str(80, 3, (uint8_t*)txt);
               
                   sprintf(txt, "speed5:%d", speed_duty[5]);
                   OLED_P6x8Str(0, 4, (uint8_t*)txt);
                   
                   sprintf(txt, "cs5:%.2f", cs[5]);
                   OLED_P6x8Str(80, 4, (uint8_t*)txt);
                     
                   sprintf(txt, "Speed_Set:%d", Speed_Set);
                   OLED_P6x8Str(0, 5, (uint8_t*)txt);
                   
                  sprintf(txt, "duty_motor:%d", duty_motor);
                  OLED_P6x8Str(0, 6, (uint8_t*)txt);
                   
               
                   
              }
              break;
            case 7:
              {

                sprintf(txt, "test_startline:%d",test_startline );
                OLED_P6x8Str(0, 0, (uint8_t*)txt);
                

                sprintf(txt, "test_roadpart:%d" ,test_roadpart );
                OLED_P6x8Str(0, 1, (uint8_t*)txt);

                sprintf(txt, "test_circle:%d" ,test_circle );
                OLED_P6x8Str(0, 2, (uint8_t*)txt);
                

                sprintf(txt, "direction_into_garage:%d",direction_into_garage );
                OLED_P6x8Str(0, 3, (uint8_t*)txt);

                
                
               sprintf(txt, "direction_pwm:%d", direction_pwm);
               OLED_P6x8Str(0, 6, (uint8_t*)txt);
                   
              }
              break;
              
            case 8:
              {
                   sprintf(txt, "speed6:%d", speed_duty[6]);
                   OLED_P6x8Str(0, 0, (uint8_t*)txt);
                   
                   sprintf(txt, "cs6:%.2f", cs[6]);
                   OLED_P6x8Str(80, 0, (uint8_t*)txt);
                
                
                   sprintf(txt, "speed7:%d", speed_duty[7]);
                   OLED_P6x8Str(0, 1, (uint8_t*)txt);
                   
                   sprintf(txt, "cs7:%.2f", cs[7]);
                   OLED_P6x8Str(80, 1, (uint8_t*)txt);
               
                   sprintf(txt, "speed8:%d", speed_duty[8]);
                   OLED_P6x8Str(0, 2, (uint8_t*)txt);
                   
                   sprintf(txt, "cs8:%.2f", cs[8]);
                   OLED_P6x8Str(80, 2, (uint8_t*)txt);
                     
                   sprintf(txt, "speed9:%d", speed_duty[9]);
                   OLED_P6x8Str(0, 3, (uint8_t*)txt);
                   
                   sprintf(txt, "cs9:%.2f", cs[9]);
                   OLED_P6x8Str(80,3, (uint8_t*)txt);
                   
                   
                  sprintf(txt, "L_encoder:%d", L_encoder);
                  OLED_P6x8Str(0, 6, (uint8_t*)txt);
               
                  sprintf(txt, "R_encoder:%d", R_encoder);
                  OLED_P6x8Str(0, 7, (uint8_t*)txt);
                   
                   
                   
              }
              break;
            }
    }
    
    switch(sendflag){
        case 0:
            OutData[0]=elect_L;
            OutData[1]=elect_M;
            OutData[2]=elect_R;
            OutData[3]=gyroy;
            break;
        case 1:
            OutData[0]=0;
            OutData[1]=gyroz;
            OutData[2]=curr_elect_val*500 ;
            OutData[3]=up_elect_val_delta * 2000;
            break;
        case 2:
            OutData[0]=elect_L;
            OutData[1]=elect_M;
            OutData[2]=elect_R;
            OutData[3]=leftRising_amplitude;
            break;
        
        case 3:
            OutData[0]=L_encoder;
            OutData[1]=R_encoder;
            OutData[2]=elect_val_delta * 1000;
            OutData[3]=gyrosensor;
            break;
    }
    
           
    if(!last_send){
        OutPut_Data();
    }else{
        SendImageData();
    }
   
}

/*
 * 检测按键是否按下
 */
void Check_BottonPress()
{
  
    static uint8_t key_up=1;     //按键松开标志
    char txt[20];
    for (uint8_t i=0; i<4; i++) 
    {
        for (uint8_t j=0; j<4; j++)
        {
            if (backup[i][j] != KeySta[i][j]) 
            {
                if (backup[i][j] != 0)       
                {
                    press_key = i*4+j + 1;     
                }
                backup[i][j] = KeySta[i][j];  
            }
        }
    }
        
        
        
      
      if(press_key == 16){
            BELL_ON;
      }else{
            BELL_OFF;
      } 
    switch(press_key){
        case 1:
          if(last_show == 1){
                if(!out_death){//默认发送相关数据
                    out_death = 1;
                }else{
                      out_death = 0;
                }
          }
          last_show = 1;
          break;
        case 2:       
          last_show = 2;
          break;
        case 3:          
          last_show = 3;
          break;
        case 4:
          last_show = 4;
          break;
        case 5:
          last_show = 5;
          break;
        case 6:
          if(last_show == 6){
            last_show = 8;
          }else{
            last_show = 6;
          }
          break;
        case 7:
          last_show = 7;//总共七个页面.每一页6个参数
          break;
        case 8:
          if(stopflag == 0){
            stopflag = 1;
          }
          else{
            stopflag = 0;
          }
          break;               
        case 9:  //1  
          if(last_show == 1){
            if(read_flash_type == 1){
                read_flash_type = 0;
            }else if(read_flash_type == 0){
                read_flash_type = 1;
            }
          }
          else if(last_show == 2){Lose_Line_Thres += plus_minus * 10;}
          else if(last_show == 3){sendflag += plus_minus * 1;}
          else if(last_show == 4){getinto_circle_thres += plus_minus * 50;}
          else if(last_show == 5){}
          else if(last_show == 6){
            if(left_right_parameter)
                speed_duty[1] += plus_minus * 5;
            else
                cs[1] += plus_minus * 0.01;
              
          }
          else if(last_show == 7){
            if(test_startline == 1){
                test_startline = 0;
            }else{
                test_startline = 1;
            }
          }
          else if(last_show == 8){
            if(left_right_parameter)
                speed_duty[6] += plus_minus * 5;
            else
                cs[6] += plus_minus * 0.01;
            

          }
          break;
        case 10://2
          if(last_show == 2){REFIND_LINE += plus_minus * 5;}
          else if(last_show == 3){dir_pwm_max += plus_minus * 10;}
          else if(last_show == 4){goto_circle_gyro += plus_minus * 2;}
          else if(last_show == 5){}
          else if(last_show == 6){
            if(left_right_parameter)
                speed_duty[2] += plus_minus * 5;
            else
                cs[2] += plus_minus * 0.01;
          }
          else if(last_show == 7){
            if(test_roadpart == 1){
                test_roadpart = 0;
            }else{
                test_roadpart = 1;
            }
          }
          else if(last_show == 8){
            if(left_right_parameter)
                speed_duty[7] += plus_minus * 5;
            else
                cs[7] += plus_minus * 0.01;
            
        
          }
          break;
        case 11://3
          if(last_show == 2){getinto_speed += plus_minus * 10;}
          else if(last_show == 3){dir_pwm_min += plus_minus * 10;}
          else if(last_show == 4){Circle_elect_thres += plus_minus * 5;}
          else if(last_show == 5){}
          else if(last_show == 6){
            if(left_right_parameter)
                speed_duty[3] += plus_minus * 5;
            else
                cs[3] += plus_minus * 0.01;
          }
          else if(last_show == 7){
            if(test_circle == 1){
                test_circle = 0;
            }else{
                test_circle = 1;
            }
          
          }
          else if(last_show == 8){
            
            if(left_right_parameter)
                speed_duty[8] += plus_minus * 5;
            else
                cs[8] += plus_minus * 0.01;
            
         
          }
          break;
        case 12://4
          if(last_show == 2){garage_cnt += plus_minus * 20;}
          else if(last_show == 3){}
          else if(last_show == 4){in_circle_thres += plus_minus * 10;}
          else if(last_show == 5){}
          else if(last_show == 6){
            if(left_right_parameter)
                speed_duty[4] += plus_minus * 5;
            else
                cs[4] += plus_minus * 0.01;
          }
          else if(last_show == 7){
            if(direction_into_garage == 1){
                direction_into_garage = 2;
            }else if(direction_into_garage == 2){
                direction_into_garage = 1;
            }
          }
          else if(last_show == 8){
           if(left_right_parameter)
                speed_duty[9] += plus_minus * 5;
            else
                cs[9] += plus_minus * 0.01;
          }
          break;
        case 13://5
          if(last_show == 2){gyro_garage += plus_minus * 5;}
          else if(last_show == 3){}
          else if(last_show == 4){gryo_circle += plus_minus * 10;}
          else if(last_show == 5){}
          else if(last_show == 6){
            if(left_right_parameter)
                speed_duty[5] += plus_minus * 5;
            else
                cs[5] += plus_minus * 0.01;
          }
          else if(last_show == 7){}
          else if(last_show == 8){}
          break;
        case 14://6
          if(last_show == 1){up_thres += plus_minus * 5;}
          else if(last_show == 2){start_line_thres += plus_minus * 10;}
          else if(last_show == 3){}
          else if(last_show == 4){}
          else if(last_show == 5){}
          else if(last_show == 6){
            Speed_Set += plus_minus * 1;
          }
          else if(last_show == 7){}
          else if(last_show == 8){}
          break;
        case 15:
          Read_Flash();      
          break;
        case 16:
          WriteFlash();
          break;
    }
    press_key = 0;   
    
    if(key_up && (GPIO_PinRead(K1)==0 || GPIO_PinRead(K2)==0 || GPIO_PinRead(K3)==0 ))
    {
      BELL_ON;
      delayms(100);   //消抖
      key_up=0;
      BELL_OFF;
      
      if(GPIO_PinRead(K1)==0)      
      { 
          if(left_right_parameter == 1){//默认加
                left_right_parameter = 0;
          }else if(left_right_parameter == 0){
                left_right_parameter = 1;
          }
      }
      
      if(GPIO_PinRead(K2)==0) 
      {
          if(plus_minus == 1){//默认加
                plus_minus = -1;
          }else if(plus_minus == -1){
                plus_minus = 1;
          }
      }
      
      if(GPIO_PinRead(K3)==0) 
      {

            Circle_Flag = 0;
            Circle_Pre_flag = 0;
            In_Out_Roundabout_I = 0;
            LeftCircle_Flag = 0;
            RightCircle_Flag = 0;
            
//                //入库相关变量
//              if(direction_into_garage == 1){
//                direction_into_garage = 1;//left
//              }else{
//                direction_into_garage = 2;//right
//              }

            encoder_cnt = 0;
            one_circle_flag = 1;
            getinto_garage_flag = 0;
            if(test_roadpart == 0){
                start_car = 0;
            }
            In_Out_Roundabout_I = 0;
      }
      
    }
    
    if(GPIO_PinRead(K1)==1 && GPIO_PinRead(K2)==1 && GPIO_PinRead(K3)==1 ) 
    {
        key_up=1;   
    }
   
}

void LED()
{
  if(elect_Up_L > Circle_elect_thres && elect_Up_R > Circle_elect_thres)
  { 
        LED_ON(LED_2);
  }
  else
  {
        LED_OFF(LED_2);
  }

  if( Circle_Pre_flag == 1)
  { 
        LED_ON(LED_5);
  }
  else
  {
        LED_OFF(LED_5);
  }
  
  if( Circle_Flag == 1)
  { 
        LED_ON(LED_6);
  }
  else
  {
        LED_OFF(LED_6);
  }
  
  if(!last_send){
        _LED_ON(LED_9);
  }else{
        _LED_OFF(LED_9);
  } 
  
  if(plus_minus == 1){
        _LED_ON(LED_10);
  }else if(plus_minus == -1){
        _LED_OFF(LED_10);
  } 
  
  if(stopflag == 1){
    _LED_ON(LED_11);
  }else{
    _LED_OFF(LED_11);
  } 
  
  if(one_circle_flag){
        _LED_ON(LED_12);
  }else if(plus_minus == -1){
        _LED_OFF(LED_12);
  } 
  
  
  if(left_right_parameter == 1){
        LED_ON(LED_7);
  }else{
        LED_OFF(LED_7);
  } 

  
}

void SendImageData(void) 
{
   uint8 i = 0;
   UART_PutChar(UART4,0x00); 
   UART_PutChar(UART4,0xff);
   UART_PutChar(UART4,0x01);
   UART_PutChar(UART4,0x00);
   
   for(i=0; i<128; i++)                        //数据
   {
      UART_PutChar(UART4,ccd_buf[i]>>8);   //发送高8位
      UART_PutChar(UART4,ccd_buf[i]&0XFF); //发送高低8位
   }
}

//==========================================
//函数名CRC_CHECK() 
//作用：  循环冗余校验，供波形输出函数调用
//==========================================
unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;
    unsigned char i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<CRC_CNT; i++){      
        CRC_Temp ^= Buf[i];
        for (j=0;j<8;j++) {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}

//==========================================
//函数名OutPut_Data() 
//作用：  波形输出，用虚拟示波器看波形时调用此函数发送数据
//==========================================
void OutPut_Data()
{
  int temp[4] = {0};
  unsigned int temp1[4] = {0};
  unsigned char databuf[10] = {0};
  unsigned char i;
  unsigned short CRC16 = 0;
  for(i=0;i<4;i++)
   {  
    temp[i]  = (int)OutData[i];
    temp1[i] = (unsigned int)temp[i];   
   }
   
  for(i=0;i<4;i++) 
  {
    databuf[i*2]   = (unsigned char)(temp1[i]%256);
    databuf[i*2+1] = (unsigned char)(temp1[i]/256);
  }
  
  CRC16 = CRC_CHECK(databuf,8);
  databuf[8] = CRC16%256;
  databuf[9] = CRC16/256;
  for(i=0;i<10;i++)
  {
   UART_PutChar(UART4,databuf[i]);
  }
}

void sendufuzzymain()
{
  char txt[10];
//  if(send_fuzzy_flag){
      UART_PutStr(UART4, "*");
      for(int i = 0;i<8;i++){  
        sprintf(txt,"%d",fuzzy_txt[i]);
        if(i==0 || i == 2 || i == 4 || i == 6){
            UART_PutStr (UART4, "[");
            UART_PutStr (UART4, txt);
            UART_PutStr (UART4, ".");
        }
        else{
            UART_PutStr (UART4, txt);
            UART_PutStr (UART4, "]");
        }       
      } 
//  }
}

void KeyScan()
{
    unsigned char i;
    static unsigned char keyout = 0;  
    static unsigned char keybuf[4][4] = { 
        {0xFF, 0xFF, 0xFF, 0xFF},  {0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF},  {0xFF, 0xFF, 0xFF, 0xFF}
    };
    
    keybuf[keyout][0] = (keybuf[keyout][0] << 1) | INPUT_STATE(KEY_IN_1);
    keybuf[keyout][1] = (keybuf[keyout][1] << 1) | INPUT_STATE(KEY_IN_2);
    keybuf[keyout][2] = (keybuf[keyout][2] << 1) | INPUT_STATE(KEY_IN_3);
    keybuf[keyout][3] = (keybuf[keyout][3] << 1) | INPUT_STATE(KEY_IN_4);

    for (i=0; i<4; i++)  
    {
        if ((keybuf[keyout][i] & 0x0F) == 0x00)
        {  
            KeySta[keyout][i] = 0;
        }
        else if ((keybuf[keyout][i] & 0x0F) == 0x0F)
        {  
            KeySta[keyout][i] = 1;
        }
    }

    keyout++;               
    keyout = keyout & 0x03;  
    switch (keyout)          
    {
        case 0: GPIO_PinWrite(KEY_OUT_4 , 1); GPIO_PinWrite(KEY_OUT_1 , 0); break;
        case 1: GPIO_PinWrite(KEY_OUT_1 , 1); GPIO_PinWrite(KEY_OUT_2 , 0); break;
        case 2: GPIO_PinWrite(KEY_OUT_2 , 1); GPIO_PinWrite(KEY_OUT_3 , 0); break;
        case 3: GPIO_PinWrite(KEY_OUT_3 , 1); GPIO_PinWrite(KEY_OUT_4 , 0); break;
        default: break;
    }
}

