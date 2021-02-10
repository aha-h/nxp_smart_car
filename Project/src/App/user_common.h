/*
 * 求最大值和最小值
 */
#define MAX( x, y ) ( ((x) > (y)) ? (x) : (y) )
#define MIN( x, y ) ( ((x) < (y)) ? (x) : (y) )

/*
 * 返回数组元素的个数
 */
#define ARR_SIZE( a ) ( sizeof( (a) ) / sizeof( ((a)[0]) ) )

/*
 * 确保x的范围为 min~max
 */
#define RANGE(x,max,min)        ((uint8)((x)<(min) ? (min) : ( (x)>(max) ? (max):(x) )))

/*
 * 宏定义实现返回绝对值（x里不能有自加自减的语句，否则变量出错）
 */
#define ABS(x) (((x) > 0) ? (x) : (-(x)))

#define INPUT_STATE(x)  GPIO_PinRead(x)

#define LED_ON(x)    GPIO_PinWrite(x,0)

#define LED_OFF(x)   GPIO_PinWrite(x,1)

#define _LED_ON(x)    GPIO_PinWrite(x,1)

#define _LED_OFF(x)   GPIO_PinWrite(x,0)

#define Myabs(x) (x = (x >= 0 ? x : -x))   //求x的绝对值


