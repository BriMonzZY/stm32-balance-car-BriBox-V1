#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"

#define PI 3.14159265
void EXTI9_5_IRQHandler(void);
int balance_UP(float Angle,float Mechanical_balance,float Gyro);
int velocity(int encoder_left,int encoder_right);
int turn(int encoder_left,int encoder_right,float gyro);

extern float recive_distance;
extern float pitch,roll,yaw;
extern short aacx,aacy,aacz;													 //加速度传感器原始数据
extern short gyrox,gyroy,gyroz;											 //陀螺仪原始数据

extern float balance_UP_KP; 	 // 小车直立环PD参数
extern float balance_UP_KD;

extern float Velocity_Kp;     // 小车速度环PI参数
extern float Velocity_Ki;

extern uint8_t Performance;
extern float Mechanical_angle;

#endif
