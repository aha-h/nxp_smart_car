/*
【平    台】北京龙邱智能科技MK66FX1M0VLQ18核心板
【编    写】CHIUSIR
【备    注】
【软件版本】V1.0
【最后更新】2016年08月20日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【交流邮箱】chiusir@163.com
*/

#ifndef _LQOLED_H
#define _LQOLED_H

/*  母板上使用OLED请将 OLED_PIN 定义为1   核心板上使用请将OLED_PIN 定义为0 */
#define OLED_PIN    1

void OLED_Init(void);
void OLED_CLS(void);
void OLED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);
void OLED_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[]);
void OLED_P14x16Str(unsigned char x,unsigned char y,unsigned char ch[]);
void OLED_Print(unsigned char x, unsigned char y, unsigned char ch[]);
void OLED_PutPixel(unsigned char x,unsigned char y);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_WrDat(unsigned char data);
void OLED_WrCmd(unsigned char cmd);
void OLED_Rectangle(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char gif);
void OLED_Show_LQLogo(void);
void OLED_Show_LibLogo(void);
void OLED_Show_BMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char * bmp); 
void OLED_Show_Road(void);
void OLED_Show_Frame80(void);
void OLED_Show_Frame94(void);
void OLED_WrDat(unsigned char data);
void OLED_WrCmd(unsigned char cmd);
void OLED_Fill(unsigned char bmp_data);
void OLED_DLY_ms(unsigned int ms);

#endif

