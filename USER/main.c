#include "sys.h"


//////////////////////////ȫ�ֱ����Ķ���////////////////////////////////////  
float pitch,roll,yaw; 								  			 //ŷ����(��̬��)
short aacx,aacy,aacz;													 //���ٶȴ�����ԭʼ����
short gyrox,gyroy,gyroz;											 //������ԭʼ����
float temp; 								  								 //�¶�
u8		recieve_bluetooth_DATA=0;								 //	�����������ݱ�־
////////////////////////////////////////////////////////////////////////////


int main(void)	
{ 
	delay_init();	    	           //=====��ʱ������ʼ��	
	NVIC_Configuration();					 //=====�ж����ȼ�����,���а��������е��ж����ȼ�������,��������һ�����޸ġ�
	LED_Init();                    //=====��ʼ���� LED ���ӵ�IO
	KEY_Init();                    //=====������ʼ��
	uart1_init(115200);	           //=====����1��ʼ��
	TIM3_Init(99,7199);	    		   //=====��ʱ����ʼ�� 10ms�ж�һ��
	OLED_Init();                   //=====OLED��ʼ��
	OLED_Clear();									 //=====OLED����
	MPU_Init();					    			 //=====��ʼ��MPU6050
	mpu_dmp_init();								 //=====��ʼ��MPU6050��DMPģʽ	
	uart3_init(115200);							 //=====����3��ʼ����������ʼ��
	delay_ms(100);
	
	// �޸�������Ĭ��ͨ�Ų������Լ�����Ĭ�ϵ�����
//	Uart3SendStr("AT\r\n");
//	Uart3SendStr("AT+NAMEBribox\r\n");//��������ģ��ָ��--��������Ϊ��Bribox
//	delay_ms(100);	
//	Uart3SendStr("AT+BAUD8\r\n"); 		 //��������ģ��ָ��,�����������ó�115200
//	delay_ms(100);

	/*
	OLED_ShowString(0,0,"Pitch:",12);
	OLED_ShowString(0,2,"Roll :",12);
	OLED_ShowString(0,4,"Yaw  :",12);
	OLED_ShowString(0,6,"Temp :",12);
	*/
	
	Usart_SendString( DEBUG_USARTx,"��������ʵ��");

  while(1)	
	{
		/*
		mpu_dmp_get_data(&pitch,&roll,&yaw);			//�õ���̬�Ǽ�ŷ����
		temp=MPU_Get_Temperature();								//�õ��¶�ֵ
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������	
		
		//������
		if(pitch<0)		
		{
			OLED_ShowString(48,0,"-",12); 
			OLED_Float(0,56,-pitch,3);
		}	
		else	
		{
			OLED_ShowString(48,0,"+",12); 
			OLED_Float(0,56,pitch,3);			
		}
		
		//������
		if(roll<0)		
		{
			OLED_ShowString(48,2,"-",12); 
			OLED_Float(2,56,-roll,3);
		}	
		else	
		{
			OLED_ShowString(48,2,"+",12); 
			OLED_Float(2,56,roll,3);			
		}
		
		
		//�����
		if(yaw<0)		
		{
			OLED_ShowString(48,4,"-",12); 
			OLED_Float(4,56,-yaw,3);
		}	
		else	
		{
			OLED_ShowString(48,4,"+",12); 
			OLED_Float(4,56,yaw,3);			
		}
		
		OLED_Float(6,48,temp/100,2);							//��ʾ�¶�
		printf("pitch=%.2f,roll=%.2f,yaw=%.2f,temp=%.2f\n",pitch,roll,yaw,temp/100);
		*/
		
		delay_ms(50);						 	 //=====50msˢһ����Ļ,Ƶ�ʾ���20HZ,����Ҫһֱˢ��
		OLED_ShowString(0,2,"Bluetooth_Test",12);
		OLED_ShowString(0,4,"Recieve:",12);
		OLED_Num2(12,4,recieve_bluetooth_DATA);
		// LED=~LED;
	} 	
}

