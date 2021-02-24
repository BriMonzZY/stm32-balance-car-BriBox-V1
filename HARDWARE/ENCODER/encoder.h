#ifndef __ENCODER_H
#define __ENCODER_H

#include "sys.h"	 

#define ENCODER_TIM_PERIOD (uint16_t)(65535)   //103�Ķ�ʱ����16λ 2��16�η������65536
void Encoder_Init_TIM2(void);
void Encoder_Init_TIM4(void);
int Read_Encoder(uint8_t TIMX);
void TIM4_IRQHandler(void);
void TIM2_IRQHandler(void);

#endif
