#include "electromagnetics.h"

float elect_L_Filter[5];                    //左侧电感值
float elect_M_Filter[5];                    //中部电感值
float elect_R_Filter[5];                    //右侧电感值
float elect_L = 0.0;
float elect_R = 0.0;
float elect_M = 0.0;
float elect_Up_L = 0.0;
float elect_Up_R = 0.0;
float curr_elect_val = 0.0;           //电磁偏差,差和比运算的结果
float up_curr_elect_val = 0.0;           //电磁偏差,差和比运算的结果
float elect_val_delta = 0.0;          //电磁值偏差变化率
float up_elect_val_delta = 0.0;          //竖直电磁值偏差变化率
float last_elect_val = 0.0;           //上一次的电磁偏差
float up_last_elect_val = 0.0;           //上一次竖直的电磁偏差

int AD_data[6] = { 0,0,0,0,0 }, AD_G[6] = { 0,0,0,0,0 };       // 电感数据,归一化值
int AD_temp[4][6];
int AD_max[6] = { 0 , 0 , 0 , 0 , 0 , 0 };// { 3619 , 3883 , 3782 , 0, 0 , 0 };
int AD_min[6] = { 1000 , 1000 , 1000 , 1000 , 1000 , 0 };       //电感极值 //{ 170 , 170 , 170 , 0 , 0 , 0 }; 

float Elect_bias_delta;           //电磁变化率
float Curr_Elect_bias;            //当前电磁偏差
float Last_Elect_bias;            //上一次电磁偏差
float L_Last_Elect_bias;          //上上次电磁偏差
float L_Last_elect_out;
float Last_elect_out;             //上一次电磁输出
float Elect_Out_Max;              //电磁控制量输出限幅
float Elect_Out_Min;

uint8_t lose_line = 1;
uint8_t lose_line_dir = 0;

//方案2 模糊
int kd_elc = 150;  //
float gyroy_d = 2; //陀螺仪的变化d

uint8_t dir_pwm_enable = 0; 
int fuzzy_txt[10];
uint16 AD_valu[4]={0};
int direction_pwm = 0;
int last_direction_pwm = 0;
/**********************环岛相关变量************************/
uint8_t Circle_Flag = 0;       //环岛标志位
uint8_t LeftCircle_Flag = 0;   //左环
uint8_t RightCircle_Flag = 0;  //右环
uint8_t Circle_Pre_flag = 0;
int Circle_elect_thres = 970;

float upl_elect[3],upr_elect[3];
float leftDecline_amplitude,leftRising_amplitude,leftSensorMax;
float first_leftDecline_amplitude,first_rightDecline_amplitude;   
float rightDecline_amplitude,rightRising_amplitude,rightSensorMax;
uint8_t left_down_flag,right_down_flag;
uint8_t second_left_down_flag,second_right_down_flag;

int losingadd = 25;
int risingadd = 25;
int lose_in_circle = 50;
int up_thres = 0;
int in_circle_thres = 0;
int goto_circle_gyro;
int getinto_circle_thres = 0;
int dir_pwm_max  = 800;
int dir_pwm_min  = -800;
/**********************环岛相关变量************************/
/**********加减速相关变量***************************/
float speed_seg = 0;
/**********加减速相关变量***************************/
/**********十字相关变量***************************/
uint8_t crosstype = 0;
uint8_t cross_cnt = 0;
uint8_t left_cross = 0;
uint8_t right_cross = 0;
uint8_t cross_enable = 0;
uint8_t cross_out = 0;
uint8_t cross_pre = 0;
int circle_up_thres = 970;
/**********十字速相关变量***************************/
/**********丢线相关变量***************************/
int Lose_Line_Thres = 70;
int REFIND_LINE   = 75;
int l_r_sum = 90;
/**********丢线相关变量***************************/

/**********舵机打角相关变量***************************/

float elec_k = 1.7;
float elec_k_r = 1;
float up_elec_l  = 1;
float up_elec_r = 1;

/**********舵机打角相关变量***************************/
/************************ 模糊运算引擎 ************************/
#define PMAX  100
#define PMIN -100
#define DMAX  100
#define DMIN -100
#define FMAX  100       //语言值的满幅值
int PFF[4] = { 0,6,14,28};
/*输入量D语言值特征点*/
int DFF[4] = { 0,2,4,6};
/*输出量U语言值特征点*/
int UFF[7] = { 0,150,300,450,600,750,900};

int rule[7][7]={//误差变化率 -3,-2,-1, 0, 1, 2, 3                //   误差
                {-6,-6,-6,-6,-5,-5,-4,},                        //   -3      0
                {-5,-5,-5,-4,-4,-3,-1,},                        //   -2      1
                {-4,-4,-3,-1, 0, 1, 2,},                        //   -1      2
                {-4,-3,-1, 0, 1, 3, 4,},                        //    0      3
                {-2,-1, 0, 1, 3, 4, 4,},                        //    1      4
                { 1, 3, 4, 4, 5, 5, 5,},                        //    2      5
                { 4, 5, 5, 6, 6, 6, 6} };                        //    3     6  5.94

float Fuzzy(float P, float D)
{
    float U;                //偏差,偏差微分以及输出值的精确量
    float PF[2], DF[2], UF[4];//偏差,偏差微分以及输出值的隶属度
    int Pn, Dn, Un[4];
    float temp1, temp2;

    /*隶属度的确定*/
    /*根据PD的指定语言值获得有效隶属度*/
    if (P > -PFF[3] && P < PFF[3])
    {
        if (P <= -PFF[2])
        {
            Pn = -2;
            PF[0] = FMAX * ((float)(-PFF[2] - P) / (PFF[3] - PFF[2]));
        }
        else if (P <= -PFF[1])
        {
            Pn = -1;
            PF[0] = FMAX * ((float)(-PFF[1] - P) / (PFF[2] - PFF[1]));
        }
        else if (P <= PFF[0])
        {
            Pn = 0;
            PF[0] = FMAX * ((float)(-PFF[0] - P) / (PFF[1] - PFF[0]));
        }
        else if (P <= PFF[1])
        {
            Pn = 1; PF[0] = FMAX * ((float)(PFF[1] - P) / (PFF[1] - PFF[0]));
        }
        else if (P <= PFF[2])
        {
            Pn = 2; PF[0] = FMAX * ((float)(PFF[2] - P) / (PFF[2] - PFF[1]));
        }
        else if (P <= PFF[3])
        {
            Pn = 3; PF[0] = FMAX * ((float)(PFF[3] - P) / (PFF[3] - PFF[2]));
        }
    }
    else if (P <= -PFF[3])
    {
        Pn = -2; PF[0] = FMAX;
    }
    else if (P >= PFF[3])
    {
        Pn = 3; PF[0] = 0;
    }
    PF[1] = FMAX - PF[0];

    if (D > -DFF[3] && D < DFF[3])
    {
        if (D <= -DFF[2])
        {
            Dn = -2; DF[0] = FMAX * ((float)(-DFF[2] - D) / (DFF[3] - DFF[2]));
        }
        else if (D <= -DFF[1])
        {
            Dn = -1;
            DF[0] = FMAX * ((float)(-DFF[1] - D) / (DFF[2] - DFF[1]));
        }
        else if (D <= DFF[0])
        {
            Dn = 0;
            DF[0] = FMAX * ((float)(-DFF[0] - D) / (DFF[1] - DFF[0]));
        }
        else if (D <= DFF[1])
        {
            Dn = 1;
            DF[0] = FMAX * ((float)(DFF[1] - D) / (DFF[1] - DFF[0]));
        }
        else if (D <= DFF[2])
        {
            Dn = 2; DF[0] = FMAX * ((float)(DFF[2] - D) / (DFF[2] - DFF[1]));
        }
        else if (D <= DFF[3])
        {
            Dn = 3; DF[0] = FMAX * ((float)(DFF[3] - D) / (DFF[3] - DFF[2]));
        }
    }
    else if (D <= -DFF[3])
    {
        Dn = -2;
        DF[0] = FMAX;
    }
    else if (D >= DFF[3])
    {
        Dn = 3;
        DF[0] = 0;
    }
    DF[1] = FMAX - DF[0];

    /*使用误差范围优化后的规则表rule[7][7]*/
    /*输出值使用13个隶属函数,中心值由UFF[7]指定*/
    /*一般都是四个规则有效*/
    fuzzy_txt[0]=Pn - 1 + 3;
    fuzzy_txt[1]=Dn - 1 + 3;
    fuzzy_txt[2]=Pn + 3;
    fuzzy_txt[3]=Dn - 1 + 3;
    fuzzy_txt[4]=Pn - 1 + 3;
    fuzzy_txt[5]=Dn + 3;
    fuzzy_txt[6]=Pn + 3;
    fuzzy_txt[7]=Dn + 3;
    
    Un[0] = rule[Pn - 1 + 3][Dn - 1 + 3];
    Un[1] = rule[Pn + 3][Dn - 1 + 3];
    Un[2] = rule[Pn - 1 + 3][Dn + 3];
    Un[3] = rule[Pn + 3][Dn + 3];
    if (PF[0] <= DF[0])	UF[0] = PF[0];
    else 	UF[0] = DF[0];
    if (PF[1] <= DF[0])	UF[1] = PF[1];
    else 	UF[1] = DF[0];
    if (PF[0] <= DF[1])	UF[2] = PF[0];
    else 	UF[2] = DF[1];
    if (PF[1] <= DF[1])	UF[3] = PF[1];
    else 	UF[3] = DF[1];

    /*同隶属函数输出语言值求大*/
    if (Un[0] == Un[1])
    {
        if (UF[0] > UF[1])	UF[1] = 0;
        else	UF[0] = 0;
    }
    if (Un[0] == Un[2])
    {
        if (UF[0] > UF[2])	UF[2] = 0;
        else	UF[0] = 0;
    }
    if (Un[0] == Un[3])
    {
        if (UF[0] > UF[3])	UF[3] = 0;
        else	UF[0] = 0;
    }
    if (Un[1] == Un[2])
    {
        if (UF[1] > UF[2])	UF[2] = 0;
        else	UF[1] = 0;
    }
    if (Un[1] == Un[3])
    {
        if (UF[1] > UF[3])	UF[3] = 0;
        else	UF[1] = 0;
    }
    if (Un[2] == Un[3])
    {
        if (UF[2] > UF[3])	UF[3] = 0;
        else	UF[2] = 0;
    }
//
    /*重心法反模糊*/
    /*Un[]原值为输出隶属函数标号，转换为隶属函数值*/
    if (Un[0] >= 0)	Un[0] = UFF[Un[0]];
    else			Un[0] = -UFF[-Un[0]];
    if (Un[1] >= 0)	Un[1] = UFF[Un[1]];
    else			Un[1] = -UFF[-Un[1]];
    if (Un[2] >= 0)	Un[2] = UFF[Un[2]];
    else			Un[2] = -UFF[-Un[2]];
    if (Un[3] >= 0)	Un[3] = UFF[Un[3]];
    else			Un[3] = -UFF[-Un[3]];

    temp1 = UF[0] * Un[0] + UF[1] * Un[1] + UF[2] * Un[2] + UF[3] * Un[3];
    temp2 = UF[0] + UF[1] + UF[2] + UF[3];
    U = temp1 / temp2;

    return U;
}

float my_sqrt(float number)
{
	long i;
	float x, y;
	const float f = 1.5F;
	x = number * 0.5F;
	y = number;
	i = * ( long * ) &y;
	i = 0x5f3759df - ( i >> 1 );

	y = * ( float * ) &i;
	y = y * ( f - ( x * y * y ) );
	y = y * ( f - ( x * y * y ) );
	return number * y;
}

void sendFuzzyData(){
  char txt[10];
 
   UART_PutStr(UART4, "-");
  
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
}

/******************************************************
* 电磁读取
*******************************************************/
void Steer_Scan(void)
{  
    int i;
    Foresight_Data(AD_data);                          // 读取前瞻数据 
    for(i=0;i<5;i++)
    {
      if(AD_data[i]>AD_max[i]) AD_max[i]=AD_data[i];
      else if(AD_data[i]<AD_min[i]) AD_min[i]=AD_data[i];
    }
}

void  LoseLineJudge() {

    if (!lose_line) {
        if(elect_L <= Lose_Line_Thres || elect_R <= Lose_Line_Thres){
                lose_line = 1;
        }
    }

}

void LoseLineDeal()
{
    uint8_t left, right;
    
    if(lose_line){
      if(Circle_Flag){
          if(RightCircle_Flag == 1){
            direction_pwm = dir_pwm_max;//右丢线
          }else if(LeftCircle_Flag == 1){
            direction_pwm = dir_pwm_min;//左丢线
          }
      }else{
             if (!dir_pwm_enable) {//只在刚丢线的时候进行最大幅度打角
                    if (elect_L >= elect_R) {
                        lose_line_dir = 1; //左丢线
                    }
                    else {
                        lose_line_dir = 2; //右丢线
                    }
                    dir_pwm_enable = 1;
             }

            if (lose_line_dir == 1) {
                direction_pwm = dir_pwm_min;//左丢线
            }
            else if (lose_line_dir == 2) {//右丢线
                direction_pwm = dir_pwm_max;
            }

       }
    }
  
    if(start_car == 1){
        lose_line = 1;
         if(direction_into_garage == 2){//right
             lose_line_dir = 2;
         }else if(direction_into_garage == 1){
             lose_line_dir = 1;
         }
    }
    
    if (lose_line) {
        if(elect_L >= REFIND_LINE && elect_R >= REFIND_LINE && elect_M >= REFIND_LINE){
                lose_line = 0;
                dir_pwm_enable = 0;
                if(start_car == 1){
                    start_car = 2;
                }
        }
        
    }
    
}

void OutGarage()
{
    if(elect_Up_L >= up_thres && elect_Up_R >= up_thres){
        start_car = 1;
    }
     
}

void Foresight_Data( int * AD_data)      // AD数据读取
{
  AD_data[0] = (ADC_Once(ADC1,ADC1_SE8,ADC_12bit));  //B0
  AD_data[1] = (ADC_Once(ADC1,ADC1_SE12,ADC_12bit));   //B6 
  AD_data[2] = (ADC_Once(ADC1,ADC1_SE9,ADC_12bit));    //B1
  AD_data[3] = (ADC_Once(ADC1,ADC1_SE10,ADC_12bit));    //B4
  AD_data[4] = (ADC_Once(ADC1,ADC1_SE11,ADC_12bit));   //B5
}

void Get_Elect_Data()
{
    //////////滑动滤波////////////
    for(int i=0;i<5;i++)
    {
        for(int j=0;j<3;j++)
           AD_temp[j][i]=AD_temp[j+1][i];     //滑动
    }
    Foresight_Data(AD_temp[3]);             // 读取前瞻数据
    for(int i=0;i<5;i++)
    {
        AD_data[i]=0;
        for(int j=0;j<4;j++)
        {
          AD_data[i]+=AD_temp[j][i];
        }
        AD_data[i]=AD_data[i]/4;
    }

    //限幅
    for(int i=0;i<5;i++)
    {
       if(AD_data[i] > 4096) 
         AD_data[i] = 4906;
       else if(AD_data[i] < 25)
         AD_data[i] = 25;//不能和最小值相同，避免0情况
    }

    
    /////////////////归一化//////////////////
    for(int i=0;i<5;i++)
    {
        AD_G[i]=(float)( (AD_data[i]-AD_min[i])*1.0/(AD_max[i]-AD_min[i])*1000 );
        if(AD_G[i]>1000)   AD_G[i]=1000;
        else if(AD_G[i]<1) AD_G[i]=1;
    }
    
    //电感数值
    elect_L = AD_G[0];//L
    elect_M = AD_G[1];//M
    elect_R = AD_G[2];//R
    elect_Up_L = AD_G[3];//UL
    elect_Up_R = AD_G[4];//UR
  
/*************************************over************************************************/
  last_elect_val = curr_elect_val;
  up_last_elect_val = up_curr_elect_val;

    //水平电感差比和
  curr_elect_val =  (my_sqrt(elect_R) - my_sqrt(elect_L ))/ (my_sqrt(elect_L) + my_sqrt(elect_R)+ my_sqrt(elect_M));
  //竖直电感差比和
  up_curr_elect_val =(my_sqrt(elect_Up_R ) - my_sqrt(elect_Up_L )) / (my_sqrt(elect_Up_R ) + my_sqrt(elect_Up_L) + my_sqrt(elect_M));
  
  curr_elect_val = KalmanFilter_Elect(curr_elect_val,last_elect_val);                 //卡尔曼滤波
  up_curr_elect_val = KalmanFilter_Elect(up_curr_elect_val,up_last_elect_val);
   
  elect_val_delta = curr_elect_val - last_elect_val;                                            //电磁变化率   
  up_elect_val_delta = up_curr_elect_val - up_last_elect_val;
  
  if(fabs(curr_elect_val) <= fabs(up_curr_elect_val)){//近端电感差比和大，弯道中间
    curr_elect_val = up_curr_elect_val;
    elect_val_delta = up_elect_val_delta;
    
  }
  
 }

void Road_IsLand(void)//环岛处理
{


//    if(elect_L > Circle_elect_thres && elect_R > Circle_elect_thres && elect_M > Circle_elect_thres){
    if(elect_Up_L > Circle_elect_thres && elect_Up_R > Circle_elect_thres){
//      BELL_ON;
      Circle_Pre_flag = 1;
//      BELL_OFF;
    }
    
     if(Circle_Pre_flag){

          if(encoder_cnt >= getinto_circle_thres){
//                BELL_ON;
                encoder_cnt = 0;
                Circle_Flag = 1;
//                BELL_OFF;
          }
      }
     
}

/******************************************************
* 电磁控制输出--计算得出舵机转向占空比
*******************************************************/
void Elect_Control_Out()
{  
//
    if(!Circle_Flag && one_circle_flag)
        Road_IsLand();

    
    last_direction_pwm = direction_pwm;
    /********舵机数据输出******************/   
        
    direction_pwm = Fuzzy( (curr_elect_val*45) , (elect_val_delta*100) ) + elect_val_delta*kd_elc \
            + gyrosensor * gyroy_d;

    
    /********舵机数据输出******************/
  
    LoseLineJudge();
    
    LoseLineDeal();
    
    if(Circle_Flag == 1){
      if(fabs(In_Out_Roundabout_I) < goto_circle_gyro){
          if(LeftCircle_Flag){
            direction_pwm = dir_pwm_min;
          }else if(RightCircle_Flag){
            direction_pwm = dir_pwm_max;
          }
      }else{

//          if(LeftCircle_Flag){//左环岛
//            if(direction_pwm >= -in_circle_thres){
//                direction_pwm = last_direction_pwm;
//            }
//          }else if(RightCircle_Flag){
//            if(direction_pwm < in_circle_thres){
//                direction_pwm = last_direction_pwm;
//            }
//          }
      }
    }
    
    if(getinto_garage_flag ){
      if(direction_into_garage == 1){//left
        direction_pwm = -900;
      }else if(direction_into_garage == 2){//right
        direction_pwm = 900;
      }
    }

    if (direction_pwm > dir_pwm_max){
        direction_pwm = dir_pwm_max;
    }else if (direction_pwm <= dir_pwm_min) {
        direction_pwm = dir_pwm_min;
    }

    FTM_PwmDuty(S3010_FTM, S3010_CH, S3010_MID + direction_pwm);    //修改舵机占空比
    
}

/******************************************************
* 停车控制
*******************************************************/
void Stop_Car()
{
    stopflag = 1;
}

/**********************************************
* 获取车身速度
***********************************************/
int last_encoder_left = 0;
int last_encoder_right = 0;
void Get_Speed()
{
    static int Left[10];
    static int Right[10];
    
    last_encoder_left = L_encoder;
    last_encoder_right = R_encoder;
    
    L_encoder = FTM_ABGet(FTM1);                       //获取左侧编码器计数
    R_encoder = -FTM_ABGet(FTM2);                      //获取右边编码器计数
  
    for(uint8_t i = 0;i < 9;i++){
        Left[i] = Left[i+1];
        Right[i] = Right[i+1];
    }
    
    Left[9] = L_encoder;
    Right[9] = R_encoder;

    L_encoder = 0;
    R_encoder = 0;
    for(int i= 0;i<10;i++){
        L_encoder += Left[i];
        R_encoder += Right[i];
    }
    
    L_encoder /= 10.0;
    R_encoder /= 10.0;
    
    L_encoder = 0.7 * last_encoder_left + 0.3 * L_encoder;
    R_encoder = 0.7 * last_encoder_right + 0.3 * R_encoder;
  
   car_encoder_speed = (L_encoder + R_encoder) * 0.5;
   
   if(Circle_Pre_flag)
     encoder_cnt+=(car_encoder_speed / 10);
   
   if(!stopflag && !start_car && !test_roadpart && !test_circle && !test_startline)
     encoder_cnt+=(car_encoder_speed / 10);
   
//  car_speed = car_encoder_speed * 0.00221 * 1000;     //获取车速，1000代表1ms读一次，单位cm/s
  
//  Run_Distance += car_speed * 0.005;
}

/**********************************************
* 蜂鸣器
***********************************************/
void Bell()
{
  //|| lose_line == 1 crosstype == 1   ||  Circle_Flag == 1 || lose_line == 1  ||   || into_garage_enable == 1
  if( (Circle_Flag == 1 || Circle_Pre_flag == 1 || start_car == 0) && !stopflag )
  { 
        BELL_ON;
  }
  else
  {
        BELL_OFF;
  }
  
  if(test_roadpart == 0 && !stopflag && start_car == 0){
     BELL_ON;
  }else{
     BELL_OFF;
  }
}

/**********************************************
* 电磁卡尔曼滤波
***********************************************/
float KalmanFilter_Elect(float curr_elect_val,float last_elect_val)
{
  static float Q_curr = 1.0;//0.1			//Q增大，动态响应增大，过程噪声的协方差
  static float Q_last = 0.0001;			//过程噪声的协方差，过程噪声的协方差为一个一行两列矩阵
  static float R_elect = 10.0;			        //测量噪声的协方差 即测量偏差

  static float Pk[2][2] = { {1, 0}, {0, 1 }};
  
  static float Pdot[4] = {0,0,0,0};

  static float q_bias = 0.0;
  static float elect_err = 0.0;
  static float PCt_0 = 0.0;
  static float PCt_1 = 0.0;
  static float E = 0.0;
  static float K_0 = 0.0, K_1 = 0.0, t_0 = 0.0, t_1 = 0.0;
  
  Pdot[0] = Q_curr - Pk[0][1] - Pk[1][0];		//Pk-先验估计误差协方差的微分
  Pdot[1] = -Pk[1][1];
  Pdot[2] = -Pk[1][1];
  Pdot[3] = Q_last;
  
  Pk[0][0] += Pdot[0] * dt;				//Pk-先验估计误差的协方差微分的积分
  Pk[0][1] += Pdot[1] * dt;				//先验估计误差协方差
  Pk[1][0] += Pdot[2] * dt;
  Pk[1][1] += Pdot[3] * dt;
  
  elect_err = curr_elect_val - last_elect_val;			//偏差 = 测量值 - 预测值，先验估计
  
  PCt_0 = Pk[0][0];
  PCt_1 = Pk[1][0];
  
  E = R_elect + PCt_0;
  
  K_0 = PCt_0 / E;
  K_1 = PCt_1 / E;
  
  t_0 = PCt_0;
  t_1 = Pk[0][1];
  
  Pk[0][0] -= K_0 * t_0;					//后验估计误差协方差
  Pk[0][1] -= K_0 * t_1;
  Pk[1][0] -= K_1 * t_0;
  Pk[1][1] -= K_1 * t_1;
  
  curr_elect_val += K_0 * elect_err; 		        //后验估计 更新最优电磁值 最优电磁值 = 预测值 + 卡尔曼增益*(测量值-预测值)
  q_bias += K_1 * elect_err;				//后验估计 更新误差		
    
  return curr_elect_val;

}
