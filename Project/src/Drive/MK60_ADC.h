/******************** LQ_K60_函数库 v1.0 ********************
 * 文件名           ：ADC.H
 * 功能             ：设置ADC工作模式
 * 备注             ：官方例程上修改
 * 日期             ：2015-10-16
 * 实验平台         ：龙丘 k60DN512Z核心板 
 * 作者             ：龙丘技术 006
 * 淘宝店           ：https://longqiu.taobao.com
 * 龙丘智能车讨论群 ：202949437
*************************************************************/
#ifndef __ADC_H__
#define __ADC_H__ 
/*
                        ADC对应引脚
----------------ADC0--------------------ADC1---------------

      ADC0_SE8-----PTB0         ADC1_SE4a-----PTE0 
      ADC0_SE9-----PTB1         ADC1_SE5a-----PTE1 
      ADC0_SE10----PTA7         ADC1_SE6a-----PTE2 
      ADC0_SE11----PTA8         ADC1_SE7a-----PTE3 
      ADC0_SE12----PTB2         ADC1_SE8------PTB0
      ADC0_SE13----PTB3         ADC1_SE9------PTB1
      ADC0_SE14----PTC0         ADC1_SE10-----PTB4
      ADC0_SE15----PTC1         ADC1_SE11-----PTB5
                                ADC1_SE12-----PTB6
      ADC0_SE16----ADC0_SE16    ADC1_SE13-----PTB7
                                ADC1_SE14-----PTB10
      ADC0_SE17----PTE24        ADC1_SE15-----PTB11
      ADC0_SE18----PTBE25       ADC1_SE16-----ADC1_SE16
                                ADC1_SE17-----PTA17

      ADC0_DM0-----ADC0_DM0     ADC1_DM0------ADC1_DM0
      ADC0_DM1-----ADC0_DM1     ADC1_DM1------ADC1_DM1

*/

typedef enum
{
    // ---------------------------------ADC0-------------------------
    ADC0_DP0  =0,
    ADC0_DP1  =1,
    PGA0_DP   =2,        //不支持ADC，需要配置 ADC PGA register 设置放大增益
    ADC0_DP3  =3,
    //软件触发不支持B通道
    ADC0_SE4b =4,      // PTC2     不支持软件ADC
    ADC0_SE5b =5,      // PTD1     不支持软件ADC
    ADC0_SE6b =6,      // PTD5     不支持软件ADC
    ADC0_SE7b =7,      // PTD6     不支持软件ADC

    ADC0_SE8  =8,       // PTB0
    ADC0_SE9  =9,       // PTB1
    ADC0_SE10 =10,      // PTA7
    ADC0_SE11 =11,      // PTA8
    ADC0_SE12 =12,      // PTB2
    ADC0_SE13 =13,      // PTB3
    ADC0_SE14 =14,      // PTC0
    ADC0_SE15 =15,      // PTC1
    ADC0_SE16 =16,      // ADC0_SE16
    ADC0_SE17 =17,      // PTE24
    ADC0_SE18 =18,      // PTE25
    ADC0_DM0  =19,      // ADC0_DM0
    ADC0_DM1  =20,      // ADC0_DM1
    ADC0_SE21 =21,      // 保留
    ADC0_SE22 =22,      // 保留
    DAC0_OUT,       // DAC0输出 不支持ADC
    RES2,           // 保留
    RES3,           // 保留
    Temp0_Sensor,   // Temperature Sensor,内部温度测量，可用ADC函数
    Bandgap0,       // 温度补偿结构带隙基准源   不支持ADC
    RES4,           // 保留
    VREFH0,         // 参考高电压,可用ADC函数 ,结果恒为 2^n-1
    VREFL0,         // 参考低电压,可用ADC函数 ,结果恒为 0
    Module0_Dis,    // 不支持 ADC

    // ---------------------------------ADC1-------------------------
    ADC1_DP0  =0,
    ADC1_DP1  =1,
    PGA1_DP   =2,        // 不支持 ADC
    ADC1_DP3  =3,
    ADC1_SE4a =4,      // PTE0
    ADC1_SE5a =5,      // PTE1
    ADC1_SE6a =6,      // PTE2
    ADC1_SE7a =7,      // PTE3

    ADC1_SE4b = ADC1_SE4a,  // PTC8    B通道 不支持软件ADC ,传递 ADC1_SE4b 到软件ADC函数里，会当作 ADC1_SE4a 处理。
    ADC1_SE5b = ADC1_SE5a,  // PTC9    B通道 不支持软件ADC
    ADC1_SE6b = ADC1_SE6a,  // PTC10   B通道 不支持软件ADC
    ADC1_SE7b = ADC1_SE7a,  // PTC11   B通道 不支持软件ADC

    ADC1_SE8  =8,       // PTB0
    ADC1_SE9  =9,       // PTB1
    ADC1_SE10 =10,      // PTB4
    ADC1_SE11 =11,      // PTB5
    ADC1_SE12 =12,      // PTB6
    ADC1_SE13 =13,      // PTB7
    ADC1_SE14 =14,      // PTB10
    ADC1_SE15 =15,      // PTB11
    ADC1_SE16 =16,      // ADC1_SE16
    ADC1_SE17 =17,      // PTA17
    VREF_OUTPUT,    // VREF Output
    ADC1_DM0  =19,       // ADC1_DM0
    ADC1_DM1  =20,       // ADC1_DM1
    RES5,           //保留
    RES6,
    DAC1_OUT,
    RES7,           //保留
    RES8,
    Temp1_Sensor,
    Bandgap1,       // 温度补偿结构带隙基准源   不支持ADC
    RES9,
    VREFH1,         // 参考高电压,可用ADC函数 ,结果恒为 2^n-1
    VREFL1,         // 参考低电压,可用ADC函数 ,结果恒为 0
    Module1_Dis,    // 不支持 ADC

} ADCn_Ch_e;



//精度位数
typedef enum ADC_nbit
{
    ADC_8bit   = 0x00,
    ADC_10bit  = 0x02,
    ADC_12bit  = 0x01,
    ADC_16bit  = 0x03
} ADC_nbit;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************************************
                                                    ADC模块函数
**************************************************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*------------------------------------------------------------------------------------------------------
【函    数】ADC_Init
【功    能】初始化ADC模式
【参    数】ADCn_e： 要初始化的ADC模块， ADC0  ADC1
【返 回 值】无
【实    例】ADC_Init(ADC0); //初始化ADC0模块
【注意事项】
--------------------------------------------------------------------------------------------------------*/
void ADC_Init(ADC_Type * adc_n);



/*------------------------------------------------------------------------------------------------------
【函    数】ADC_Start
【功    能】启动ADC转换
【参    数】adc_n ：  模块名ADC0或ADC1 
【参    数】adc_ch：  ADC通道编号 
【参    数】bit   ：  精度选择ADC_8bit，ADC_12bit，ADC_10bit，ADC_16bit
【返 回 值】无
【实    例】ADC_Init(ADC0,ADC1_SE8,ADC_12bit); //通道编号为 ADC1_SE8的引脚
【注意事项】使用前应初始化对应ADC模块
--------------------------------------------------------------------------------------------------------*/
void ADC_Start(ADC_Type * adc_n, ADCn_Ch_e adc_ch, ADC_nbit bit);



/*------------------------------------------------------------------------------------------------------
【函    数】ADC_Once
【功    能】启动ADC转换 获取一次ADC转换值
【参    数】adc_n ：  模块名ADC0或ADC1 
【参    数】adc_ch：  ADC通道编号 
【参    数】bit   ：  精度选择ADC_8bit，ADC_12bit，ADC_10bit，ADC_16bit
【返 回 值】无
【实    例】ADC_Once(ADC0,ADC1_SE8,ADC_12bit) 通道编号为 ADC1_SE8的引脚，进行一次ADC转换。
【注意事项】
--------------------------------------------------------------------------------------------------------*/
uint16_t ADC_Once(ADC_Type * adc_n, ADCn_Ch_e adc_ch, ADC_nbit bit);



/*------------------------------------------------------------------------------------------------------
【函    数】ADC_Mid
【功    能】进行ADC3次转换取中间值
【参    数】adc_n ：  模块名ADC0或ADC1 
【参    数】adc_ch：  ADC通道编号 
【参    数】bit   ：  精度选择ADC_8bit，ADC_12bit，ADC_10bit，ADC_16bit
【返 回 值】中值
【实    例】ADC_Mid(ADC0,ADC1_SE8,ADC_12bit) 通道编号为 ADC1_SE8的引脚，进行3次ADC转换，选取中间值返回。
【注意事项】
--------------------------------------------------------------------------------------------------------*/
uint16_t ADC_Mid(ADC_Type * adc_n,  ADCn_Ch_e adc_ch, ADC_nbit bit);



/*------------------------------------------------------------------------------------------------------
【函    数】ADC_Ave
【功    能】进行ADC3次转换取中间值
【参    数】adc_n ：  模块名ADC0或ADC1 
【参    数】adc_ch：  ADC通道编号 
【参    数】bit   ：  精度选择ADC_8bit，ADC_12bit，ADC_10bit，ADC_16bit
【参    数】N     :   均值滤波次数（1~65535）
【返 回 值】平均值
【实    例】ADC_Ave(ADC0,ADC1_SE8,ADC_12bit,100) 通道编号为 ADC1_SE8的引脚，进行100次ADC转换，计算出平均值返回。   
【注意事项】
--------------------------------------------------------------------------------------------------------*/
uint16_t ADC_Ave(ADC_Type * adc_n,  ADCn_Ch_e adc_ch, ADC_nbit bit, uint16_t N);




#endif /* __MK60_ADC16_H__ */
