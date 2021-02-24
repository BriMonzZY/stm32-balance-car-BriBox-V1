#ifndef __HCSR04_2_H
#define __HCSR04_2_H


#include "sys.h" 

void Hcsr04Init(void);               //对超声波模块初始化
void UltrasonicWave_StartMeasure(void);                //开始测距，发送一个>10us的脉冲，然后测量返回的高电平时间

#endif /* __UltrasonicWave_H */

