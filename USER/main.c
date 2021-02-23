#include "sys.h"


//////////////////////////全局变量的定义////////////////////////////////////  
float pitch,roll,yaw; 								  			 //欧拉角(姿态角)
short aacx,aacy,aacz;													 //加速度传感器原始数据
short gyrox,gyroy,gyroz;											 //陀螺仪原始数据
float temp; 								  								 //温度
u8		recieve_bluetooth_DATA=0;								 //	蓝牙接受数据标志
////////////////////////////////////////////////////////////////////////////


int main(void)	
{ 
	delay_init();	    	           //=====延时函数初始化	
	NVIC_Configuration();					 //=====中断优先级分组,其中包含了所有的中断优先级的配置,方便管理和一次性修改。
	LED_Init();                    //=====初始化与 LED 连接的IO
	KEY_Init();                    //=====按键初始化
	uart1_init(115200);	           //=====串口1初始化
	TIM3_Init(99,7199);	    		   //=====定时器初始化 10ms中断一次
	OLED_Init();                   //=====OLED初始化
	OLED_Clear();									 //=====OLED清屏
	MPU_Init();					    			 //=====初始化MPU6050
	mpu_dmp_init();								 //=====初始化MPU6050的DMP模式	
	uart3_init(115200);							 //=====串口3初始化即蓝牙初始化
	delay_ms(100);
	
	// 修改蓝牙的默认通信波特率以及蓝牙默认的名字
//	Uart3SendStr("AT\r\n");
//	Uart3SendStr("AT+NAMEBribox\r\n");//发送蓝牙模块指令--设置名字为：Bribox
//	delay_ms(100);	
//	Uart3SendStr("AT+BAUD8\r\n"); 		 //发送蓝牙模块指令,将波特率设置成115200
//	delay_ms(100);

	/*
	OLED_ShowString(0,0,"Pitch:",12);
	OLED_ShowString(0,2,"Roll :",12);
	OLED_ShowString(0,4,"Yaw  :",12);
	OLED_ShowString(0,6,"Temp :",12);
	*/
	
	Usart_SendString( DEBUG_USARTx,"蓝牙串口实验");

  while(1)	
	{
		/*
		mpu_dmp_get_data(&pitch,&roll,&yaw);			//得到姿态角即欧拉角
		temp=MPU_Get_Temperature();								//得到温度值
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据	
		
		//俯仰角
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
		
		//翻滚角
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
		
		
		//航向角
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
		
		OLED_Float(6,48,temp/100,2);							//显示温度
		printf("pitch=%.2f,roll=%.2f,yaw=%.2f,temp=%.2f\n",pitch,roll,yaw,temp/100);
		*/
		
		delay_ms(50);						 	 //=====50ms刷一次屏幕,频率就是20HZ,不需要一直刷。
		OLED_ShowString(0,2,"Bluetooth_Test",12);
		OLED_ShowString(0,4,"Recieve:",12);
		OLED_Num2(12,4,recieve_bluetooth_DATA);
		// LED=~LED;
	} 	
}

