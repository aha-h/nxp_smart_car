/******************** LQ_K60_函数库 v1.0 ********************
 * 文件名           ：PLL.c
 * 功能             ：设置工作时钟模式，锁相环，即超频
 * 备注             ：官方例程上修改
 * 日期             ：2015-10-16
 * 实验平台         ：龙丘 k60 vg板 
 * 作者             ：龙丘技术 006
 * 淘宝店           ：https://longqiu.taobao.com
 * 龙丘智能车讨论群 ：202949437
*************************************************************/

#ifndef __PLL_H__
#define __PLL_H__

extern uint8_t core_clk;//单位MHZ
extern uint8_t bus_clk; //单位MHZ

//PLL参数
typedef enum clk_option
{
    PLLUNULL ,
    PLL80    ,
    PLL90    ,
    PLL100   ,
    PLL120   ,
    PLL130   ,
    PLL140   ,
    PLL150   ,
    PLL160   ,
    PLL170   ,
    PLL180   ,
    PLL200   ,
    PLL220   ,
    PLL225   ,
    PLL230   ,
    PLL235   ,
    PLL237_5 , //实际为237.5M
} clk_option;



/*********************** PLL功能函数 **************************/
void PLL_Init(clk_option);     //锁相环初始化




#endif 
