
#include "sys.h"

int main(void)	
{ 
	delay_init();	    	           //=====延时函数初始化	
	NVIC_Configuration();					 //=====中断优先级分组
	uart1_init(128000);	           //=====串口1初始化
	LED_Init();                    //=====初始化与 LED 连接的IO
	KEY_Init();                    //=====按键初始化
	OLED_Init();                   //=====OLED初始化
	OLED_Clear();									 //=====OLED清屏
  while(1)	
	{
			delay_ms(50);				 			 					//=====50ms刷新一次屏幕即可,不需要一直刷新。
			OLED_ShowString(0,1,"DAYUTC OLED Test",14);
	} 	
}

