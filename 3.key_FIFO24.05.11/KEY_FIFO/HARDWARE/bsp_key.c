//------------------------
// 头 文 件
//------------------------
#include "bsp_key.h"

//------------------------
// 变 量 定 义 类
//------------------------
static const KEY_GPIO_T s_gpio_list[HARD_KEY_NUM] = {
	{GPIOG, GPIO_Pin_11,  0, RCC_AHB1Periph_GPIOG},    /* K1 */
	{GPIOD, GPIO_Pin_3, 0, RCC_AHB1Periph_GPIOD},    /* K2 */
	{GPIOF, GPIO_Pin_10, 0, RCC_AHB1Periph_GPIOF},    /* K3 */
	{GPIOB, GPIO_Pin_11,  0, RCC_AHB1Periph_GPIOB},    /* JOY_U */    
	{GPIOG, GPIO_Pin_6,  0, RCC_AHB1Periph_GPIOG},    /* JOY_D */
	{GPIOF, GPIO_Pin_6, 0, RCC_AHB1Periph_GPIOF},    /* JOY_L */    
	{GPIOA, GPIO_Pin_0,  0, RCC_AHB1Periph_GPIOA},    /* JOY_R */    
	{GPIOA, GPIO_Pin_1, 0, RCC_AHB1Periph_GPIOA},    /* JOY_OK */
};

static volatile KEY_T s_tBtn[KEY_COUNT];
static volatile KEY_FIFO_T s_tKey;   //define a struct var

//------------------------
// 函 数 申 明 类
//------------------------
static void bsp_InitKeyVar(void);
static void bsp_InitKeyHard(void);

static unsigned char IsKeyDownFunc(unsigned char _id);
static unsigned char KeyPinActive(unsigned char _id);
static void bsp_DetectKey(uint8_t i);

/*
*********************************************************************************************************
*    函 数 名: bsp_PutKey
*    功能说明: 将1个键值压入按键FIFO缓冲区。可用于模拟一个按键。
*    形    参: _KeyCode : 按键代码
*    返 回 值: 无
*********************************************************************************************************
*/
void bsp_PutKey(unsigned char _KeyCode)
{
    s_tKey.Buf[s_tKey.Write] = _KeyCode;

    if (++s_tKey.Write  >= KEY_FIFO_SIZE)
    {
        s_tKey.Write = 0;
    }
}


/*
*********************************************************************************************************
*    函 数 名: bsp_GetKey
*    功能说明: 从按键FIFO缓冲区读取一个键值。
*    形    参:  无
*    返 回 值: 按键代码
*********************************************************************************************************
*/
unsigned char bsp_GetKey(void)
{
    unsigned char ret;

    if (s_tKey.Read == s_tKey.Write)
    {
        return KEY_NONE;
    }
    else
    {
        ret = s_tKey.Buf[s_tKey.Read];

        if (++s_tKey.Read >= KEY_FIFO_SIZE)
        {
            s_tKey.Read = 0;
        }
        return ret;
    }
}

/*
*********************************************************************************************************
*    函 数 名: bsp_GetKey2
*    功能说明: 从按键FIFO缓冲区读取一个键值。独立的读指针。
*    形    参:  无
*    返 回 值: 按键代码
*********************************************************************************************************
*/
unsigned char bsp_GetKey2(void)
{
    unsigned char ret;

    if (s_tKey.Read2 == s_tKey.Write)
    {
        return KEY_NONE;
    }
    else
    {
        ret = s_tKey.Buf[s_tKey.Read2];

        if (++s_tKey.Read2 >= KEY_FIFO_SIZE)
        {
            s_tKey.Read2 = 0;
        }
        return ret;
    }
}


//按键检测函数
/*
*********************************************************************************************************
*    函 数 名: bsp_InitKey
*    功能说明: 初始化按键. 该函数被 bsp_Init() 调用。
*    形    参: 无
*    返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitKey(void)
{
    bsp_InitKeyVar();        /* 初始化按键变量 */
    bsp_InitKeyHard();        /* 初始化按键硬件 */
}

/*
*********************************************************************************************************
*    函 数 名: bsp_InitKeyVar
*    功能说明: 初始化按键变量
*    形    参:  无
*    返 回 值: 无
*********************************************************************************************************
*/
static void bsp_InitKeyVar(void)
{
    unsigned char i;

    /* 对按键FIFO读写指针清零 */
    s_tKey.Read = 0;
    s_tKey.Write = 0;
    s_tKey.Read2 = 0;

    /* 给每个按键结构体成员变量赋一组缺省值 */
    for (i = 0; i < KEY_COUNT; i++)
    {
        s_tBtn[i].LongTime = KEY_LONG_TIME;            /* 长按时间 0 表示不检测长按键事件 */
        s_tBtn[i].Count = KEY_FILTER_TIME / 2;        /* 计数器设置为滤波时间的一半 */
        s_tBtn[i].State = 0;                        /* 按键缺省状态，0为未按下 */
        s_tBtn[i].RepeatSpeed = 10;                    /* 按键连发的速度，0表示不支持连发.n*10ms 连发一次 */
        s_tBtn[i].RepeatCount = 0;                    /* 连发计数器 */
    }

    /* 如果需要单独更改某个按键的参数，可以在此单独重新赋值 */
    
    /* 摇杆上下左右，支持长按1秒后，自动连发 */
//    bsp_SetKeyParam(KID_JOY_U, 100, 6);
//    bsp_SetKeyParam(KID_JOY_D, 100, 6);
//    bsp_SetKeyParam(KID_JOY_L, 100, 6);
//    bsp_SetKeyParam(KID_JOY_R, 100, 6);
}
/*
*********************************************************************************************************
*    函 数 名: KeyPinActive
*    功能说明: 判断按键是否按下
*    形    参: 无
*    返 回 值: 返回值1 表示按下(导通），0表示未按下（释放）
*********************************************************************************************************
*/
static unsigned char KeyPinActive(unsigned char _id)
{
	unsigned char level;
	static unsigned char _id_temp;
	_id_temp = _id;
	
	if ((s_gpio_list[_id].gpio->IDR & s_gpio_list[_id].pin) == 0)
	{
			level = 0;
	}
	else
	{
			level = 1;
	}

	//判断寄存器中的值是否和结构体中值相同（结构体中的应该是满足滤波条件后才会更改，中断中处理）
	if (level == s_gpio_list[_id].ActiveLevel)
	{
			return 1;
	}
	else
	{
			return 0;
	}
}

/*
*********************************************************************************************************
*    函 数 名: IsKeyDownFunc
*    功能说明: 判断按键是否按下。单键和组合键区分。单键事件不允许有其他键按下。
*    形    参: 无
*    返 回 值: 返回值1 表示按下(导通），0表示未按下（释放）
*********************************************************************************************************
*/
static unsigned char IsKeyDownFunc(unsigned char _id)
{
    /* 实体单键 */
    if (_id < HARD_KEY_NUM)
    {
        unsigned char i;
        unsigned char count = 0;
        unsigned char save = 255;
        
        /* 判断有几个键按下 */
        for (i = 0; i < HARD_KEY_NUM; i++)
        {
            if (KeyPinActive(i)) 
            {
                count++;
                save = i;
            }
						//如果有两个按键按下，且不是组合键
						if(count > 1) break;
        }
        
        if (count == 1 && save == _id)
        {
            return 1;    /* 只有1个键按下时才有效 */
        }        

        return 0;
    }
    
    /* 组合键 K1K2 */
    if (_id == HARD_KEY_NUM + 0)
    {
        if (KeyPinActive(KID_K1) && KeyPinActive(KID_K2))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

    /* 组合键 K2K3 */
    if (_id == HARD_KEY_NUM + 1)
    {
        if (KeyPinActive(KID_K2) && KeyPinActive(KID_K3))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

    return 0;
}


/*
*********************************************************************************************************
*    函 数 名: bsp_InitKeyHard
*    功能说明: 配置按键对应的GPIO
*    形    参:  无
*    返 回 值: 无
*********************************************************************************************************
*/
static void bsp_InitKeyHard(void)
{    
	GPIO_InitTypeDef GPIO_InitStructure;
	unsigned char i;

	//1、开启时钟
	for (i = 0; i < HARD_KEY_NUM; i++)
	{
		RCC_AHB1PeriphClockCmd(s_gpio_list[i].rcc_gpio,ENABLE);   //开启片外ADC时钟
	}
	
	//2、初始化按键GPIO结构体
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	for (i = 0; i < HARD_KEY_NUM; i++)
	{
		GPIO_InitStructure.GPIO_Pin = s_gpio_list[i].pin;
		GPIO_Init(s_gpio_list[i].gpio,&GPIO_InitStructure);
	}
}



/*
*********************************************************************************************************
*    函 数 名: bsp_DetectKey
*    功能说明: 检测一个按键。非阻塞状态，必须被周期性的调用。
*    形    参: IO的id， 从0开始编码
*    返 回 值: 无
*********************************************************************************************************
*/
static void bsp_DetectKey(uint8_t i)
{
    KEY_T *pBtn;

    pBtn = &s_tBtn[i];
    if (IsKeyDownFunc(i))
    {
        if (pBtn->Count < KEY_FILTER_TIME)
        {
            pBtn->Count = KEY_FILTER_TIME;
        }
        else if(pBtn->Count < 2 * KEY_FILTER_TIME)
        {
            pBtn->Count++;
        }
        else
        {
            if (pBtn->State == 0)
            {
                pBtn->State = 1;

                /* 发送按钮按下的消息 */
                bsp_PutKey((uint8_t)(3 * i + 1));
            }

            if (pBtn->LongTime > 0)
            {
                if (pBtn->LongCount < pBtn->LongTime)
                {
                    /* 发送按钮持续按下的消息 */
                    if (++pBtn->LongCount == pBtn->LongTime)
                    {
                        /* 键值放入按键FIFO */
                        bsp_PutKey((uint8_t)(3 * i + 3));
                    }
                }
                else
                {
                    if (pBtn->RepeatSpeed > 0)
                    {
                        if (++pBtn->RepeatCount >= pBtn->RepeatSpeed)
                        {
                            pBtn->RepeatCount = 0;
                            /* 常按键后，每隔RepeatSpeed * 10ms发送1个按键 */
                            bsp_PutKey((uint8_t)(3 * i + 1));
                        }
                    }
                }
            }
        }
    }
    else
    {
        if(pBtn->Count > KEY_FILTER_TIME)
        {
            pBtn->Count = KEY_FILTER_TIME;
        }
        else if(pBtn->Count != 0)
        {
            pBtn->Count--;
        }
        else
        {
            if (pBtn->State == 1)
            {
                pBtn->State = 0;

                /* 发送按钮弹起的消息 */
                bsp_PutKey((uint8_t)(3 * i + 2));
            }
        }

        pBtn->LongCount = 0;
        pBtn->RepeatCount = 0;
    }
}

/*
*********************************************************************************************************
*    函 数 名: bsp_KeyScan10ms
*    功能说明: 扫描所有按键。非阻塞，被systick中断周期性的调用，10ms一次
*    形    参: 无
*    返 回 值: 无
*********************************************************************************************************
*/
void bsp_KeyScan10ms(void)
{
    uint8_t i;

    for (i = 0; i < KEY_COUNT; i++)
    {
        bsp_DetectKey(i);
    }
}

