#ifndef __GENERAL_TIM_H
#define	__GENERAL_TIM_H

#include "stm32f4xx.h"

extern  unsigned int i_timer_num;
extern unsigned int bt_Timer_start_flg;

#define GENERAL_TIM           		TIM2
#define GENERAL_TIM_CLK       		RCC_APB1Periph_TIM2

#define GENERAL_TIM_IRQn					TIM2_IRQn
#define GENERAL_TIM_IRQHandler    TIM2_IRQHandler


void 	TIMx_Configuration(void);
void  GENERAL_TIM_IRQHandler (void);


#endif /* __GENERAL_TIM_H */

