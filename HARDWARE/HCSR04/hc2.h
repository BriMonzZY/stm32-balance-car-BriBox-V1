#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H


#include "sys.h"



extern  volatile u32 count,Distance0,Distance1;

void ultrasonic_Init(void);
void TIME3_Init(void);
u32 calculer(u32 count);
void TIME2_Init(void);
// void Display(u32 Distance);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void GetDistance(void);
void delay_usx(u32 time1);


#endif


