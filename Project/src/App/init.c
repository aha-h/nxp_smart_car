#include "init.h"

/***************************
* 参数初始化
****************************/

uint8_t one_circle_flag = 1;

int into_garage_thres = 0;
uint8_t direction_into_garage = 0;

void Para_Init()
{                                           
//
//    if(GPIO_PinRead(BM1) == 1 && GPIO_PinRead(BM2) == 1 ){                              //00
//        Speed_Set = 1;
//    }
//    else if(GPIO_PinRead(BM1) == 1 && GPIO_PinRead(BM2) == 0){                        //01
//        Speed_Set = 2;
//    }
//    else if(GPIO_PinRead(BM1) == 0 && GPIO_PinRead(BM2) == 1){                        //10
//        Speed_Set = 3;
//    }
//    else if(GPIO_PinRead(BM1) == 0 && GPIO_PinRead(BM2) == 0 ){                       //11
//        Speed_Set = 4;
//    }
  
  
     test_startline = 1;
     test_roadpart = 1;
     test_circle = 1;
    direction_into_garage = 1;
    getinto_circle_thres = 2200;
  //环岛相关变量
    one_circle_flag = 1;
    In_Out_Roundabout_I = 0;
    Circle_Flag = 0;
    Circle_Pre_flag = 0;
    LeftCircle_Flag = 0;
    RightCircle_Flag = 0;
    Circle_elect_thres = 930;
    gryo_circle = 315;
    in_circle_thres = 0;
    goto_circle_gyro = 10;
    dir_pwm_max  = 900;
    dir_pwm_min  = -900;
    //发车相关变量
    start_car = 0;
    //舵机相关变量
    kd_elc = 150;
    gyroy_d = 2; 
    direction_pwm = 0;
    //丢线相关变量
    lose_line = 1;
    Lose_Line_Thres = 70;
    REFIND_LINE   = 75;
    //开车变量
    stopflag = 1;
    getinto_garage_flag = 0; 
    //入库相关变量
    into_garage_enable = 0;
    start_line_thres = 300;
    into_garage_flag = 0;
    garage_cnt = 900;
    gyro_garage = 70;
    getinto_speed = 440;
     //电机
    cross_p = 50;
    cross_i = 1.0;
    //加减相关变量
    left_right_parameter = 0;
    
}

/***********************************************
* 获取偏移误差
************************************************/
void Get_OffsetData()
{
  uint8 i;
//  char txt[20];
//  float off_set = 0;
  
  for(i=20; i>0; i--)
  {
    mpu_gyro_y_offset  += ((short)((((uint16)ICM_Read_Byte(ICM_GYRO_YOUTH_REG)) << 8) | ICM_Read_Byte(ICM_GYRO_YOUTL_REG)) /16.4f);
    
    mpu_gyro_z_offset  += ((short)((((uint16)ICM_Read_Byte(ICM_GYRO_ZOUTH_REG)) << 8) | ICM_Read_Byte(ICM_GYRO_ZOUTL_REG)) /16.4f);

  }
                       
  mpu_gyro_y_offset /= 20.0f;
  mpu_gyro_z_offset /= 20.0f;                   //求平均零漂误差
//  sprintf(txt,"offset:%4f",mpu_gyro_z_offset);
//  OLED_P8x16Str(20,0,(uint8_t*)txt);
        
} 

/******************************
* 铃声
*******************************/
void Start_Bell()
{
  BELL_ON;
  delayms(100);
  BELL_OFF;
  delayms(100);
  BELL_ON;
  delayms(100);
  BELL_OFF;
  delayms(100);
}

void IO_Init(void)
{
    GPIO_PinInit(LED_1, GPO, 1);    // 初始化6个LED,0为点亮
    GPIO_PinInit(LED_2, GPO, 1);
    GPIO_PinInit(LED_3, GPO, 1);
    GPIO_PinInit(LED_4, GPO, 1);    
    GPIO_PinInit(LED_5, GPO, 1);    //0为点亮
    GPIO_PinInit(LED_6, GPO, 1);   
    GPIO_PinInit(LED_7, GPO, 0);     //1为点亮
    GPIO_PinInit(LED_8, GPO, 0);
    GPIO_PinInit(LED_9, GPO, 0);
    GPIO_PinInit(LED_10, GPO, 0);
    GPIO_PinInit(LED_11, GPO, 0);
    GPIO_PinInit(LED_12, GPO, 0);
    
    GPIO_PinInit(BM1,GPI_UP,1);        //初始化按键
    GPIO_PinInit(BM2,GPI_UP,1); 
    GPIO_PinInit(BM3,GPI_UP,1); 
    GPIO_PinInit(BM4,GPI_UP,1); 

    GPIO_PinInit(K1,GPI_UP,1); 
    GPIO_PinInit(K2,GPI_UP,1); 
    GPIO_PinInit(K3,GPI_UP,1); 
    
    GPIO_PinInit(PTE26,GPI_DOWN,0);      //BEEP

    //按键矩阵初始化
    GPIO_PinInit(KEY_IN_1, GPI, 0);                
    GPIO_PinInit(KEY_IN_2, GPI, 0);
    GPIO_PinInit(KEY_IN_3, GPI, 0);
    GPIO_PinInit(KEY_IN_4, GPI, 0);
    GPIO_PinInit(KEY_OUT_1, GPO, 0);
    GPIO_PinInit(KEY_OUT_2, GPO, 0);
    GPIO_PinInit(KEY_OUT_3, GPO, 0);
    GPIO_PinInit(KEY_OUT_4, GPO, 0);   
       
}

/* 设置中断优先级组
0: 0个抢占优先级16位个子优先级 
1: 2个抢占优先级 8个子优先级 
2: 4个抢占优先级 4个子优先级 
3: 8个抢占优先级 2个子优先级 
4: 16个抢占优先级 0个子优先级
*/
void InitAll(void)
{
    DisableInterrupts;
    /************************ 时钟初始化  ***********************/
    PLL_Init(PLL180);         //初始化PLL为180M 
    /************************ 配置 K66 的优先级  ***********************/
    NVIC_SetPriorityGrouping(0x07 - 4);            //设置优先级分组,4bit 抢占优先级,没有亚优先级
    NVIC_SetPriority(PIT0_IRQn,0);          //配置优先级
    NVIC_SetPriority(PIT1_IRQn,1);          //配置优先级
    /****************************** 舵机 初始化 ******************************/
    FTM_PwmInit(FTM0, FTM_CH1, S3010_HZ, S3010_MID);
    /****************************** 电机初始化 ******************************/
    //电机初始化
    FTM_PwmInit(FTM3, FTM_CH1, MOTOR_HZ,0);
    FTM_PwmInit(FTM3, FTM_CH2, MOTOR_HZ,0);
    FTM_PwmInit(FTM3, FTM_CH3, MOTOR_HZ,0);
    FTM_PwmInit(FTM3, FTM_CH4, MOTOR_HZ,0);

    /************************ 正交解码  ***********************/
    FTM_ABInit(FTM1);                   //FTM1初始化
    FTM_ABInit(FTM2);                   //FTM2初始化
    /****************************** GPIO 初始化 *******************************/
    IO_Init();
    /****************************** 摄像头 初始化 ****************************/
    CCD_Init();
    /****************************** 开启systick定时器 ****************************/
//    systime.init();
    /****************************** ICM20602 初始化 ****************************/
    ICM20602_Init();
    /******************************ADC初始化*********************************/
    ADC_Init(ADC1);
    /****************************** 串口初始化 ******************************/
    UART_Init(UART4, 115200); 
    /***************************** 定时器初始化 ***************************/
    PIT_Init(PIT0,2);				 //定时时钟后中断2ms
    NVIC_EnableIRQ(PIT0_IRQn);		//打开pit0的中断开关
        
    PIT_Init(PIT1,10);				 //定时时钟后中断5ms
    NVIC_EnableIRQ(PIT1_IRQn);		//打开pit1的中断开关
    /***************************** 测距初始化 ***************************/
    FLASH_Init();
    /**********************LCD初始化****************************/
#ifdef LQ_OLED
     OLED_Init();
     OLED_CLS();
#endif
#ifdef  LQ_TFT1_8 
     TFTSPI_Init(0);
     TFTSPI_CLS(u16WHITE);
#endif
    EnableInterrupts;
}