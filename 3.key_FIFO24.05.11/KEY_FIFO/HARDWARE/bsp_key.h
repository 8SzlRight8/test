#ifndef _BSP_KEY_H_
#define	_BSP_KEY_H_


#include "stm32f4xx.h"

//------------------------
// FIFO 类
//------------------------

#define KEY_FIFO_SIZE   10

typedef struct
{
	unsigned char Read;                    /* 缓冲区读指针1 */
	unsigned char Write;                 /* 缓冲区写指针 */
	unsigned char Read2;                 /* 缓冲区读指针2 */
	unsigned char temp[5];
	unsigned char Buf[KEY_FIFO_SIZE];        /* 键值缓冲区 */
    
}KEY_FIFO_T;


//键值定义
typedef enum
{
    KEY_NONE = 0,            /* 0 表示按键事件 */

    KEY_1_DOWN,            /* 1键按下 */
    KEY_1_UP,                /* 1键弹起 */
    KEY_1_LONG,            /* 1键长按 */

    KEY_2_DOWN,            /* 2键按下 */
    KEY_2_UP,                /* 2键弹起 */
    KEY_2_LONG,            /* 2键长按 */

    KEY_3_DOWN,            /* 3键按下 */
    KEY_3_UP,                /* 3键弹起 */
    KEY_3_LONG,            /* 3键长按 */

    KEY_4_DOWN,            /* 4键按下 */
    KEY_4_UP,                /* 4键弹起 */
    KEY_4_LONG,            /* 4键长按 */

    KEY_5_DOWN,            /* 5键按下 */
    KEY_5_UP,                /* 5键弹起 */
    KEY_5_LONG,            /* 5键长按 */

    KEY_6_DOWN,            /* 6键按下 */
    KEY_6_UP,                /* 6键弹起 */
    KEY_6_LONG,            /* 6键长按 */

    KEY_7_DOWN,            /* 7键按下 */
    KEY_7_UP,                /* 7键弹起 */
    KEY_7_LONG,            /* 7键长按 */

    KEY_8_DOWN,            /* 8键按下 */
    KEY_8_UP,                /* 8键弹起 */
    KEY_8_LONG,            /* 8键长按 */

    /* 组合键 */
    KEY_9_DOWN,            /* 9键按下 */
    KEY_9_UP,                /* 9键弹起 */
    KEY_9_LONG,            /* 9键长按 */

    KEY_10_DOWN,            /* 10键按下 */
    KEY_10_UP,            /* 10键弹起 */
    KEY_10_LONG,            /* 10键长按 */
}KEY_ENUM;


//------------------------
// 按 键 类
//------------------------

#define HARD_KEY_NUM    8                 /* 实体按键个数 */
#define KEY_COUNT   (HARD_KEY_NUM + 2)   /* 8个独立建 + 2个组合按键 */

#define KEY_FILTER_TIME   5
#define KEY_LONG_TIME     100


typedef struct{
	
	GPIO_TypeDef *gpio;
	unsigned int pin;
	unsigned char ActiveLevel;   //引脚端口时低有效还是高有效
	unsigned int rcc_gpio;
	
}KEY_GPIO_T;


typedef struct
{
    /* 下面是一个函数指针，指向判断按键手否按下的函数 */
    unsigned char  (*IsKeyDownFunc)(void); /* 按键按下的判断函数,1表示按下 */

    unsigned char   Count;        /* 滤波器计数器 */
	  unsigned char   State;        /* 按键当前状态（按下还是弹起） */
    
	  unsigned short  LongCount;    /* 长按计数器 */
    unsigned short  LongTime;        /* 按键按下持续时间, 0表示不检测长按 */
    
    unsigned char   RepeatSpeed;    /* 连续按键周期 */
    unsigned char   RepeatCount;    /* 连续按键计数器,当其次数大于RepeatSpeed时，会连发一个按键数据 */
}KEY_T;


//复合按键检测，可以直接用0，1，2来代替
typedef enum
{
	KID_K1,
	KID_K2,
	KID_K3,
}KEY_PLUS_ENUM;  


//------------------------
// 函 数 类
//------------------------
void bsp_InitKey(void);    //按键初始化函数


void bsp_KeyScan10ms(void);  //定时器周期调用

void bsp_PutKey(unsigned char _KeyCode);
unsigned char bsp_GetKey(void);
unsigned char bsp_GetKey2(void);

#endif /* _BSP_KEY_H_ */

