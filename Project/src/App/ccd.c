/*-----------------------------------------------------------------------------------------------------
【平    台】龙邱K60核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0，龙邱开源代码，仅供参考，后果自负
【最后更新】2019年04月02日
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【编译平台】IAR 8.2
【功    能】测试线性CCD
【注意事项】
-------------------------------------------------------------
CCD接口
              
GND            GND 
VDD            5V
AO             ADC0_SE16 
CLK            PTE2        
SI             PTE3               
-------------------------------------------------------------
-------------------------------------------------------------------------------------------------------*/
#include "include.h"
#include "ccd.h"
#define CCD_A0   ADC1_SE13
#define CCD_CLK  PTD13
#define CCD_SI   PTD14

#define TSL_CLK  PTD13_OUT   
#define TSL_SI   PTD14_OUT  


#define CCD_CLK_PORT   PTD13                     	//CCD CLK引脚定义
#define CCD_SI_PORT    PTD14                     	//CCD SI 引脚定义
#define AD_CHANNEL     ADC1_SE13                 //定义AD采集通道  B0
#define EXPOSURE_TIME  10                       //定义CCD曝光时间 单位MS

#define CCD_CLK(x)     GPIO_PinWrite (CCD_CLK_PORT, x)
#define CCD_SI(x)      GPIO_PinWrite (CCD_SI_PORT,  x)

/*------------------------------------------------------------------------------------------------------
【函    数】CCD_Delayus
【功    能】CCD的延时函数，大概延时，
【参    数】us    ：    延时时间
【返 回 值】无
【实    例】CCD_Delayus(1);
【注意事项】
--------------------------------------------------------------------------------------------------------*/
void CCD_Delayus(uint8_t us)
{
  while(us--)
  {
    for(uint16_t i=0;i<50;i++)
	{
		 asm("nop");  
	}  
  }
}

/*------------------------------------------------------------------------------------------------------
【函    数】CCD_Init
【功    能】初始化CCD
【参    数】无
【返 回 值】无
【实    例】CCD_Init(); 
【注意事项】
--------------------------------------------------------------------------------------------------------*/
void CCD_Init(void) 
{
  GPIO_PinInit(CCD_CLK_PORT,GPO,1);  
  GPIO_PinInit(CCD_SI_PORT, GPO,1);

}

/*------------------------------------------------------------------------------------------------------
【函    数】CCD_Collect
【功    能】CCD采集函数
【参    数】p  ：  存放采集数据的首地址
【返 回 值】无
【实    例】uint16_t ccd_buf[128];
【实    例】CCD_Collect(ccd_buf); 
【注意事项】
--------------------------------------------------------------------------------------------------------*/
void CCD_Collect(uint16_t *p) 
{
//	unsigned char i;
//
//
//	TSL_SI=1;;             //SI  = 1 
//	CCD_Delayus(1);
//	TSL_CLK = 1;           // CLK = 1 
//	CCD_Delayus(1);
//	TSL_SI=0;              // SI  = 0 
//	CCD_Delayus(1);
//
//	for(i=0; i<128; i++) 
//	{
//		CCD_Delayus(1);
//		TSL_CLK = 1;       // CLK = 1 
//		CCD_Delayus(1);
//		temp= ADC_Ave(ADC0, CCD_A0, ADC_12bit, 5);       
//		*p++ = temp;     
//		TSL_CLK = 0;       // CLK = 0 
//	}
//	CCD_Delayus(1);
//	TSL_CLK = 1;           // CLK = 1 
//	CCD_Delayus(1);
//	TSL_CLK = 0;           // CLK = 0 
//  
  
   uint16 i = 0;
    unsigned int  temp = 0;

 
    CCD_CLK(1);
    CCD_SI(0);
    

    CCD_SI(1);
    CCD_CLK(0);
    

    CCD_CLK(1);
    CCD_SI(0);
    
    
    for(i=0;i<128;i++)
    {
        CCD_CLK(0);   
        temp = ADC_Ave(ADC1, AD_CHANNEL, ADC_12bit, 5);   
        *p++ = temp;   
        CCD_CLK(1);
    }
}



