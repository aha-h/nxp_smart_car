#ifndef __ANO_H
#define __ANO_H

#include "include.h"

//数据拆分宏定义，在发送大于1字节的数据类型时，比如int16、float等，需要把数据拆分成单独字节进行发送
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)    ) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

//static void ANO_DT_Send_Check(uint8_t head, uint8_t check_sum);
//void ANO_DT_Send_Data(uint8_t *dataToSend , uint8_t length);
void ANO_DT_send_int16(short data1, short data2, short data3/*, short data4, short data5/*, short data6, short data7, short data8 ,short data7, short data8, short .....可根据需要自行添加 */);

#endif