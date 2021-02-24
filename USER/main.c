#include "sys.h"


//////////////////////////ȫ�ֱ����Ķ���////////////////////////////////////  
float temp; 								  								 //�¶�
u8		recieve_bluetooth_DATA=0;								 //	�����������ݱ�־
float recive_distance = 0;

float pitch,roll,yaw; 								  			 //ŷ����(��̬��)
short aacx,aacy,aacz;													 //���ٶȴ�����ԭʼ����
short gyrox,gyroy,gyroz;											 //������ԭʼ����
float UltrasonicWave_Distance;                 //���������
int   Encoder_Left,Encoder_Right;         		 //���ұ��������������
int 	Moto1=0,Moto2=0;												 //������������ո��������PWM
int 	Flag_Qian,Flag_Hou,Flag_Left,Flag_Right; //����ң����صı���
u8    flag_UltrasonicWave=0;									 //�������Ƿ񳬳���ȫ�����־λ
u8    key=0;								 									 //�����ļ�ֵ
////////////////////////////////////////////////////////////////////////////


int main(void)	
{ 	
	LED_Init();                    //=====��ʼ���� LED ���ӵ�IO
	KEY_Init();                    //=====��ʼ���밴�����ӵ�IO
	delay_init();	    	           //=====��ʱ������ʼ��	
	uart1_init(115200);	           //=====����1��ʼ��
	delay_ms(100);
	uart3_init(115200);            //=====��ʼ������3
	NVIC_Configuration();					 //=====�ж����ȼ�����,���а��������е��ж����ȼ�������,��������һ�����޸ġ�
	Encoder_Init_TIM2();           //=====��ʼ��������2
	TIM3_Init(1000-1,72-1);	    	 //=====��ʱ����ʼ�� 1ms�ж�һ��
	Encoder_Init_TIM4();           //=====��ʼ��������4
	OLED_Init();                   //=====OLED��ʼ��
	OLED_Clear();									 //=====OLED����
	MPU_Init();					    			 //=====��ʼ��MPU6050
	mpu_dmp_init();								 //=====��ʼ��MPU6050��DMPģʽ					 
	Hcsr04Init();									 //=====��ʼ����������Ӳ��IO��
	TIM1_PWM_Init(7199,0);   			 //=====��ʼ��PWM 10KHZ,������������� 
	delay_ms(1000);								 //=====��ʱ1s ���С���ϵ�������ת������
	delay_ms(1000);								 //=====��ʱ1s ���С���ϵ�������ת������
	Motor_Init();									 //=====��ʼ���������ӵ�Ӳ��IO�ӿ� 
	MPU6050_EXTI_Init();					 //=====MPU6050 5ms��ʱ�жϳ�ʼ��
	// oled_first_show();					   //ֻ��Ҫ��ʾһ�ε��ַ�,�ڴ�ˢ��һ�μ��ɡ�
	
	/*      pitch = -90
	OLED_ShowString(0,0,"Pitch:",12);
	OLED_ShowString(0,2,"Roll :",12);
	OLED_ShowString(0,4,"Yaw  :",12);
	OLED_ShowString(0,6,"Temp :",12);
	*/
	
	oled_first_show();
	 
  while(1)	
	{
		oled_show();
		delay_ms(50);
		
		/*
		mpu_dmp_get_data(&pitch,&roll,&yaw);			//�õ���̬�Ǽ�ŷ����
		temp = MPU_Get_Temperature();								//�õ��¶�ֵ
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
		
		
		
		
		
		delay_ms(50);						 	 //=====50msˢһ����Ļ,Ƶ�ʾ���20HZ,����Ҫһֱˢ��
		OLED_ShowString(0,2,"Bluetooth_Test",12);
		OLED_ShowString(0,4,"Recieve:",12);
		OLED_Num2(12,4,recieve_bluetooth_DATA);
		
	
		
		// Ҫ�������붨ʱ���ж���
		// OLED_ShowString(0, 2, "HCSR04_Test", 12);
		recive_distance = Hcsr04GetLength();
		OLED_Float(4, 0, recive_distance, 3);
		
		
		delay_ms(50);
		OLED_ShowString(0,0,"DAYUTC--OLED",12);
		OLED_ShowString(0,4,"L: ",12);
		OLED_ShowString(60,4,"R: ",12);
		OLED_Num2(4,4,Encoder_Left);					//��ʾ�ұߵ���ı�����ֵ
		OLED_Num2(14,4,Encoder_Right);				//��ʾ��ߵ���ı�����ֵ
		
		LED=~LED;	
		*/
		
	} 	
}

