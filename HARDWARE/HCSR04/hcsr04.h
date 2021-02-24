#ifndef __HCSR04_H
#define __HCSR04_H

#include "sys.h"

/* trig A2   echo  A3 */

#define HCSR04_PORT     GPIOA
#define HCSR04_CLK      RCC_APB2Periph_GPIOA
#define HCSR04_TRIG     GPIO_Pin_2
#define HCSR04_ECHO     GPIO_Pin_3

#define TRIG_Send  PAout(2) 
#define ECHO_Reci  PAin(3)

extern u16 msHcCount;
extern int	  Encoder_Left,Encoder_Right;

float Hcsr04GetLength(void);
void Hcsr04Init(void);

#endif
