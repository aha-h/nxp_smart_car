/******************** LQ_K60_函数库 v1.0 ********************
 * 文件名           ：FTM.H
 * 功能             ：设置FTM工作模式
 * 备注             ：官方例程上修改
 * 日期             ：2015-10-16
 * 实验平台         ：龙丘 k60DN512Z核心板 
 * 作者             ：龙丘技术 006
 * 淘宝店           ：https://longqiu.taobao.com
 * 龙丘智能车讨论群 ：202949437
*************************************************************/

#ifndef _FTM_H_
#define _FTM_H_

/**********************************  FTM(引脚复用) ***************************************/
//   PWM输出通道    端口          可选范围              建议
#define FTM0_CH0    PTC1        //PTC1、PTA3            PTA3不要用（与JLINK冲突）
#define FTM0_CH1    PTC2        //PTC2、PTA4
#define FTM0_CH2    PTC3        //PTC3、PTA5
#define FTM0_CH3    PTC4        //PTC4、PTA6
#define FTM0_CH4    PTA7        //PTD4、PTA7
#define FTM0_CH5    PTD5        //PTD5、PTA0            PTA0不要用（与JLINK冲突）
#define FTM0_CH6    PTD6        //PTD6、PTA1            PTA1不要用（与JLINK冲突）
#define FTM0_CH7    PTD7        //PTD7、PTA2            PTA2不要用（与JLINK冲突）

//   PWM输出通道    端口          可选范围              建议
#define FTM1_CH0    PTA8       //PTA8、PTA12、PTB0
#define FTM1_CH1    PTA9       //PTA9、PTA13、PTB1

//   PWM输出通道    端口          可选范围              建议
#define FTM2_CH0    PTA10       //PTA10、PTB18
#define FTM2_CH1    PTA11       //PTA11、PTB19

//正交解码模块通道  端口          可选范围              建议
#define FTM1_QDPHA  PTA12       //PTA8、PTA12、PTB0
#define FTM1_QDPHB  PTA13       //PTA9、PTA13、PTB1

#define FTM2_QDPHA  PTB18       //PTA10、PTB18
#define FTM2_QDPHB  PTB19       //PTA11、PTB19

//      模块通道    端口          可选范围
#define FTM3_CH0    PTE5        // PTE5、PTD0
#define FTM3_CH1    PTE6        // PTE6、PTD1
#define FTM3_CH2    PTE7        // PTE7、PTD2
#define FTM3_CH3    PTE8        // PTE8、PTD3
#define FTM3_CH4    PTE9        // PTE9、PTC8
#define FTM3_CH5    PTE10       // PTE10、PTC9
#define FTM3_CH6    PTC10       // PTE11、PTC10
#define FTM3_CH7    PTC11       // PTE12、PTC11


//定义占空比精度，意思即使把一个周期分成多少分，分多越多这样精度越高，设置占空比的时候duty的范围就是0-FTM0_PRECISON
#define FTM_PRECISON   10000
#define FTM0_PRECISON  100000u     //定义占空比精度，100即精度为1%，1000u则精度为0.1%，用于占空比 duty 形参传入，即占空比为 duty/FTM_PRECISON
#define FTM1_PRECISON  1000u     //定义占空比精度，100即精度为1%，1000u则精度为0.1%，用于占空比 duty 形参传入，即占空比为 duty/FTM_PRECISON
#define FTM2_PRECISON  1000u     //定义占空比精度，100即精度为1%，1000u则精度为0.1%，用于占空比 duty 形参传入，即占空比为 duty/FTM_PRECISON
#define FTM3_PRECISON  10000u     //定义占空比精度，100即精度为1%，1000u则精度为0.1%，用于占空比 duty 形参传入，即占空比为 duty/FTM_PRECISON



//定义FTM 通道号
typedef enum
{

    FTM_CH0,
    FTM_CH1,
    FTM_CH2,
    FTM_CH3,
    FTM_CH4,
    FTM_CH5,
    FTM_CH6,
    FTM_CH7,

} FTM_CHn_e;

typedef enum
{
    FTM_Rising,               //上升沿捕捉
    FTM_Falling,              //下降沿捕捉
    FTM_Rising_or_Falling     //跳变沿捕捉
} FTM_Input_cfg;

#define FTM_IRQ_EN(FTMn,CHn)        FTM_CnSC_REG(FTMN[FTMn],CHn) |= FTM_CnSC_CHIE_MASK       //开启 FTMn_CHn 中断
#define FTM_IRQ_DIS(FTMn,CHn)       FTM_CnSC_REG(FTMN[FTMn],CHn) &= ~FTM_CnSC_CHIE_MASK      //关闭 FTMn_CHn 中断
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************************************
                                                    FTM  PWM输出函数
**************************************************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*------------------------------------------------------------------------------------------------------
【函    数】FTM_PwmInit
【功    能】初始化FTM模式PWM通道
【参    数】ftmn  :  模块名FTM0,FTM1，FTM2
【参    数】ch    :  通道号 对应引脚参考ftm.h文件
【参    数】freq  ： 设置PWM的频率
【参    数】duty  ： 设置PWM的占空比
【返 回 值】无
【实    例】FTM_PwmInit(FTM0, FTM_CH0, 10000, 500); //频率10KHZ，占空比为百分之（500/FTM_PRECISON *100）;
【注意事项】
--------------------------------------------------------------------------------------------------------*/
void FTM_PwmInit(FTM_Type * ftmn, FTM_CHn_e ch, uint16_t freq, uint16_t duty);


/*------------------------------------------------------------------------------------------------------
【函    数】FTM_PwmDuty
【功    能】初始化FTM模式PWM通道
【参    数】ftmn  :  模块名FTM0,FTM1，FTM2
【参    数】ch    :  通道号 对应引脚参考ftm.h文件
【参    数】duty  ： 设置PWM的占空比
【返 回 值】无
【实    例】FTM_PwmDuty(FTM0, FTM_CH0, 500); //占空比为百分之（500/FTM_PRECISON *100）;
【注意事项】
--------------------------------------------------------------------------------------------------------*/
void FTM_PwmDuty(FTM_Type * ftmn, FTM_CHn_e ch, u16 duty);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************************************
                                                    FTM  正交解码函数
**************************************************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*------------------------------------------------------------------------------------------------------
【函    数】FTM_ABInit
【功    能】初始化FTM模式正交解码模式
【参    数】ftmn  :  模块名FTM1，FTM2
【返 回 值】无
【实    例】FTM_ABInit(FTM1); //FTM1为正交解码模式  
【注意事项】使用管脚在ftm.h文件查看
--------------------------------------------------------------------------------------------------------*/
void FTM_ABInit(FTM_Type * ftmn);


/*------------------------------------------------------------------------------------------------------
【函    数】FTM_ABGet
【功    能】获取正交解码的计数数值
【参    数】ftmn  :  模块名FTM1，FTM2
【返 回 值】计数值
【实    例】FTM_ABGet(FTM1); //FTM1正交解码值  
【注意事项】使用管脚在ftm.h文件查看
--------------------------------------------------------------------------------------------------------*/
short FTM_ABGet(FTM_Type * ftmn);

uint8_t FTM_GetNum(FTM_Type * ftmn);
#endif  



