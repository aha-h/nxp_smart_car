/*-----------------------------------------------------------------------------------------------------
【平    台】龙邱K60核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0，龙邱开源代码，仅供参考，后果自负
【最后更新】2019年04月02日
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【编译平台】IAR 8.2
【功    能】
【注意事项】ICM20602可以看作MPU6050的升级版，性能更加稳定并且兼容6050
-------------------------------------------------------------*/
#include "ICM20602.h"


//void ICM20602(void)
//{
//    char  txt[30];
   
//    if(ICM20602_Init())
//    {
////        OLED_P8x16Str(15,2,"ICM20602 Test Fail");
////        while(1);
//    }
        
//    while(1)
//    {
      
//        sprintf((char*)txt,"ax:%06d",aacx);
//        OLED_P6x8Str(0,2,(u8*)txt);
//        sprintf((char*)txt,"ay:%06d",aacy);
//        OLED_P6x8Str(0,3,(u8*)txt);
//        sprintf((char*)txt,"az:%06d",aacz);
//        OLED_P6x8Str(0,4,(u8*)txt);
//        sprintf((char*)txt,"gx:%06d",gyrox);
//        OLED_P6x8Str(0,5,(u8*)txt);
//        sprintf((char*)txt,"gy:%06d",gyroy);
//        OLED_P6x8Str(0,6,(u8*)txt);
//        sprintf((char*)txt,"gz:%06d",gyroz);
//        OLED_P6x8Str(0,7,(u8*)txt);

//        printf("\r\nAX: %d  ",aacx); 
//        printf("\r\nAY: %d  ",aacy);
//        printf("\r\nAZ: %d  ",aacz); 
//        printf("\r\nGX: %d  ",gyrox);
//        printf("\r\nGY: %d  ",gyroy); 
//        printf("\r\nGZ: %d  ",gyroz);
//        delayms(100);
//        printf("*********************");
//    }
    
//}

/*
【作  者】LQ-005
【功能说明】初始化 ICM20602
【软件版本】V1.0
【最后更新】2019年03月13日 
【函数名】
【返回值】无 0：初始化成功   1：失败
【参数值】无 
*/
void ICM20602_Init(void)
{
    uint8_t res;
    SPI_Init(SPI1, SPI_PCS0, MASTER, 1, 10);            //ICM20602 支持10M SPI
    delayms(100);
    res=ICM_Read_Byte(WHO_AM_I);                        //读取ICM20602的ID
    if(res!=ICM20602_ID)                                 //器件ID正确
    {
//        printf("ID=%#X\r\n",res);
//        printf("ICM20602 is fail!\n");
        GPIO_PinWrite(LED_8,0);
        while(1);
    }
    GPIO_PinWrite(LED_8,1);
    res = 0;
    ICM_Write_Byte(ICM_PWR_MGMT1_REG,0X80);//复位
    delayms(100);  //延时100ms
    ICM_Write_Byte(ICM_PWR_MGMT1_REG,0X00);//唤醒
    delayms(100);  //延时100ms

    ICM_Set_Gyro_Fsr(3);			       //陀螺仪传感器,±2000dps   
    ICM_Set_Accel_Fsr(0x10);				   //加速度传感器,±8g
    ICM_Set_Rate(1000);					   //设置采样率1000Hz
    ICM_Write_Byte(ICM_CFG_REG,0x02);      //设置数字低通滤波器   98hz
    ICM_Write_Byte(ICM_INT_EN_REG,0X00);   //关闭所有中断
    ICM_Write_Byte(ICM_USER_CTRL_REG,0X00);//I2C主模式关闭
    ICM_Write_Byte(ICM_PWR_MGMT1_REG,0X01);//设置CLKSEL,PLL X轴为参考
    ICM_Write_Byte(ICM_PWR_MGMT2_REG,0X00);//加速度与陀螺仪都工作

}

//设置MPU6050陀螺仪传感器满量程范围
//fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//    其他,设置失败 
void ICM_Set_Gyro_Fsr(uint8_t fsr)
{
	ICM_Write_Byte(ICM_GYRO_CFG_REG,fsr<<3);//设置陀螺仪满量程范围  
}
//设置MPU6050加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//    其他,设置失败 
void ICM_Set_Accel_Fsr(uint8_t fsr)
{
	ICM_Write_Byte(ICM_ACCEL_CFG_REG,fsr<<3);//设置加速度传感器满量程范围  
}

//设置MPU6050的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败 
void ICM_Set_LPF(uint16_t lpf)
{
	uint8_t data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	ICM_Write_Byte(ICM_CFG_REG,data);//设置数字低通滤波器  
}

//设置MPU6050的采样率(假定Fs=1KHz)
//rate:4~1000(Hz)
//返回值:0,设置成功
//    其他,设置失败 
void ICM_Set_Rate(uint16_t rate)
{
	uint8_t data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	ICM_Write_Byte(ICM_SAMPLE_RATE_REG,data);	//设置数字低通滤波器
 	ICM_Set_LPF(rate/2);	//自动设置LPF为采样率的一半
}

//得到温度值
//返回值:温度值
short ICM_Get_Temperature(void)
{
    uint8_t buf[3]; 
    short raw;
	float temp;
	ICM_Read_Len(ICM_TEMP_OUTH_REG,2,buf); 
    raw=((uint16_t)buf[0]<<8)|buf[1];  
    temp=21+((double)raw)/333.87;  
    return (short)temp;
}
//得到陀螺仪值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
void ICM_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    uint8_t buf[7]; 
	ICM_Read_Len(ICM_GYRO_XOUTH_REG,6,buf);
    
    *gx=((uint16_t)buf[1]<<8)|buf[2];  
    *gy=((uint16_t)buf[3]<<8)|buf[4];  
    *gz=((uint16_t)buf[5]<<8)|buf[6];
    
}
//得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
void ICM_Get_Accelerometer(short *ax,short *ay,short *az)
{
    uint8_t buf[7];  
	ICM_Read_Len(ICM_ACCEL_XOUTH_REG,6,buf);
    
    *ax=((uint16_t)buf[1]<<8)|buf[2];  
    *ay=((uint16_t)buf[3]<<8)|buf[4];  
    *az=((uint16_t)buf[5]<<8)|buf[6];
    
}

//得到加计值、温度值、角速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
void ICM_Get_Raw_data(short *ax,short *ay,short *az,short *gx,short *gy,short *gz)
{
    uint8_t buf[15];  
	ICM_Read_Len(ICM_ACCEL_XOUTH_REG,14,buf);
    
//    *ax=((uint16_t)buf[1]<<8)|buf[2];  
//    *ay=((uint16_t)buf[3]<<8)|buf[4];  
//    *az=((uint16_t)buf[5]<<8)|buf[6];
//    *gx=((uint16_t)buf[9]<<8)|buf[10];  
//    *gy=((uint16_t)buf[11]<<8)|buf[12];  
//    *gz=((uint16_t)buf[13]<<8)|buf[14];
//    short temp_gx;
    short temp_gy ;
    short temp_gz;
//    short temp_ax ;
//    short temp_ay ;
//    short temp_az ;

    
//    temp_ax = (short)((uint16_t)buf[1]<<8)|buf[2];  
//    temp_ay = (short)((uint16_t)buf[3]<<8)|buf[4];  
//    temp_az = (short)((uint16_t)buf[5]<<8)|buf[6];  
//    temp_gx = (short)((uint16_t)buf[9]<<8)|buf[10];  
    temp_gy = (short)((uint16_t)buf[11]<<8)|buf[12];  
    temp_gz = (short)((uint16_t)buf[13]<<8)|buf[14];  
      
    
//    *ax=(temp_ax * 0.0244;  
//    *ay=((temp_ay - mpu_acce_y_offset) * 0.0244);  
//    *az=temp_az * 0.0244;
//    *gx=(temp_gx - mpu_gyro_x_offset) * 0.061 ;  
    *gy=(temp_gy - mpu_gyro_y_offset) * 0.061; 
    *gz=(temp_gz - mpu_gyro_z_offset) * 0.061 ;
	
}

/**
  * @brief    SPI 连续读
  *
  * @param    reg   读的寄存器地址
  * @param    len   长度
  * @param    buf   存放读出数据 的地址
  *
  * @return   
  *
  * @note     
  * 
  * @example  
  *
  * @date     2019/5/27 星期一
  */

void ICM_Read_Len(uint8_t reg,uint8_t len,uint8_t *buf)
{   
    buf[0] = reg | 0x80;
    /* 写入要读的寄存器地址 */
    SPI_RadeWrite(SPI1, SPI_PCS0, buf, buf, len + 1);

}


/**
  * @brief    SPI 写
  *
  * @param    reg   写的寄存器地址
  * @param    value 要写的值
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/5/27 星期一
  */
void ICM_Write_Byte(uint8_t reg,uint8_t value)
{
    uint8_t buff[2];

    buff[0] = reg;          //先发送寄存器
    buff[1] = value;        //再发送数据

    SPI_RadeWrite(SPI1, SPI_PCS0, buff, buff, 2); //发送buff里数据，并采集到 buff里
}

/**
  * @brief    SPI 读
  *
  * @param    reg   读的寄存器地址
  *
  * @return   读出的数据
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/5/27 星期一
  */
uint8_t ICM_Read_Byte(uint8_t reg)
{
    uint8 buff[2];
    buff[0] = reg | 0x80;          //先发送寄存器
    
    SPI_RadeWrite(SPI1, SPI_PCS0, buff, buff,  2);
    
    return buff[1];
}

