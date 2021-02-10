#include "ANO.h"

uint8_t data_to_send[50];	//发送数据缓存

//static void ANO_DT_Send_Check(uint8_t head, uint8_t check_sum)
//{
//    
//	data_to_send[0]=0xAA;
//	data_to_send[1]=0x05;
//    data_to_send[2]=0xAF;
//	data_to_send[3]=0xF1;
//	data_to_send[4]=0;
//	data_to_send[5]=head;
//	data_to_send[6]=check_sum;
//	
//	
//	uint8_t sum = 0;
//	for(uint8_t i=0;i<6;i++)
//		sum += data_to_send[i];
//	data_to_send[6]=sum;
//
//	ANO_DT_Send_Data(data_to_send, 7);
//}

void ANO_DT_Send_Data(uint8_t *dataToSend , uint8_t length)
{
	
	UART_PutBuff(UART4, dataToSend, length);     //可以修改不同的串口发送数据
	
}

/*发送给上位机的数据协议*/
void ANO_DT_send_int16(short data1, short data2, short data3/*, short data4, short data5/*, short data6, short data7, short data8 ,short data7, short data8, short .....可根据需要自行添加 */)
{
    uint8_t _cnt=0;
    
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x05;
    data_to_send[_cnt++]=0xAF;
	data_to_send[_cnt++]=0xF1;
	data_to_send[_cnt++]=0;
//	data_to_send[_cnt++]=head;
//	data_to_send[_cnt++]=check_sum;
	
    
	data_to_send[_cnt++]=BYTE1(data1);
	data_to_send[_cnt++]=BYTE0(data1);

	data_to_send[_cnt++]=BYTE1(data2);
	data_to_send[_cnt++]=BYTE0(data2);

	data_to_send[_cnt++]=BYTE1(data3);
	data_to_send[_cnt++]=BYTE0(data3);
    
//    data_to_send[_cnt++]=BYTE1(data4);
//	data_to_send[_cnt++]=BYTE0(data4);
//
//	data_to_send[_cnt++]=BYTE1(data5);
//	data_to_send[_cnt++]=BYTE0(data5);

//	data_to_send[_cnt++]=BYTE1(data6);
//	data_to_send[_cnt++]=BYTE0(data6);
//    
//    data_to_send[_cnt++]=BYTE1(data7);
//	data_to_send[_cnt++]=BYTE0(data7);
//
//	data_to_send[_cnt++]=BYTE1(data8);
//	data_to_send[_cnt++]=BYTE0(data8);
    
    /*,short data7, short data8, short .....可根据需要自行添加 */
    
    data_to_send[4] = _cnt - 3;
    
    uint8_t sum = 0;
	for(uint8_t i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
	
//	ANO_DT_Send_Data(data_to_send, _cnt);
    UART_PutBuff(UART4, data_to_send, _cnt);     //可以修改不同的串口发送数据
}