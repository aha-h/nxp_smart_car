#ifndef _INIT_H
#define _INIT_H

#include "include.h"

#define dt              0.0055   //0.006                    //积分间隔
#define BELL_ON         GPIO_PinWrite(PTE26,1)
#define BELL_OFF        GPIO_PinWrite(PTE26,0)
#define BM1 PTC5
#define BM2 PTC4
#define BM3 PTC6
#define BM4 PTC7
#define K1  PTC0
#define K2  PTC1
#define K3  PTC3

#define KEY_IN_1   PTD1
#define KEY_IN_2   PTD3
#define KEY_IN_3   PTD5
#define KEY_IN_4   PTD7
#define KEY_OUT_1  PTD0
#define KEY_OUT_2  PTD2
#define KEY_OUT_3  PTD4
#define KEY_OUT_4  PTD6

#define LED_1	    PTE26   //核心板上逆时针排列,这个是左上角
#define LED_2	    PTA17
#define LED_3	    PTC0
#define LED_4	    PTD15	
#define LED_5	    PTA5	//0为点亮
#define LED_6	    PTA7
#define LED_7	    PTC18  //1为点亮
#define LED_8	    PTC19
#define LED_9	    PTA8  //1为点亮
#define LED_10	  PTA9
#define LED_11    PTA10 
#define LED_12	  PTA11



/*
                        ADC对应引脚
----------------ADC0--------
ADC0_SE8-----PTB0
ADC0_SE9-----PTB1
ADC0_SE12----PTB2
ADC0_SE13----PTB3

------------ADC1---------------
ADC1_SE8------PTB0
ADC1_SE9------PTB1
ADC1_SE10-----PTB4
ADC1_SE11-----PTB5
ADC1_SE12-----PTB6
ADC1_SE13-----PTB7
    
*/

//*********电机****************//
#define MOTOR_HZ         17000                     //C车电机频率建议在17k左右

//*********舵机****************//
#define S3010_FTM   FTM0
#define S3010_CH    FTM_CH1
#define S3010_HZ    50

extern int into_garage_thres;
extern uint8_t direction_into_garage;
extern uint8_t left_right_parameter ;
extern uint8_t one_circle_flag ;

void InitAll(void);
void Get_OffsetData(void);
void Para_Init();
void Start_Bell();

#endif
