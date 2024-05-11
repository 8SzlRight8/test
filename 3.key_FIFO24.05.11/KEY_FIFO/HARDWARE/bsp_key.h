#ifndef _BSP_KEY_H_
#define	_BSP_KEY_H_


#include "stm32f4xx.h"

//------------------------
// FIFO ��
//------------------------

#define KEY_FIFO_SIZE   10

typedef struct
{
	unsigned char Read;                    /* ��������ָ��1 */
	unsigned char Write;                 /* ������дָ�� */
	unsigned char Read2;                 /* ��������ָ��2 */
	unsigned char temp[5];
	unsigned char Buf[KEY_FIFO_SIZE];        /* ��ֵ������ */
    
}KEY_FIFO_T;


//��ֵ����
typedef enum
{
    KEY_NONE = 0,            /* 0 ��ʾ�����¼� */

    KEY_1_DOWN,            /* 1������ */
    KEY_1_UP,                /* 1������ */
    KEY_1_LONG,            /* 1������ */

    KEY_2_DOWN,            /* 2������ */
    KEY_2_UP,                /* 2������ */
    KEY_2_LONG,            /* 2������ */

    KEY_3_DOWN,            /* 3������ */
    KEY_3_UP,                /* 3������ */
    KEY_3_LONG,            /* 3������ */

    KEY_4_DOWN,            /* 4������ */
    KEY_4_UP,                /* 4������ */
    KEY_4_LONG,            /* 4������ */

    KEY_5_DOWN,            /* 5������ */
    KEY_5_UP,                /* 5������ */
    KEY_5_LONG,            /* 5������ */

    KEY_6_DOWN,            /* 6������ */
    KEY_6_UP,                /* 6������ */
    KEY_6_LONG,            /* 6������ */

    KEY_7_DOWN,            /* 7������ */
    KEY_7_UP,                /* 7������ */
    KEY_7_LONG,            /* 7������ */

    KEY_8_DOWN,            /* 8������ */
    KEY_8_UP,                /* 8������ */
    KEY_8_LONG,            /* 8������ */

    /* ��ϼ� */
    KEY_9_DOWN,            /* 9������ */
    KEY_9_UP,                /* 9������ */
    KEY_9_LONG,            /* 9������ */

    KEY_10_DOWN,            /* 10������ */
    KEY_10_UP,            /* 10������ */
    KEY_10_LONG,            /* 10������ */
}KEY_ENUM;


//------------------------
// �� �� ��
//------------------------

#define HARD_KEY_NUM    8                 /* ʵ�尴������ */
#define KEY_COUNT   (HARD_KEY_NUM + 2)   /* 8�������� + 2����ϰ��� */

#define KEY_FILTER_TIME   5
#define KEY_LONG_TIME     100


typedef struct{
	
	GPIO_TypeDef *gpio;
	unsigned int pin;
	unsigned char ActiveLevel;   //���Ŷ˿�ʱ����Ч���Ǹ���Ч
	unsigned int rcc_gpio;
	
}KEY_GPIO_T;


typedef struct
{
    /* ������һ������ָ�룬ָ���жϰ����ַ��µĺ��� */
    unsigned char  (*IsKeyDownFunc)(void); /* �������µ��жϺ���,1��ʾ���� */

    unsigned char   Count;        /* �˲��������� */
	  unsigned char   State;        /* ������ǰ״̬�����»��ǵ��� */
    
	  unsigned short  LongCount;    /* ���������� */
    unsigned short  LongTime;        /* �������³���ʱ��, 0��ʾ����ⳤ�� */
    
    unsigned char   RepeatSpeed;    /* ������������ */
    unsigned char   RepeatCount;    /* ��������������,�����������RepeatSpeedʱ��������һ���������� */
}KEY_T;


//���ϰ�����⣬����ֱ����0��1��2������
typedef enum
{
	KID_K1,
	KID_K2,
	KID_K3,
}KEY_PLUS_ENUM;  


//------------------------
// �� �� ��
//------------------------
void bsp_InitKey(void);    //������ʼ������


void bsp_KeyScan10ms(void);  //��ʱ�����ڵ���

void bsp_PutKey(unsigned char _KeyCode);
unsigned char bsp_GetKey(void);
unsigned char bsp_GetKey2(void);

#endif /* _BSP_KEY_H_ */

