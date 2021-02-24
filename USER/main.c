#include "sys.h"


//////////////////////////全局变量的定义////////////////////////////////////  
float temp; 								  								 //温度
u8		recieve_bluetooth_DATA=0;								 //	蓝牙接受数据标志
float recive_distance = 0;

float pitch,roll,yaw; 								  			 //欧拉角(姿态角)
short aacx,aacy,aacz;													 //加速度传感器原始数据
short gyrox,gyroy,gyroz;											 //陀螺仪原始数据
float UltrasonicWave_Distance;                 //超声波测距
int   Encoder_Left,Encoder_Right;         		 //左右编码器的脉冲计数
int 	Moto1=0,Moto2=0;												 //计算出来的最终赋给电机的PWM
int 	Flag_Qian,Flag_Hou,Flag_Left,Flag_Right; //蓝牙遥控相关的变量
u8    flag_UltrasonicWave=0;									 //超声波是否超出安全距离标志位
u8    key=0;								 									 //按键的键值
////////////////////////////////////////////////////////////////////////////


int main(void)	
{ 	
	LED_Init();                    //=====初始化与 LED 连接的IO
	KEY_Init();                    //=====初始化与按键连接的IO
	delay_init();	    	           //=====延时函数初始化	
	uart1_init(115200);	           //=====串口1初始化
	delay_ms(100);
	uart3_init(115200);            //=====初始化串口3
	NVIC_Configuration();					 //=====中断优先级分组,其中包含了所有的中断优先级的配置,方便管理和一次性修改。
	Encoder_Init_TIM2();           //=====初始化编码器2
	TIM3_Init(1000-1,72-1);	    	 //=====定时器初始化 1ms中断一次
	Encoder_Init_TIM4();           //=====初始化编码器4
	OLED_Init();                   //=====OLED初始化
	OLED_Clear();									 //=====OLED清屏
	MPU_Init();					    			 //=====初始化MPU6050
	mpu_dmp_init();								 //=====初始化MPU6050的DMP模式					 
	Hcsr04Init();									 //=====初始化超声波的硬件IO口
	TIM1_PWM_Init(7199,0);   			 //=====初始化PWM 10KHZ,用于驱动电机。 
	delay_ms(1000);								 //=====延时1s 解决小车上电轮子乱转的问题
	delay_ms(1000);								 //=====延时1s 解决小车上电轮子乱转的问题
	Motor_Init();									 //=====初始化与电机连接的硬件IO接口 
	MPU6050_EXTI_Init();					 //=====MPU6050 5ms定时中断初始化
	// oled_first_show();					   //只需要显示一次的字符,在此刷新一次即可。
	
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
		mpu_dmp_get_data(&pitch,&roll,&yaw);			//得到姿态角即欧拉角
		temp = MPU_Get_Temperature();								//得到温度值
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
		
		
		
		
		
		delay_ms(50);						 	 //=====50ms刷一次屏幕,频率就是20HZ,不需要一直刷。
		OLED_ShowString(0,2,"Bluetooth_Test",12);
		OLED_ShowString(0,4,"Recieve:",12);
		OLED_Num2(12,4,recieve_bluetooth_DATA);
		
	
		
		// 要将测距放入定时器中断中
		// OLED_ShowString(0, 2, "HCSR04_Test", 12);
		recive_distance = Hcsr04GetLength();
		OLED_Float(4, 0, recive_distance, 3);
		
		
		delay_ms(50);
		OLED_ShowString(0,0,"DAYUTC--OLED",12);
		OLED_ShowString(0,4,"L: ",12);
		OLED_ShowString(60,4,"R: ",12);
		OLED_Num2(4,4,Encoder_Left);					//显示右边电机的编码器值
		OLED_Num2(14,4,Encoder_Right);				//显示左边电机的编码器值
		
		LED=~LED;	
		*/
		
	} 	
}

