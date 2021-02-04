#ifndef _MK60_FLASH_H_
#define _MK60_FLASH_H_

//k66包含1024K的程序Flash  
//1024K的程序Flash分为256个扇区，每个扇区4K大小
//    sector（4K）为擦除最小单位
//    长字（32b）为写的最小单位

#include "common.h"

#define FLASH_SECTOR_SIZE     	    (4096)                  //扇区大小 4k 字节
#define FLASH_SECTOR_NUM        	(256)                   //扇区数
#define FLASH_ALIGN_ADDR        	8                       //地址对齐整数倍
typedef u64                    	FLASH_WRITE_TYPE;       //flash_write 函数写入 的数据类型


/*
 * 定义带位域的联合体类型
 */
typedef union
{
    uint32  DW;
    uint16  W[2];
    uint8   B[4];
    struct
    {
        uint32 b0: 1;
        uint32 b1: 1;
        uint32 b2: 1;
        uint32 b3: 1;
        uint32 b4: 1;
        uint32 b5: 1;
        uint32 b6: 1;
        uint32 b7: 1;
        uint32 b8: 1;
        uint32 b9: 1;
        uint32 b10: 1;
        uint32 b11: 1;
        uint32 b12: 1;
        uint32 b13: 1;
        uint32 b14: 1;
        uint32 b15: 1;
        uint32 b16: 1;
        uint32 b17: 1;
        uint32 b18: 1;
        uint32 b19: 1;
        uint32 b20: 1;
        uint32 b21: 1;
        uint32 b22: 1;
        uint32 b23: 1;
        uint32 b24: 1;
        uint32 b25: 1;
        uint32 b26: 1;
        uint32 b27: 1;
        uint32 b28: 1;
        uint32 b29: 1;
        uint32 b30: 1;
        uint32 b31: 1;
    };
} Dtype;    //sizeof(Dtype) 为 4



/*
 *  功能：      读取 flash指定地址数据到缓冲区
 *  参数：      sector_num      扇区号（K60N512实际使用0~255）
 *              offset          读取扇区内部偏移地址（0~2043 中 4的倍数）
 *              len             数目      sizeof(buff)
 *              buf             需要写入的数据缓冲区首地址
 *  返回值：    执行结果(1成功)
*/
uint8_t FLASH_ReadBuff(uint16 sectorNo, uint16 offset, uint16 len, char *buf);


/*!
 *  @brief      使用宏定义对flash进行数据读取
 *  @param      sectorNo 		倒数扇区号（K66实际使用1~256）
 *  @param      offset	 		入扇区内部偏移地址（0~4095 中 4的倍数）
 *  @param      type		 	读取的数据类型
 *  @return     				返回给定地址的数据
 *  @notice     扇区不要使用前面的  程序存放在靠前的扇区，所以这里的sector_num是倒数的
 *  Sample usage:               FLASH_Read(1,0,uint32);//读取倒数第一个扇区偏移0数据类型为uint32
 */
#define     FLASH_Read(sectorNo,offset,type)        (*(type *)((uint32_t)(((FLASH_SECTOR_NUM - sectorNo)*FLASH_SECTOR_SIZE) + (offset)))) 

/*!
 *  @brief      Flash 初始化   
 *  @return     无
 *  @notice     使用Flash时一定记得初始化
 *  Sample usage:  
 */
void FLASH_Init(void);


/*
 *  功能：   写入长字节数据到 flash指定地址
 *  参数：   sector_num      倒数扇区号（K66实际使用1~256）
 *           offset          写入扇区内部偏移地址（0~2043 中 4的倍数）
 *           data            需要写入的数据
 *  返回值： 执行结果(1成功，0失败)
 *  例：     flash_write(127,0,0xFFFFFFFE);        //扇区127,偏移地址为0,写入数据:0xFFFFFFFE
 */
uint8 flash_write(uint16 sector_num, uint16 offset, FLASH_WRITE_TYPE data);

/*!
 *  @brief      擦出扇区
 *  @param      sectorNo 		倒数扇区号（K66实际使用1~256）
 *  @return     返回1擦除失败，返回0擦除成功
 *  @notice     
 *  Sample usage: uint32 dat = FLASH_GetSectorSize(10);
 */
uint8 FLASH_EraseSector(uint32 SectorNum);


/*!
 *  @brief      把数据写入扇区
 *  @param      sectorNo 		倒数扇区号（K66实际使用1~256）
 *  @param      offset	 		入扇区内部偏移地址（0~4095 中 4的倍数）
 *  @param      buf		 	    数据首地址
 *  @param      len		 	    数据长度
 *  @return     				返回给定地址的数据
 *  @notice     扇区不要使用前面的  程序存放在靠前的扇区，所以这里的sector_num是倒数的
 *  Sample usage:               FLASH_Read(1,0,uint32);//读取倒数第一个扇区偏移0数据类型为uint32
 */
uint8 FLASH_WriteBuf(uint32 SectorNum, const uint8 *buf, uint32 len, uint32 offset);



#endif //_FLASH_H_
