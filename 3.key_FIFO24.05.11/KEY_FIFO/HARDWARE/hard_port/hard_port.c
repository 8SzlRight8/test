#include "hard_port.h"


void hard_port(void)
{
	uint8_t ucKeyCode;
	
	//1、硬件初始化
	Debug_USART_Config();
	TIMx_Configuration();
	bsp_InitKey(); //按键初始化
	

	bsp_PutKey((uint8_t)(3 * 0 + 1));
	
	while(1)
	{
		ucKeyCode = bsp_GetKey();    /* 读取键值, 无键按下时返回 KEY_NONE = 0 */
		
		if (ucKeyCode != KEY_NONE)
		{
			switch(ucKeyCode)
			{
				case KEY_1_DOWN:
						printf("KEY_1_DOWN\n");
					break;
				case KEY_1_UP:
						printf("KEY_1_UP\n");
					break;
				case KEY_1_LONG:
						printf("KEY_1_LONG\n");
					break;
				case KEY_2_DOWN:
						printf("KEY_2_DOWN\n");
					break;
				case KEY_2_UP:
						printf("KEY_2_UP\n");
					break;
				case KEY_2_LONG:
						printf("KEY_2_LONG\n");
					break;
				case KEY_3_DOWN:
						printf("KEY_3_DOWN\n");
					break;
				case KEY_3_UP:
						printf("KEY_3_UP\n");
					break;
				case KEY_3_LONG:
						printf("KEY_3_LONG\n");
					break;
				
				case KEY_4_DOWN:
						printf("KEY_4_DOWN\n");
					break;
				case KEY_4_UP:
						printf("KEY_4_UP\n");
					break;
				case KEY_4_LONG:
						printf("KEY_4_LONG\n");
					break;
				case KEY_5_DOWN:
						printf("KEY_5_DOWN\n");
					break;
				case KEY_5_UP:
						printf("KEY_5_UP\n");
					break;
				case KEY_5_LONG:
						printf("KEY_5_LONG\n");
					break;
				case KEY_6_DOWN:
						printf("KEY_6_DOWN\n");
					break;
				case KEY_6_UP:
						printf("KEY_6_UP\n");
					break;
				case KEY_6_LONG:
						printf("KEY_6_LONG\n");
					break;
				
				case KEY_7_DOWN:
						printf("KEY_7_DOWN\n");
					break;
				case KEY_7_UP:
						printf("KEY_7_UP\n");
					break;
				case KEY_7_LONG:
						printf("KEY_7_LONG\n");
					break;
				case KEY_8_DOWN:
						printf("KEY_8_DOWN\n");
					break;
				case KEY_8_UP:
						printf("KEY_8_UP\n");
					break;
				case KEY_8_LONG:
						printf("KEY_8_LONG\n");
					break;
				
				//组合键
				case KEY_9_DOWN:
						printf("KEY_9_DOWN\n");
					break;
				case KEY_9_UP:
						printf("KEY_9_UP\n");
					break;
				case KEY_9_LONG:
						printf("KEY_9_LONG\n");
					break;
				case KEY_10_DOWN:
						printf("KEY_10_DOWN\n");
					break;
				case KEY_10_UP:
						printf("KEY_10_UP\n");
					break;
				case KEY_10_LONG:
						printf("KEY_10_LONG\n");
					break;
				
			}
		}
	}
	
}


