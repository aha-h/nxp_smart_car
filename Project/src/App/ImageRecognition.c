#include "ImageRecognition.h"

uint8_t into_garage_enable = 0;
uint8_t goto_garage_cnt;
static uint8_t see_cnt = 0;
int cntColorChange = 0;  //计数跳变的次数，用于判断斑马线
uint16_t ccd_buf[128];
uint8_t getinto_garage_flag = 0;

void GetImageArray()
{
    
    	int iTmp, jLeft, jRight, cntBlackBlock;
        cntColorChange = 0;
//    if(start_car == 1 && !into_garage_enable){
        if(test_startline == 0){
          if((!stopflag && !Circle_Flag && test_roadpart == 0) || (!stopflag && test_roadpart == 1)){//判断完环岛之后才会进行入库 //&& !one_circle_flag
              CCD_Collect(ccd_buf);
              for (uint8_t i = 0; i <= 123; i++)
              {
                if ((ccd_buf[i] > start_line_thres) && (ccd_buf[i + 1 < 128 ? (i + 1) : 127] > start_line_thres) && (ccd_buf[i + 2 < 128 ? (i + 2) : 127] < start_line_thres) && (ccd_buf[i + 3 < 128 ? (i + 3) : 127] < start_line_thres) && (ccd_buf[i + 4 < 128 ? (i + 4) : 127] < start_line_thres))
                {
                  jLeft = i + 2;    //黑块左侧位置
                  iTmp = jLeft;
                  for (; iTmp <= 123; iTmp++)
                  {
                    if ((ccd_buf[iTmp] < start_line_thres) && (ccd_buf[iTmp + 1] < start_line_thres) && (ccd_buf[iTmp + 2] > start_line_thres) && (ccd_buf[iTmp + 3] > start_line_thres) && (ccd_buf[iTmp + 4] > start_line_thres))
                    {
                      jRight = iTmp + 1;   //黑块右侧问题
                      break;
                    }
                  }
                  if (jRight - jLeft > 2)  //黑块宽度大于2才判定为有效的起跑线
                  {
                    cntColorChange++;
                  }
                  if(cntColorChange >= 5)
                  {
//                    BELL_ON;
                    cntColorChange = 0;
//                    stopflag = 1;
                    getinto_garage_flag = 1;
//                    into_garage_enable ++;
//                    BELL_OFF;
                    break;
                  }
                }
              }
            }
        }else{
            CCD_Collect(ccd_buf);
        }
                
        
        

//            for(int i = 0; i < 128;i++){
//              if(ccd_buf[i] > start_line_thres &&  ccd_buf[i+1] < start_line_thres){
//                goto_garage_cnt++;
//                if(goto_garage_cnt > 4){
//                  goto_garage_cnt = 0;
//                  if(see_cnt++ > 20){
//                    see_cnt = 0;
//                    stopflag = 1;
//                    goto_garage_cnt = 0;
//                    into_garage_enable = 1;
//                  }
//                  
//                }
//              }
//            }
//        }
//    }
}

















//uint8_t pixle[60][94];      //一帧图像
//float BlackThres = 170.0;   //黑白阈值
//uint8_t cntWhiteToBlack = 0;    //计数黑白跃变的次数，用于判断斑马线
//uint8_t Starting_Line_Flag = 0;     //是否找到起跑线标志位
//
//uint8_t res_Garage = 0;     //是否检测到了车库。0是未检测到，1是左车库，2是右车库
//uint8_t RightLine[60] = { 0 };     //每一行的右边界数据  初值94
//uint8_t LeftLine[60] = { 0 };      //每一行的左边界数据  初值1
//uint8_t WidthLeft[60] = { 0 };     //从中线向左找的赛道宽度  初值47
//uint8_t WidthRight[60] = { 0 };    //从中线向右找的赛道宽度  初值47
//
//uint8_t Image_Use[LCDH][LCDW];           //压缩后之后用于存放屏幕显示数据
//uint8_t pixle[LCDH][LCDW];               //二值化后用于OLED显示的数据
//uint8_t zip_pixle[ZIPH][ZIPW];
//u8 arry_cnt = 0;                      //处理图像帧数
//float  BlackThres = 170.0;   //黑白阈值
//
///************************************************  起跑线检测 StartLineCheck  ***************************************************/
//void StartLineCheck()
//{
//	int site_x;  //用于记录检测到起跑线的行i
//	int i, j;
//    static uint8_t cntWhiteToBlack = 0;
//    static uint8_t isStartLine;
//	for (i = 20; i < 59; )   //检测图像的前40行
//	{
//		i += 2;  //隔行处理
//		site_x = i;
//		/************前20行、线齐、不是环岛************/
//		if ((site_x >= 20) && (site_x <= 59))
//		{
//			for (j = 20; j < 65; j++)
//			{
//				if ((Image_Use[i][j - 1] > BlackThres) && (Image_Use[i][j] > BlackThres) && (Image_Use[i][j + 1] < BlackThres) && (Image_Use[i][j + 2] < BlackThres))
//				{
//					cntWhiteToBlack++;
//					if (cntWhiteToBlack >= 4)
//					{
//						cntWhiteToBlack = 0;
//						isStartLine = 1;
//						break;
//					}
//				}
//			}
//		}
//		if (isStartLine)
//		{
//			LED_ON(LED_6);   //如果检测到起跑线，蜂鸣器响
//			break;
//		}else{
//            LED_OFF(LED_7);
//            break;
//        }
//	}
//}
//

//
///******************
//     迭代法
//*****************/
//#define Thres      60    //阈值
//#define ERROR      2      //误差
//__ramfunc void GetThreshold(void)
//{ 
//    uint16_t i = 0,j = 0,N0 = 0,N1 = 0,flag = 0;
//    float T0,T1,T2,T_center;
//    uint32_t S0 = 0,S1 = 0;
//    T2 = BlackThres;
//    do{
//        for(i=0;i < LCDH;i++){
//          for(j=0;j < LCDW;j++){
//            if(Image_Use[i][j] < T2){
//              S0 += Image_Use[i][j];
//              N0++;
//            }
//            else{
//              S1 += Image_Use[i][j];
//              N1++;
//            }
//          }
//        }
//        T0 = S0/N0;
//        T1 = S1/N1;
//        T_center = (T0+T1)/2;
//        if(T2 < T_center){
//          if((T_center - T2)> ERROR){
//            flag = 1;
//          }
//          else{
//            flag = 0;
//          }
//        }
//        else{
//          if((T2 - T_center) > ERROR){
//            flag = 1;
//          }
//          else{
//            flag = 0;
//          }
//        }
//        T2 =T_center;
//        BlackThres = T2;
//    }
//    while(flag);
//}
//
//__ramfunc  void Get_01_Value(u8 image[LCDH][LCDW])
//{
// for(int i = 0; i < LCDH; i++)
// {
//   for(int j = 0; j < LCDW; j++)
//   {                                
//     if(Image_Use[i][j] > BlackThres) //数值越大，显示的内容越多，较浅的图像也能显示出来    
//       pixle[i][j] = 255;        
//     else                                        
//       pixle[i][j] = 0;
//   }    
// }
//}
//
//__ramfunc void sobel(uint8_t imag[LCDH][LCDW],uint8_t imag1[LCDH][LCDW])
//{
////                char txt[20];   6ms左右
////     static uint64_t nowtime = 0;
////      nowtime = systime.get_time_us();
//	int tempx=0,tempy=0,temp=0,i=0,j=0;
//	for(i=1;i <LCDH-1; i++)
//		{
//		  for(j=1;j<LCDW-1;j++)
//			{
//        				
//				tempx=(-  imag[i-1][j-1])
//						 +(-2*imag[i  ][j-1])
//						 +(-  imag[i+1][j-1])
//						 +(   imag[i-1][j+1])
//				     +( 2*imag[i  ][j+1])
//				     +(   imag[i+1][j+1]);
//				if(tempx<0)
//					tempx=-tempx;
//				
//				tempy=(   imag[i+1][j-1])
//						 +( 2*imag[i+1][j  ])
//						 +(   imag[i+1][j+1])
//						 +(-  imag[i-1][j-1])
//				     +(-2*imag[i-1][j  ])
//				     +(-  imag[i-1][j+1]);
//				if(tempy<0)
//					tempy=-tempy;
//				temp=tempx+tempy;
//				if(temp>255)
//					temp=255;
//					
//				imag1[i][j]=(u8)temp;
//                if(imag1[i][j] > 240){
//                    imag1[i][j] = 255;
//                }else{
//                    imag1[i][j] = 0;
//                }
//				
//			}
//		}
//    
////              
////    sprintf(txt,"usetime:%ld",(systime.get_time_us() - nowtime));
////    UART_PutStr(UART4, txt);
//}

