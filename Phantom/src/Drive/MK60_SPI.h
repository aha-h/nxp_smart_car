#ifndef __MK60_SPI_H
#define __MK60_SPI_H

/**********************************  SPI   ***************************************/
//PCS接口，不用的时候需要注释，就不会进行初始化对应的管脚
//      模块通道    端口          可选范围                  建议

#define SPI0_SCK_PIN    PTA15       // PTA15、PTC5、PTD1        全部都是 ALT2
#define SPI0_SOUT_PIN   PTA16       // PTA16、PTC6、PTD2        全部都是 ALT2
#define SPI0_SIN_PIN    PTA17       // PTA17、PTC7、PTD3        全部都是 ALT2

#define SPI0_PCS0_PIN   PTA14       // PTA14、PTC4、PTD0、      全部都是 ALT2
#define SPI0_PCS1_PIN   PTC3        // PTC3、PTD4               全部都是 ALT2
#define SPI0_PCS2_PIN   PTC2        // PTC2、PTD5               全部都是 ALT2
#define SPI0_PCS3_PIN   PTC1        // PTC1、PTD6               全部都是 ALT2
#define SPI0_PCS4_PIN   PTC0        // PTC0、                   全部都是 ALT2
#define SPI0_PCS5_PIN   PTB23       // PTB23                    ALT3

#define SPI1_SCK_PIN    PTE2        // PTE2、PTB11、            全部都是 ALT2
#define SPI1_SOUT_PIN   PTB16        // PTE1、PTB16、            全部都是 ALT2
#define SPI1_SIN_PIN    PTE3        // PTE3、PTB17、            全部都是 ALT2

#define SPI1_PCS0_PIN   PTE4       // PTE4、PTB10、            全部都是 ALT2
#define SPI1_PCS1_PIN   PTE0        // PTE0、PTB9、             全部都是 ALT2
#define SPI1_PCS2_PIN   PTE5        // PTE5、                   全部都是 ALT2
#define SPI1_PCS3_PIN   PTE6        // PTE6、                   全部都是 ALT2


#define SPI2_SCK_PIN    PTB21       // PTB21、PTD12             全部都是 ALT2
#define SPI2_SOUT_PIN   PTB22       // PTB22、PTD13             全部都是 ALT2
#define SPI2_SIN_PIN    PTB23       // PTB23、PTD14             全部都是 ALT2
#define SPI2_PCS0_PIN   PTB20       // PTB20、PTD11             全部都是 ALT2
#define SPI2_PCS1_PIN   PTD15       // PTD15                    全部都是 ALT2
/*! 枚举PORT 配置 */
typedef enum
{
    //中断方式和DMA请求方式，两者只能选其中一种（可以不选）
    //中断方式选择
    IRQ_ZERO     = 0x08 << PORT_PCR_IRQC_SHIFT,   //低电平触发
    IRQ_RISING   = 0x09 << PORT_PCR_IRQC_SHIFT,   //上升沿触发
    IRQ_FALLING  = 0x0A << PORT_PCR_IRQC_SHIFT,   //下降沿触发
    IRQ_EITHER   = 0x0B << PORT_PCR_IRQC_SHIFT,   //跳变沿触发
    IRQ_ONE      = 0x0C << PORT_PCR_IRQC_SHIFT,   //高电平触发

    //DMA请求选择
    DMA_RISING   = 0x01 << PORT_PCR_IRQC_SHIFT,   //上升沿触发
    DMA_FALLING  = 0x02 << PORT_PCR_IRQC_SHIFT,   //下降沿触发
    DMA_EITHER   = 0x03 << PORT_PCR_IRQC_SHIFT,   //跳变沿触发


    HDS          = 0x01 << PORT_PCR_DSE_SHIFT,    //输出高驱动能力
    ODO          = 0x01 << PORT_PCR_ODE_SHIFT,    //漏极输出
    PF           = 0x01 << PORT_PCR_PFE_SHIFT,    //带无源滤波器
    SSR          = 0x01 << PORT_PCR_SRE_SHIFT,    //输出慢变化率          Slow slew rate

    //下拉上拉选择
    PULLDOWN     = 0x02 << PORT_PCR_PS_SHIFT,     //下拉
    PULLUP       = 0x03 << PORT_PCR_PS_SHIFT,     //上拉

    //功能复用选择(如果不需要改变功能复用选择，保留原先的功能复用，直接选择ALT0 )
    //需要查 K60 Signal Multiplexing and Pin Assignments
    ALT0         = 0x00 << PORT_PCR_MUX_SHIFT,
    ALT1         = 0x01 << PORT_PCR_MUX_SHIFT,    //GPIO
    ALT2         = 0x02 << PORT_PCR_MUX_SHIFT,
    ALT3         = 0x03 << PORT_PCR_MUX_SHIFT,
    ALT4         = 0x04 << PORT_PCR_MUX_SHIFT,
    ALT5         = 0x05 << PORT_PCR_MUX_SHIFT,
    ALT6         = 0x06 << PORT_PCR_MUX_SHIFT,
    ALT7         = 0x07 << PORT_PCR_MUX_SHIFT,
} port_cfg;
/**
 *  @brief 主从机模式
 */
typedef enum
{
    MASTER,    //主机模式
    SLAVE      //从机模式
} SPI_CFG;



/**
 *  @brief SPI模块片选号
 */
typedef enum
{
    SPI_PCS0 = 1 << 0,
    SPI_PCS1 = 1 << 1,
    SPI_PCS2 = 1 << 2,
    SPI_PCS3 = 1 << 3,
    SPI_PCS4 = 1 << 4,
    SPI_PCS5 = 1 << 5,
} SPI_PCSn_e;



/**
  * @brief    SPI初始化
  *
 *  @param      SPIn_e          SPI模块(SPI0、SPI1、SPI2)
 *  @param      SPIn_PCSn_e     片选管脚编号
 *  @param      master          片选管脚编号
 *  @param      mode            0: CLK空闲时刻为高电平 在第一个CLK跳边沿（下降沿）捕获数据
 *  @param      mode            1: CLK空闲时刻为高电平 在第二个CLK跳边沿（上升沿）捕获数据
 *  @param      mode            2: CLK空闲时刻为低电平 在第一个CLK跳边沿（上升沿）捕获数据
 *  @param      mode            3: CLK空闲时刻为低电平 在第二个CLK跳边沿（下降沿）捕获数据
 *  @param      SPI_CFG         SPI主从机模式选择 
 *  @param      baud            baud MHz
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/5/27 星期一
  */
uint32 SPI_Init(SPI_Type *spin, SPI_PCSn_e pcs, SPI_CFG master,uint8_t mode, uint32 baud);

/**
  * @brief    SPI 读写函数
  *
  * @param      SPIn_e          SPI模块(SPI0、SPI1、SPI2)
  * @param      SPIn_PCSn_e     片选管脚编号
  * @param      modata          发送的数据缓冲区地址(不需要接收则传 NULL)
  * @param      midata          发送数据时接收到的数据的存储地址(不需要接收则传 NULL) 
  * @param      len             读写长度
  *
  * @return   
  *
  * @note     
  *
  * @example  
  *
  * @date     2019/5/27 星期一
  */
void SPI_RadeWrite(SPI_Type *spin, SPI_PCSn_e pcs, uint8 *modata, uint8 *midata, uint32 len);





#endif