#ifndef __SysTick_H
#define __SysTick_H

#include "sys.h"


#define MOST 72000000          //系统的主频率 72MHz

void SysTick_Init(void);
void SysTick_IRQHandler1(void);

#endif


