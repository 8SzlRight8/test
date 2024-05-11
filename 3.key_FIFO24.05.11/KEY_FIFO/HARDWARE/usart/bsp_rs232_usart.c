/**
  ******************************************************************************
  * @file    bsp_rs232_usart.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ʹ�ô���6���ض���c��printf������usart�˿ڣ��жϽ���ģʽ
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F429 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "./usart/bsp_rs232_usart.h"
#include "bsp_general_tim.h"
//#include "ExtVar.h" 


 /**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
	
uint16_t data_length;								/// ���ݳ���
uint8_t rx_buffer[DATA_BUF_SIZE] ;		/// ���ջ�����
uint8_t tx_buffer[DATA_BUF_SIZE];		/// ���ͻ�����


unsigned int bt_rx_over_flg = 0;
unsigned int i_rx_ord = 0;

static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* �����ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = RS232_USART_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}


 /**
  * @brief  RS232_USART GPIO ����,����ģʽ���á�115200 8-N-1 ���жϽ���ģʽ
  * @param  ��
  * @retval ��
  */
void Debug_USART_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd( RS232_USART_RX_GPIO_CLK|RS232_USART_TX_GPIO_CLK, ENABLE);

  /* ʹ�� UART ʱ�� */
  RCC_APB2PeriphClockCmd(RS232_USART_CLK, ENABLE);
  
  /* ���� PXx �� USARTx_Tx*/
  GPIO_PinAFConfig(RS232_USART_RX_GPIO_PORT,RS232_USART_RX_SOURCE, RS232_USART_RX_AF);

  /*  ���� PXx �� USARTx__Rx*/
  GPIO_PinAFConfig(RS232_USART_TX_GPIO_PORT,RS232_USART_TX_SOURCE,RS232_USART_TX_AF);

  /* ����Tx����Ϊ���ù���  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = RS232_USART_TX_PIN  ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RS232_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* ����Rx����Ϊ���ù��� */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = RS232_USART_RX_PIN;
  GPIO_Init(RS232_USART_RX_GPIO_PORT, &GPIO_InitStructure);
			
  /* ���ô���RS232_USART ģʽ */
  USART_InitStructure.USART_BaudRate = RS232_USART_BAUDRATE;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(RS232_USART, &USART_InitStructure); 
	
	NVIC_Configuration();
	/*���ô��ڽ����ж�*/
	USART_ITConfig(RS232_USART, USART_IT_RXNE, ENABLE);
	
  USART_Cmd(RS232_USART, ENABLE);
}

/**
* @��			��  �������ݺ���
* @��			��  *buf:���ͻ�������ַ
* @��			��	len:��Ҫ���͵����ݳ���
* @��	��	ֵ 	��
*/
void usart_Send_Data(u8 *buf,uint16_t len)
{
	uint16_t t; 
  	for(t=0;t<len;t++)		//ѭ����������
		{
			while(USART_GetFlagStatus(RS232_USART,USART_FLAG_TC)==RESET); //�ȴ����ͽ���		
			USART_SendData(RS232_USART,buf[t]); //��������
		}	 
		while(USART_GetFlagStatus(RS232_USART,USART_FLAG_TC)==RESET); //�ȴ����ͽ���	
}


/*****************  ����һ���ַ� **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/*****************  �����ַ��� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* �ȴ�������� */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/*****************  ����һ��16λ�� **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* ȡ���߰�λ */
	temp_h = (ch&0XFF00)>>8;
	/* ȡ���Ͱ�λ */
	temp_l = ch&0XFF;
	
	/* ���͸߰�λ */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* ���͵Ͱ�λ */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ����� */
		USART_SendData(RS232_USART, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(RS232_USART, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(RS232_USART, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(RS232_USART);
}
/*********************************************END OF FILE**********************/

void RS232_USART_IRQHandler(void)
{
  	//�����ж�
	if(USART_GetITStatus(RS232_USART,USART_IT_RXNE)==SET)
	{
		USART_ClearITPendingBit(RS232_USART,USART_IT_RXNE);
		UART6_Rx_deal_Fun_LY(USART_ReceiveData(RS232_USART)); //���ն���������������Ϣ
	}
	
	//���-������������Ҫ�ȶ�SR,�ٶ�DR�Ĵ��� �������������жϵ�����
	if(USART_GetFlagStatus(RS232_USART,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(RS232_USART,USART_FLAG_ORE);	//��SR
		USART_ReceiveData(RS232_USART);				//��DR
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
//			if (i_Rx_irq_num_LY  == i_Rx_comm_len_LY + 4)//������ȫ������
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
//					bt_is_convert_flg = 0xff;    //��;����ֱ�ӹر�
//				}
//			}	
//		break;
//	}			            
}
