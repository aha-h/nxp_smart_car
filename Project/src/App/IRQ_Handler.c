/*---------------------------------------------------------------
【平    台】龙邱K60核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0，龙邱开源代码，仅供参考，后果自负
【最后更新】2019年04月02日
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【编译平台】IAR 8.2
【功    能】中断服务函数
【注意事项】中断服务函数的名称在startup_MK60D10.s 中
----------------------------------------------------------------*/
#include "include.h"

int out_ring_cnt = 0;
int out_ring_flag = 0;

/////////////////////////////////////////////////////////////////
///////////////PIT中断服务函数///////////////////////////////////
/////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------
【函    数】PIT0_Interrupt
【功    能】PIT0的中断服务函数
【参    数】无
【返 回 值】无
【注意事项】注意进入后要清除中断标志位
----------------------------------------------------------------*/
short aacx,aacy,aacz;	        //加速度传感器原始数据
short gyrox,gyroy,gyroz;        //陀螺仪原始数据 //为了减少代码的修改量，因此交换gyroz和gyroy的位置
short gyro_temp;               
extern u8 s_flag;
short gyrosensor;
short accsenser;
int cross_remain = 0;
int out_garbage;
int gryo_circle = 180;
int gyro_garage = 80;
int garage_cnt = 600;
int into_garage_flag;
uint8_t ramp_flag = 0;
/******/

uint8_t test_startline = 1;
uint8_t test_roadpart = 1;
uint8_t test_circle = 1;

/******/

void PIT0_IRQHandler()
{
    PIT_Flag_Clear(PIT0);       //清中断标志位
    static int s_cnt = 0;
    static int out_garage_cnt;
    static int into_cnt;
    static int _out_garage_cnt = 0;

    if(++s_cnt == 150) {        //屏幕数据刷新速率        
        s_flag = 1;
        s_cnt = 0;
    }

    duty_motor = speed_duty[Speed_Set];
    Steer_Scan();//当测试赛道电感最大最小值时使用
    ICM_Get_Raw_data(&aacx,&aacy,&aacz,&gyrox,&gyroy,&gyroz);	//得到加速度传感器数据 
    gyrosensor = gyroz;
    accsenser = gyroy;
    KeyScan();
    Get_Elect_Data();  
    Get_Speed();
    
    if(abs(accsenser) > 0){
        ramp_flag = 1;
    }else{
        ramp_flag = 0;
    }
    

      if(direction_into_garage == 1){//左出车库，右进环岛
              RightCircle_Flag = 1;
              LeftCircle_Flag = 0;
      }else{
              LeftCircle_Flag = 1;
              RightCircle_Flag = 0;
      }

    if( test_roadpart == 0 ){
        if(stopflag == 0)//发车
        {
            if(start_car == 0){//出库
                    if(encoder_cnt > garage_cnt){
                        encoder_cnt = 0;
                        start_car = 1;
                    }
            }else{
                if(getinto_garage_flag){
                    In_Out_Roundabout_I += gyrosensor * 0.002;  
                    if(fabs(In_Out_Roundabout_I) >= gyro_garage){//车子入库，参数全部清零
                        stopflag = 1;
                    }
                }
                
                if(Circle_Flag == 1)      
                {
                      In_Out_Roundabout_I += gyrosensor * 0.002; 
                      if (fabs(In_Out_Roundabout_I) >= gryo_circle) {//出环并且将标志位清零
                            one_circle_flag = 0;//环岛判断
                            Circle_Flag = 0;
                            Circle_Pre_flag = 0;
                            In_Out_Roundabout_I = 0;
                            LeftCircle_Flag = 0;
                            RightCircle_Flag = 0;
                      }
                }
                
                Elect_Control_Out(); 
                
                
            }
        }else{
              if(getinto_garage_flag){
                if((L_encoder == 0 && R_encoder == 0)){
                        getinto_garage_flag = 0;
                        In_Out_Roundabout_I = 0;
                        start_car = 0;
                }
              }
             direction_pwm = 0;
             FTM_PwmDuty(S3010_FTM, S3010_CH, S3010_MID + direction_pwm);    //修改舵机占空比
        }
              
    }else{
        
            if(!stopflag){
                if(out_ring_cnt++ > 2300){
                    out_ring_cnt = 0;
                    stopflag = 1;
                }
            }

            if(getinto_garage_flag){
                In_Out_Roundabout_I += gyrosensor * 0.002;  
                if(fabs(In_Out_Roundabout_I) >= gyro_garage){//车子入库，参数全部清零
                    stopflag = 1;
                }
                
                if(stopflag){
                    if((L_encoder == 0 && R_encoder == 0)){
                        getinto_garage_flag = 0;
                        In_Out_Roundabout_I = 0;
                        start_car = 0;
                    }
                }
            }
          
            if(Circle_Flag == 1)      
            {
                  In_Out_Roundabout_I += gyrosensor * 0.002; 
                  if (fabs(In_Out_Roundabout_I) >= gryo_circle) {//出环并且将标志位清零
                        one_circle_flag = 0;//环岛判断
                        Circle_Flag = 0;
                        Circle_Pre_flag = 0;
                        In_Out_Roundabout_I = 0;
                        LeftCircle_Flag = 0;
                        RightCircle_Flag = 0;
                  }
            }
        
            Elect_Control_Out(); 

    }
    
    
}

    /*用户添加所需代码*/  

/*---------------------------------------------------------------
【函    数】PIT1_Interrupt
【功    能】PIT1的中断服务函数
【参    数】无
【返 回 值】无
【注意事项】注意进入后要清除中断标志位
----------------------------------------------------------------*/
extern u16 temperature ;
extern u16 battery;
void PIT1_IRQHandler()
{
    PIT_Flag_Clear(PIT1);       //清中断标志位
        

    GetImageArray();
    Adjust_Speed();//变速控制计算
    CSControl();//差速控制计算 
    Motor_Out();
   
    /*用户添加所需代码*/
}

/////////////////////////////////////////////////////////////////
///////////////GPIO中断服务函数//////////////////////////////////
/////////////////////////////////////////////////////////////////

/*---------------------------------------------------------------
【函    数】DMA_CH4_Handler
【功    能】DMA通道4的中断服务函数
【参    数】无
【返 回 值】无
【注意事项】注意进入后要清除中断标志位
----------------------------------------------------------------*/
void UART4_RX_TX_IRQHandler(void)
{
    if(UART4_S1 & UART_S1_RDRF_MASK)                                     //接收数据寄存器满
    {

    }

}

u8 ch1 = 0;
u8 ch2 = 0;
u8 ch3 = 0;
u8 ch4 = 0;
u8 ch5 = 0;
u8 ch6 = 0;
u8 ch7 = 0;
u8 ch8 = 0;

void PORTE_IRQHandler()
{
    /* 中断使用的引脚号，这里只写了管脚20和管脚21和22，使用其他管脚可以自行添加 */
    int n;  
    n=10;
    if((PORTE_ISFR & (1<<n)))
    {
        ch5 = 1;
    }
    n=11;
    if((PORTE_ISFR & (1<<n)))
    {
        ch6 = 1;
    }

    PORTE_ISFR = 0xffffffff;
}

void PORTB_IRQHandler()
{
    /* 中断使用的引脚号，这里只写了管脚20和管脚21和22，使用其他管脚可以自行添加 */
    int n;  
    
    n=7;
    if((PORTB_ISFR & (1<<n)))
    {
        /* 用户自行添加中断内程序 */
        ch7 = 1;
    } 

    PORTB_ISFR = 0xffffffff;
}

void PORTC_IRQHandler()
{
    /* 中断使用的引脚号，这里只写了管脚20和管脚21和22，使用其他管脚可以自行添加 */
    int n;  
    
    n=10;
    if((PORTC_ISFR & (1<<n)))
    {
        /* 用户自行添加中断内程序 */
        ch8 = 1;
    } 

    PORTC_ISFR = 0xffffffff;
}

void PORTA_IRQHandler()
{
    /* 中断使用的引脚号，这里只写了管脚20和管脚21和22，使用其他管脚可以自行添加 */
    int n;  
    
    n=8;
    if((PORTA_ISFR & (1<<n)))
    {
        /* 用户自行添加中断内程序 */
        ch1 = 1;
    } 
    n=9;
    if((PORTA_ISFR & (1<<n)))
    {
        /* 用户自行添加中断内程序 */
        ch2 = 1;
    } 
    n=10;
    if((PORTA_ISFR & (1<<n)))
    {
        /* 用户自行添加中断内程序 */
        ch3 = 1;
    } 
    n=11;
    if((PORTA_ISFR & (1<<n)))
    {
        /* 用户自行添加中断内程序 */
        ch4 = 1;
    } 

    PORTA_ISFR = 0xffffffff;
}

/*---------------------------------------------------------------
【函    数】PIT2_Interrupt
【功    能】PIT2的中断服务函数
【参    数】无
【返 回 值】无
【注意事项】注意进入后要清除中断标志位
----------------------------------------------------------------*/
extern u8 arry_cnt;
void PIT2_IRQHandler()
{
    PIT_Flag_Clear(PIT2);       //清中断标志位
//    char txt[20];
//    sprintf(txt,"arry_cnt:%d",arry_cnt);
//    TFTSPI_P6X8Str(0,11,txt,u16BLACK,u16WHITE);
//    arry_cnt = 0;
    /*用户添加所需代码*/
}
/*---------------------------------------------------------------
【函    数】PIT3_Interrupt
【功    能】PIT3的中断服务函数
【参    数】无
【返 回 值】无
【注意事项】注意进入后要清除中断标志位
----------------------------------------------------------------*/
void PIT3_IRQHandler()
{
    PIT_Flag_Clear(PIT3);       //清中断标志位
    /*用户添加所需代码*/
}

/////////////////////////////////////////////////////////////////
///////////////串口中断服务函数/////////////////////////////////
/////////////////////////////////////////////////////////////////

