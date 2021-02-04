#include "include.h"
#include "MK60_FLASH.h"

//宏定义 FCCOB Number
#define     FCMD        FTFE_FCCOB0         //FTFL 命令

#define     FADDR2      FTFE_FCCOB1         //Flash address [23:16]
#define     FADDR1      FTFE_FCCOB2         //Flash address [15:8]
#define     FADDR0      FTFE_FCCOB3         //Flash address [7:0]
//Must be 64-bit aligned (Flash address [2:0] = 000)  8字节对齐

#define     FDATA0      FTFE_FCCOB4         //Data Byte 0       //注意一点，4字节排序，FDATA3是最低位，FDATA0是最高位
#define     FDATA1      FTFE_FCCOB5         //Data Byte 1
#define     FDATA2      FTFE_FCCOB6         //Data Byte 2
#define     FDATA3      FTFE_FCCOB7         //Data Byte 3
#define     FDATA4      FTFE_FCCOB8         //Data Byte 4
#define     FDATA5      FTFE_FCCOB9         //Data Byte 5
#define     FDATA6      FTFE_FCCOBA         //Data Byte 6
#define     FDATA7      FTFE_FCCOBB         //Data Byte 7

//FCMD 命令
#define     RD1BLK    0x00   // 读整块Flash
#define     RD1SEC    0x01   // 读整个扇区
#define     PGMCHK    0x02   // 写入检查
#define     RDRSRC    0x03   // 读目标数据(8字节)
#define     PGM8      0x07   // 写入长字(8字节)
#define     ERSBLK    0x08   // 擦除整块Flash
#define     ERSSCR    0x09   // 擦除Flash扇区
#define     PGMSEC    0x0B   // 写入扇区
#define     RD1ALL    0x40   // 读所有的块
#define     RDONCE    0x41   // 只读一次
#define     PGMONCE   0x43   // 只写一次
#define     ERSALL    0x44   // 擦除所有块
#define     VFYKEY    0x45   // 验证后门访问钥匙
#define     PGMPART   0x80   // 写入分区
#define     SETRAM    0x81   // 设定FlexRAM功能


void flash_delayms(uint16_t ms)
{
    while(ms--)
    {
        for(int i = 0; i < 100000; i++)
        {
            __asm("NOP");
        }
    }

}

/*!
 *  功能：      Flash命令
 *  返回值：    命令执行结果(1成功，0失败)
 */
uint8 flash_cmd()
{
    //写 FTFL_FSTAT 启动 Flash命令
    FTFE_FSTAT =    (0
                     |  FTFE_FSTAT_CCIF_MASK        // 指令完成标志(写1清0)
                     |  FTFE_FSTAT_RDCOLERR_MASK    // 读冲突错误标志(写1清0)
                     |  FTFE_FSTAT_ACCERR_MASK      // 访问错误标志位(写1清0)
                     |  FTFE_FSTAT_FPVIOL_MASK      // 非法访问保护标志位(写1清0)
                    );

    while(!(FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK));    // 等待命令完成

    // 检查错误标志
    if( FTFE_FSTAT & (FTFE_FSTAT_ACCERR_MASK | FTFE_FSTAT_RDCOLERR_MASK | FTFE_FSTAT_FPVIOL_MASK | FTFE_FSTAT_MGSTAT0_MASK))
    {
        return 0 ;                                  //执行命令出错
    }else{
        return 1;                                   //执行命令成功
    }

}

    



//-------------------------------------------------------------------------------------------------------------------
//  @brief      FLASH初始化
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void FLASH_Init(void)
{
    // 清除Flash预读取缓冲区
    FMC_PFB01CR |= FMC_PFB01CR_S_B_INV_MASK;      //
    //FMC_PFB23CR |= FMC_PFB23CR_S_B_INV_MASK;

    while(!(FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK));    // 等待命令完成

    FTFE_FSTAT =    (0
                     |  FTFE_FSTAT_CCIF_MASK        // 指令完成标志(写1清0)
                     |  FTFE_FSTAT_RDCOLERR_MASK    // 读冲突错误标志(写1清0)
                     |  FTFE_FSTAT_ACCERR_MASK      // 访问错误标志位(写1清0)
                     |  FTFE_FSTAT_FPVIOL_MASK      // 非法访问保护标志位(写1清0)
                    );
    flash_delayms(10);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      FLASH擦除扇区
//  @param      SectorNum 		倒数扇区号（K66实际使用1~256）
//  @return     				返回1擦除失败，返回0擦除成功
//  @since      v1.0
//  Sample usage:               uint32 dat = FLASH_GetSectorSize(10);
//-------------------------------------------------------------------------------------------------------------------
uint8 FLASH_EraseSector(uint32 SectorNum)
{
    uint32 addr = (FLASH_SECTOR_NUM - SectorNum) * FLASH_SECTOR_SIZE;

    // 设置擦除命令
    FCMD = ERSSCR;

    // 设置目标地址
    FADDR2 = ((Dtype *)&addr)->B[2];
    FADDR1 = ((Dtype *)&addr)->B[1];
    FADDR0 = ((Dtype *)&addr)->B[0];

    if(flash_cmd() == 0)
    {
        return 0;
    }

    if(SectorNum == 0)
    {
        return flash_write(SectorNum,0x000408,(u64)0xFFFFFFFEFFFFFFFF);
    }

    return 1;
}


/*
 *  功能：   写入长字节数据到 flash指定地址
 *  参数：   sector_num      倒数扇区号（K66实际使用1~256）
 *           offset          写入扇区内部偏移地址（0~2043 中 4的倍数）
 *           data            需要写入的数据
 *  返回值： 执行结果(1成功，0失败)
 *  例：     flash_write(127,0,0xFFFFFFFE);        //扇区127,偏移地址为0,写入数据:0xFFFFFFFE
 */
uint8 flash_write(uint16 sector_num, uint16 offset, FLASH_WRITE_TYPE data)
{
    uint32 addr = (FLASH_SECTOR_NUM - sector_num) * FLASH_SECTOR_SIZE  + offset ;
    uint32 tmpdata;

    // 设置目标地址
    FADDR2 = ((Dtype *)&addr)->B[2];
    FADDR1 = ((Dtype *)&addr)->B[1];
    FADDR0 = ((Dtype *)&addr)->B[0];

    // 设置 低32位数据
    tmpdata = (uint32)data;

    FDATA0 = ((Dtype *)&tmpdata)->B[3];                    // 设置写入数据
    FDATA1 = ((Dtype *)&tmpdata)->B[2];
    FDATA2 = ((Dtype *)&tmpdata)->B[1];
    FDATA3 = ((Dtype *)&tmpdata)->B[0];

    // 设置擦除命令
    FCMD = PGM8;

    // 设置 高32位数据
    tmpdata = (uint32)(data>>32);

    FDATA4 = ((Dtype *)&tmpdata)->B[3];                 // 设置写入数据
    FDATA5 = ((Dtype *)&tmpdata)->B[2];
    FDATA6 = ((Dtype *)&tmpdata)->B[1];
    FDATA7 = ((Dtype *)&tmpdata)->B[0];

    if(flash_cmd() == 0)
    {
        return 0;
    }

    return 1;  //成功执行

}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      FLASH数据写入到扇区
//  @param      SectorNum 		需要写入的扇区编号
//  @param      *buf	 		数据首地址
//  @param      len		 		写入的字节数 特别提醒一个uint16变量是两个字节 一个uint32变量是四个字节
//  @param      offset		 	必须为4的整数倍
//  @return     				返回1写入失败，返回0写入成功
//  @since      v1.0
//  Sample usage:               FLASH_WriteSector(10,(const uint8 *)buf,4,0);//将buf数组里面的前四位数据写入到10号扇区(每一位是uint8类型)
//-------------------------------------------------------------------------------------------------------------------
uint8 FLASH_WriteBuf(uint32 SectorNum, const uint8 *buf, uint32 len, uint32 offset)
{
    uint32  size;

    uint32  addr;
    uint32  data;
    
	addr = (uint32)(FLASH_SECTOR_NUM - SectorNum)*FLASH_SECTOR_SIZE + offset;

	// 设置写入命令
    FCMD = PGM8;

    for(size = 0; size < len; size += FLASH_ALIGN_ADDR )
    {

        // 设置目标地址
        FADDR2 = ((Dtype *)&addr)->B[2];
        FADDR1 = ((Dtype *)&addr)->B[1];
        FADDR0 = ((Dtype *)&addr)->B[0];

        // 设置 低32位数据
        data =  *(uint32 *)buf;

        FDATA0 = ((Dtype *)&data)->B[3];                    // 设置写入数据
        FDATA1 = ((Dtype *)&data)->B[2];
        FDATA2 = ((Dtype *)&data)->B[1];
        FDATA3 = ((Dtype *)&data)->B[0];


        // 设置 高32位数据
        data = *(uint32 *)(buf+4);

        FDATA4 = ((Dtype *)&data)->B[3];                    // 设置写入数据
        FDATA5 = ((Dtype *)&data)->B[2];
        FDATA6 = ((Dtype *)&data)->B[1];
        FDATA7 = ((Dtype *)&data)->B[0];

        if(flash_cmd() == 0)
        {
            return 0;
        }

        addr += FLASH_ALIGN_ADDR;
        buf += FLASH_ALIGN_ADDR;
    }
    return 1;  //成功执行
}


/*
 *  功能：      读取 flash指定地址数据到缓冲区
 *  参数：      sector_num      扇区号（K60N512实际使用0~255）
 *              offset          读取扇区内部偏移地址（0~2043 中 4的倍数）
 *              cnt             数目      sizeof(buff)
 *              buf             需要写入的数据缓冲区首地址
 *  返回值：    执行结果(1成功)
*/
uint8_t FLASH_ReadBuff(uint16 sectorNo, uint16 offset, uint16 cnt, char *buf)
{
    u32 wAddr = 0;
    wAddr = (FLASH_SECTOR_NUM - sectorNo) * FLASH_SECTOR_SIZE + offset;
    while (cnt--)
        *buf++=*(u8*)wAddr++;
    return 1;
}