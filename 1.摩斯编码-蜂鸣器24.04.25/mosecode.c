#include "mosecode.h"
#include "stdio.h"
#include "string.h"
#include "define.h"


// ----------------------------------
// 变量定义
// ----------------------------------
static char singal_morse[16];


// ----------------------------------
// 函数声明
// ----------------------------------
void beep_time(unsigned time);


static void convertToMorseStr(char ch);
static void convertToMorseBeep(char ch);

static void beep_morse_dot(void);
static void beep_morse_long(void);
static void beep_morse_gap(void);


// ----------------------------------
// 函数实现
// ----------------------------------
void ConvertToMorse(char * str,unsigned int length)
{
	int i;
	for(i = 0; i<length; i++)
	{
//		convertToMorseStr(str[i]);  //转化成字符串
		convertToMorseBeep(str[i]);  //转换成蜂鸣器滴
	}
}

//Convert char to mosecode
static void convertToMorseStr(char ch)
{
	switch(ch)
	{
		case '0':
			printf("----- ");
		  break;
		case '1':
			printf(".---- ");
		  break;
		case '2':
			printf("..--- ");
		  break;
		case '3':
			printf("...-- ");
		  break;
		case '4':
			printf("....- ");
		  break;
		case '5':
			printf("..... ");
		  break;
		case '6':
			printf("-.... ");
		  break;
		case '7':
			printf("--... ");
		  break;
		case '8':
			printf("---.. ");
		  break;
		case '9':
			printf("----. ");
		  break;
		
		case 'a':
		case 'A':
			printf(".- ");
		  break;
		
		default:
			break;
	}
}


//Convert char to mosecode beep
static void convertToMorseBeep(char ch)
{
	int i = 0;
	
	memset(singal_morse,0,sizeof(singal_morse));
	
	switch(ch)
	{
		case '0':
			strcpy(singal_morse,"----- ");
		  break;
		case '1':
			strcpy(singal_morse,".---- ");
		  break;
		case '2':
			strcpy(singal_morse,"..--- ");
		  break;
		case '3':
			strcpy(singal_morse,"...-- ");
		  break;
		case '4':
			strcpy(singal_morse,"....- ");
		  break;
		case '5':
			strcpy(singal_morse,"..... ");
		  break;
		case '6':
			strcpy(singal_morse,"-.... ");
		  break;
		case '7':
			strcpy(singal_morse,"--... ");
		  break;
		case '8':
			strcpy(singal_morse,"---.. ");
		  break;
		case '9':
			strcpy(singal_morse,"----. ");
		  break;
		
		case 'a':
		case 'A':
			strcpy(singal_morse,".- ");
		  break;
		
		default:
			break;
	}
	
	//调用蜂鸣器转换函数
//	Ch_Convert_Beep(singal_morse);
	
	for(i = 0; i < sizeof(singal_morse); i++)
	{
		if(singal_morse[i] != 0x00)
		{
			if(singal_morse[i] == '.')
			{
				beep_morse_dot();
			}
			else if(singal_morse[i] == '-')
			{
				beep_morse_long();
			}
			else if(singal_morse[i] == ' ')
			{
				beep_morse_gap();
			}
		}
	}
}

void beep_time(unsigned int time)
{
	BEEP_ON
	DelaymS(time);
	BEEP_OFF
	DelaymS(400 - time);
}

static void beep_morse_dot(void)
{
	beep_time(50);
}

static void beep_morse_long(void)
{
	beep_time(350);
}

static void beep_morse_gap(void)
{
	beep_time(0);
}
