
#include "sys.h"

int main(void)	
{ 
	delay_init();	    	           //=====��ʱ������ʼ��	
	NVIC_Configuration();					 //=====�ж����ȼ�����
	uart1_init(128000);	           //=====����1��ʼ��
	LED_Init();                    //=====��ʼ���� LED ���ӵ�IO
	KEY_Init();                    //=====������ʼ��
	OLED_Init();                   //=====OLED��ʼ��
	OLED_Clear();									 //=====OLED����
  while(1)	
	{
			delay_ms(50);				 			 					//=====50msˢ��һ����Ļ����,����Ҫһֱˢ�¡�
			OLED_ShowString(0,1,"DAYUTC OLED Test",14);
	} 	
}

