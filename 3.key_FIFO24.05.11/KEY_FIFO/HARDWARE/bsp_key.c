//------------------------
// ͷ �� ��
//------------------------
#include "bsp_key.h"

//------------------------
// �� �� �� �� ��
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
// �� �� �� �� ��
//------------------------
static void bsp_InitKeyVar(void);
static void bsp_InitKeyHard(void);

static unsigned char IsKeyDownFunc(unsigned char _id);
static unsigned char KeyPinActive(unsigned char _id);
static void bsp_DetectKey(uint8_t i);

/*
*********************************************************************************************************
*    �� �� ��: bsp_PutKey
*    ����˵��: ��1����ֵѹ�밴��FIFO��������������ģ��һ��������
*    ��    ��: _KeyCode : ��������
*    �� �� ֵ: ��
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
*    �� �� ��: bsp_GetKey
*    ����˵��: �Ӱ���FIFO��������ȡһ����ֵ��
*    ��    ��:  ��
*    �� �� ֵ: ��������
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
*    �� �� ��: bsp_GetKey2
*    ����˵��: �Ӱ���FIFO��������ȡһ����ֵ�������Ķ�ָ�롣
*    ��    ��:  ��
*    �� �� ֵ: ��������
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


//������⺯��
/*
*********************************************************************************************************
*    �� �� ��: bsp_InitKey
*    ����˵��: ��ʼ������. �ú����� bsp_Init() ���á�
*    ��    ��: ��
*    �� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitKey(void)
{
    bsp_InitKeyVar();        /* ��ʼ���������� */
    bsp_InitKeyHard();        /* ��ʼ������Ӳ�� */
}

/*
*********************************************************************************************************
*    �� �� ��: bsp_InitKeyVar
*    ����˵��: ��ʼ����������
*    ��    ��:  ��
*    �� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_InitKeyVar(void)
{
    unsigned char i;

    /* �԰���FIFO��дָ������ */
    s_tKey.Read = 0;
    s_tKey.Write = 0;
    s_tKey.Read2 = 0;

    /* ��ÿ�������ṹ���Ա������һ��ȱʡֵ */
    for (i = 0; i < KEY_COUNT; i++)
    {
        s_tBtn[i].LongTime = KEY_LONG_TIME;            /* ����ʱ�� 0 ��ʾ����ⳤ�����¼� */
        s_tBtn[i].Count = KEY_FILTER_TIME / 2;        /* ����������Ϊ�˲�ʱ���һ�� */
        s_tBtn[i].State = 0;                        /* ����ȱʡ״̬��0Ϊδ���� */
        s_tBtn[i].RepeatSpeed = 10;                    /* �����������ٶȣ�0��ʾ��֧������.n*10ms ����һ�� */
        s_tBtn[i].RepeatCount = 0;                    /* ���������� */
    }

    /* �����Ҫ��������ĳ�������Ĳ����������ڴ˵������¸�ֵ */
    
    /* ҡ���������ң�֧�ֳ���1����Զ����� */
//    bsp_SetKeyParam(KID_JOY_U, 100, 6);
//    bsp_SetKeyParam(KID_JOY_D, 100, 6);
//    bsp_SetKeyParam(KID_JOY_L, 100, 6);
//    bsp_SetKeyParam(KID_JOY_R, 100, 6);
}
/*
*********************************************************************************************************
*    �� �� ��: KeyPinActive
*    ����˵��: �жϰ����Ƿ���
*    ��    ��: ��
*    �� �� ֵ: ����ֵ1 ��ʾ����(��ͨ����0��ʾδ���£��ͷţ�
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

	//�жϼĴ����е�ֵ�Ƿ�ͽṹ����ֵ��ͬ���ṹ���е�Ӧ���������˲�������Ż���ģ��ж��д���
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
*    �� �� ��: IsKeyDownFunc
*    ����˵��: �жϰ����Ƿ��¡���������ϼ����֡������¼������������������¡�
*    ��    ��: ��
*    �� �� ֵ: ����ֵ1 ��ʾ����(��ͨ����0��ʾδ���£��ͷţ�
*********************************************************************************************************
*/
static unsigned char IsKeyDownFunc(unsigned char _id)
{
    /* ʵ�嵥�� */
    if (_id < HARD_KEY_NUM)
    {
        unsigned char i;
        unsigned char count = 0;
        unsigned char save = 255;
        
        /* �ж��м��������� */
        for (i = 0; i < HARD_KEY_NUM; i++)
        {
            if (KeyPinActive(i)) 
            {
                count++;
                save = i;
            }
						//����������������£��Ҳ�����ϼ�
						if(count > 1) break;
        }
        
        if (count == 1 && save == _id)
        {
            return 1;    /* ֻ��1��������ʱ����Ч */
        }        

        return 0;
    }
    
    /* ��ϼ� K1K2 */
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

    /* ��ϼ� K2K3 */
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
*    �� �� ��: bsp_InitKeyHard
*    ����˵��: ���ð�����Ӧ��GPIO
*    ��    ��:  ��
*    �� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_InitKeyHard(void)
{    
	GPIO_InitTypeDef GPIO_InitStructure;
	unsigned char i;

	//1������ʱ��
	for (i = 0; i < HARD_KEY_NUM; i++)
	{
		RCC_AHB1PeriphClockCmd(s_gpio_list[i].rcc_gpio,ENABLE);   //����Ƭ��ADCʱ��
	}
	
	//2����ʼ������GPIO�ṹ��
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
*    �� �� ��: bsp_DetectKey
*    ����˵��: ���һ��������������״̬�����뱻�����Եĵ��á�
*    ��    ��: IO��id�� ��0��ʼ����
*    �� �� ֵ: ��
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

                /* ���Ͱ�ť���µ���Ϣ */
                bsp_PutKey((uint8_t)(3 * i + 1));
            }

            if (pBtn->LongTime > 0)
            {
                if (pBtn->LongCount < pBtn->LongTime)
                {
                    /* ���Ͱ�ť�������µ���Ϣ */
                    if (++pBtn->LongCount == pBtn->LongTime)
                    {
                        /* ��ֵ���밴��FIFO */
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
                            /* ��������ÿ��RepeatSpeed * 10ms����1������ */
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

                /* ���Ͱ�ť�������Ϣ */
                bsp_PutKey((uint8_t)(3 * i + 2));
            }
        }

        pBtn->LongCount = 0;
        pBtn->RepeatCount = 0;
    }
}

/*
*********************************************************************************************************
*    �� �� ��: bsp_KeyScan10ms
*    ����˵��: ɨ�����а���������������systick�ж������Եĵ��ã�10msһ��
*    ��    ��: ��
*    �� �� ֵ: ��
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

