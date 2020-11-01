/*
【平    台】龙邱i.MX RT1052核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0，龙邱开源代码，仅供参考，后果自负
【最后更新】2019年03月13日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
------------------------------------------------
【dev.env.】IAR8.20.1及以上版本
【Target 】 i.MX RT1052
【Crystal】 24.000Mhz
【ARM PLL】 1200MHz
【SYS PLL】 528MHz
【USB PLL】 480MHz
================================================
下载及调试模式：0010
下载      模式：0001
运行      模式：0010
------------------------------------------------
用JLINKV9/OB及以上版本可以下载SDRAM和QSPI FLASH;
用LQDAPLINKV3可以下载SDRAM和QSPI FLASH;
------------------------------------------------
*/
#include "include.h"
#include "MK60_SYSTICK.h"
#define EACH_PER_MS    25   //每隔 25 ms 中断一次  systick 定时器是24位向下计数的定时器  最大装载值16777215 / 600 000 000= 0.2796 最大计时27ms

struct time{
	
    uint32_t fac_us;                  //us分频系数
	uint32_t fac_ms;                  //ms分频系数
	volatile uint32_t millisecond;   //ms
	uint64_t microsecond;             //us
	uint8_t ms_per_tick;              //1ms多少systick计数次数
	
}timer;
/*
【作  者】LQ-005
【功能说明】初始化systick计数器  
【软件版本】V1.0
【最后更新】2019年03月12日 
【返回值】无
【参数值】无     
【例子  】systime.init();                     //开启systick定时器 
*/
static void systime_init()
{
	timer.fac_us = core_clk;
	timer.fac_ms = core_clk * 1000;
	timer.ms_per_tick = EACH_PER_MS;
    timer.millisecond = 100;
	SysTick_Config(timer.fac_ms * timer.ms_per_tick );   //开启systick中断
}
/*       
【作  者】LQ-005
【功能说明】systick  中断服务函数 
【软件版本】V1.0
【最后更新】2019年03月12日 
【返回值】无
【参数值】无     
【例子  】
       */
void SysTick_Handler(void)
{
	timer.millisecond += timer.ms_per_tick;
}

/*        
【作  者】LQ-005
【功能说明】systick  获取当前ms值 
【软件版本】V1.0
【最后更新】2019年03月12日 
【返回值】当前ms值
【参数值】无     
【例子  】systime.get_time_ms();         //计时功能  得到当前时间 ms   
       */
static uint32_t get_current_time_ms(void)
{
    uint32_t val = SysTick->VAL;
	return timer.millisecond -  val/timer.fac_ms;
}
/*     
【作  者】LQ-005
【功能说明】systick  获取当前ms值 
【软件版本】V1.0
【最后更新】2019年03月12日 
【返回值】当前ms值
【参数值】无     
【例子  】systime.get_time_us();         //计时功能  得到当前时间 us   
       */
static uint64_t get_current_time_us(void)
{
    uint32_t val = SysTick->VAL;
	return (uint64_t)((uint64_t)(timer.millisecond * 1000) -  val / timer.fac_us);
}

/*     
【作  者】LQ-005
【功能说明】systick  延时us 
【软件版本】V1.0
【最后更新】2019年03月12日 
【返回值】无
【参数值】无     
【例子  】systime.delay_us(1000000);         //延时功能  延时1s
       */
static void delay_us( uint32_t us)     
{
    uint64_t now = systime.get_time_us();
	uint64_t end_time = now + us - 1;
	while( systime.get_time_us() < end_time)
    {
        ;
    }
}

/*     
【作  者】LQ-005
【功能说明】systick  延时ms
【软件版本】V1.0
【最后更新】2019年03月12日 
【返回值】无
【参数值】无     
【例子  】systime.delay_ms(1000);         //延时功能 延时1s 
*/
static void delay_ms( uint16_t ms) 
{
	systime.delay_us(ms * 1000);
}

systime_t  systime = 
{
	systime_init,
	get_current_time_us,
	get_current_time_ms,
	delay_us,
	delay_ms
};



