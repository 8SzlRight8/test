#ifndef __RS232_USART_H
#define	__RS232_USART_H

#include "stm32f4xx.h"
#include <stdio.h>



//////蓝牙
///*******************************************************/
//#define RS232_USART                             USART6
//#define RS232_USART_CLK                         RCC_APB2Periph_USART6

//#define RS232_USART_RX_GPIO_PORT                GPIOC
//#define RS232_USART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOC
//#define RS232_USART_RX_PIN                      GPIO_Pin_7
//#define RS232_USART_RX_AF                       GPIO_AF_USART6
//#define RS232_USART_RX_SOURCE                   GPIO_PinSource7

//#define RS232_USART_TX_GPIO_PORT                GPIOC
//#define RS232_USART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOC
//#define RS232_USART_TX_PIN                      GPIO_Pin_6
//#define RS232_USART_TX_AF                       GPIO_AF_USART6
//#define RS232_USART_TX_SOURCE                   GPIO_PinSource6


//#define RS232_USART_IRQHandler                  USART6_IRQHandler
//#define RS232_USART_IRQ                 		  USART6_IRQn
///************************************************************/


//串口
/*******************************************************/
#define RS232_USART                             USART1
#define RS232_USART_CLK                         RCC_APB2Periph_USART1

#define RS232_USART_RX_GPIO_PORT                GPIOA
#define RS232_USART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define RS232_USART_RX_PIN                      GPIO_Pin_10
#define RS232_USART_RX_AF                       GPIO_AF_USART1
#define RS232_USART_RX_SOURCE                   GPIO_PinSource10

#define RS232_USART_TX_GPIO_PORT                GPIOA
#define RS232_USART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define RS232_USART_TX_PIN                      GPIO_Pin_9
#define RS232_USART_TX_AF                       GPIO_AF_USART1
#define RS232_USART_TX_SOURCE                   GPIO_PinSource9


#define RS232_USART_IRQHandler                  USART1_IRQHandler
#define RS232_USART_IRQ                 		USART1_IRQn
/************************************************************/

//串口波特率
#define RS232_USART_BAUDRATE                    115200

#define DATA_BUF_SIZE							 128

extern uint16_t data_length;
extern uint8_t rx_buffer[];
extern uint8_t tx_buffer[];

extern  unsigned int bt_rx_over_flg;
extern  unsigned int i_rx_ord;

void Debug_USART_Config(void);

void usart_Send_Data(u8 *buf,uint16_t len);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);

void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);

void RS232_USART_IRQHandler(void);
void UART6_Rx_deal_Fun_LY(unsigned char data);
//int fputc(int ch, FILE *f);

#endif /* __USART1_H */
