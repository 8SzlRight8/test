/**
  ******************************************************************************
  * @file    bsp_rs232_usart.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   使用串口6，重定向c库printf函数到usart端口，中断接收模式
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F429 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "./usart/bsp_rs232_usart.h"
#include "bsp_general_tim.h"
//#include "ExtVar.h" 


 /**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
	
uint16_t data_length;								/// 数据长度
uint8_t rx_buffer[DATA_BUF_SIZE] ;		/// 接收缓冲区
uint8_t tx_buffer[DATA_BUF_SIZE];		/// 发送缓冲区


unsigned int bt_rx_over_flg = 0;
unsigned int i_rx_ord = 0;

static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* 配置中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = RS232_USART_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}


 /**
  * @brief  RS232_USART GPIO 配置,工作模式配置。115200 8-N-1 ，中断接收模式
  * @param  无
  * @retval 无
  */
void Debug_USART_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd( RS232_USART_RX_GPIO_CLK|RS232_USART_TX_GPIO_CLK, ENABLE);

  /* 使能 UART 时钟 */
  RCC_APB2PeriphClockCmd(RS232_USART_CLK, ENABLE);
  
  /* 连接 PXx 到 USARTx_Tx*/
  GPIO_PinAFConfig(RS232_USART_RX_GPIO_PORT,RS232_USART_RX_SOURCE, RS232_USART_RX_AF);

  /*  连接 PXx 到 USARTx__Rx*/
  GPIO_PinAFConfig(RS232_USART_TX_GPIO_PORT,RS232_USART_TX_SOURCE,RS232_USART_TX_AF);

  /* 配置Tx引脚为复用功能  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = RS232_USART_TX_PIN  ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RS232_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* 配置Rx引脚为复用功能 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = RS232_USART_RX_PIN;
  GPIO_Init(RS232_USART_RX_GPIO_PORT, &GPIO_InitStructure);
			
  /* 配置串口RS232_USART 模式 */
  USART_InitStructure.USART_BaudRate = RS232_USART_BAUDRATE;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(RS232_USART, &USART_InitStructure); 
	
	NVIC_Configuration();
	/*配置串口接收中断*/
	USART_ITConfig(RS232_USART, USART_IT_RXNE, ENABLE);
	
  USART_Cmd(RS232_USART, ENABLE);
}

/**
* @描			述  发送数据函数
* @参			数  *buf:发送缓冲区地址
* @参			数	len:需要发送的数据长度
* @返	回	值 	无
*/
void usart_Send_Data(u8 *buf,uint16_t len)
{
	uint16_t t; 
  	for(t=0;t<len;t++)		//循环发送数据
		{
			while(USART_GetFlagStatus(RS232_USART,USART_FLAG_TC)==RESET); //等待发送结束		
			USART_SendData(RS232_USART,buf[t]); //发送数据
		}	 
		while(USART_GetFlagStatus(RS232_USART,USART_FLAG_TC)==RESET); //等待发送结束	
}


/*****************  发送一个字符 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* 等待发送完成 */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/*****************  发送一个16位数 **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* 取出高八位 */
	temp_h = (ch&0XFF00)>>8;
	/* 取出低八位 */
	temp_l = ch&0XFF;
	
	/* 发送高八位 */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* 发送低八位 */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		USART_SendData(RS232_USART, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(RS232_USART, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(RS232_USART, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(RS232_USART);
}
/*********************************************END OF FILE**********************/

void RS232_USART_IRQHandler(void)
{
  	//接收中断
	if(USART_GetITStatus(RS232_USART,USART_IT_RXNE)==SET)
	{
		USART_ClearITPendingBit(RS232_USART,USART_IT_RXNE);
		UART6_Rx_deal_Fun_LY(USART_ReceiveData(RS232_USART)); //接收读卡器送上来的信息
	}
	
	//溢出-如果发生溢出需要先读SR,再读DR寄存器 则可清除不断入中断的问题
	if(USART_GetFlagStatus(RS232_USART,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(RS232_USART,USART_FLAG_ORE);	//读SR
		USART_ReceiveData(RS232_USART);				//读DR
	}
}	

void UART6_Rx_deal_Fun_LY(unsigned char data)
{
//	switch (i_Rx_irq_num_LY)
//	{
//		case 0:
//			if(data == 0x68)
//			{
//				b_RX_Array_LY[i_Rx_irq_num_LY] = data;
//				i_Rx_irq_num_LY++;	
//				i_Rx_TN1ms_LY = 0;
//    			bt_Rx_Timer_start_flg_LY = 1;
//    			
//    			
//			}
//			else
//			{
//    			i_Rx_TN1ms_LY = 0;
//    			i_Rx_irq_num_LY = 0; 
//    			bt_Rx_Timer_start_flg_LY = 0;
//    			
//			}
//		break;
//		case 1:
//			if(data == 0x86)
//			{
//				b_RX_Array_LY[i_Rx_irq_num_LY] = data; 
//				i_Rx_irq_num_LY++;
//					
//			}
//			else
//			{
//				i_Rx_TN1ms_LY = 0;
//    			i_Rx_irq_num_LY = 0; 
//    			bt_Rx_Timer_start_flg_LY = 0;
//    				
//			}
//		break;
//		case 2:
//			b_RX_Array_LY[i_Rx_irq_num_LY] = data; 
//			i_Rx_irq_num_LY++;
//		break;
//		case 3:
//			b_RX_Array_LY[i_Rx_irq_num_LY] = data; 
//			i_Rx_irq_num_LY++;
//			i_Rx_comm_len_LY = b_RX_Array_LY[3];
//			i_Rx_comm_len_LY <<= 8;
//			i_Rx_comm_len_LY += b_RX_Array_LY[2];
//		break;
//		
//		default:
//			b_RX_Array_LY[i_Rx_irq_num_LY] = data;
//			i_Rx_irq_num_LY++;
//			if (i_Rx_irq_num_LY  == i_Rx_comm_len_LY + 4)//接收完全部数据
//			{
//				d_LY_RX_IRQ_DIS
//				
//				i_Rx_TN1ms_LY = 0;
//    			i_Rx_irq_num_LY = 0; 
//    			bt_Rx_Timer_start_flg_LY = 0;
//				bt_Rx_Over_flg_LY = 1;
//				
//				if(b_RX_Array_LY[4] == 0x22)  
//				{
//					bt_is_convert_flg = 0xff;    //中途将其直接关闭
//				}
//			}	
//		break;
//	}			            
}
