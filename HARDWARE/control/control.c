#include "control.h"	

/**************************************************************************
函数功能：所有的控制代码都在这里面
         5ms定时中断由MPU6050的INT引脚触发
         严格保证采样和数据处理的时间同步	
				 在MPU6050的采样频率设置中，设置成100HZ，即可保证6050的数据是10ms更新一次。
				 imv_mpu.h文件第26行的宏定义进行修改(#define DEFAULT_MPU_HZ  (100))
**************************************************************************/
int Balance_Pwm,Velocity_Pwm,Turn_Pwm;
uint8_t Performance = 0;  // 性能模式标志位


float Mechanical_angle = -0.3;   //0

float balance_UP_KP = 310; 	 // 小车直立环PD参数  500时出现大幅度抖动   300
float balance_UP_KD = 1;    // 极性为正        1.7出现高频振动    1  

#define VKP -76.5
float Velocity_Kp = VKP;     // 小车速度环PI参数   极性为负
float Velocity_Ki = VKP * 0.005;// ki = (1/200)kp


// float Velocity_Kp=-90;     // 小车速度环PI参数
// float Velocity_Ki=-0.45;

u8 UltrasonicWave_Voltage_Counter=0;


void EXTI9_5_IRQHandler(void) 
{    
	if(PBin(5)==0)		
	{		
		EXTI->PR=1<<5;                                           //===清除LINE5上的中断标志位   
		mpu_dmp_get_data(&pitch,&roll,&yaw);										 //===得到欧拉角（姿态角）的数据
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);								 //===得到陀螺仪数据
		Encoder_Left = -Read_Encoder(2);                           //===读取编码器的值，因为两个电机的旋转了180度的，所以对其中一个取反，保证输出极性一致
		Encoder_Right = Read_Encoder(4);                           //===读取编码器的值
		UltrasonicWave_Voltage_Counter++;
		if(UltrasonicWave_Voltage_Counter==10)									 //===100ms读取一次超声波的数据以及电压
		{
			UltrasonicWave_Voltage_Counter=0;
			UltrasonicWave_StartMeasure();												 //===读取超声波的值
		}
		Balance_Pwm =balance_UP(pitch,Mechanical_angle,gyroy);   //===直立环PID控制	
		Velocity_Pwm = velocity(Encoder_Left,Encoder_Right);       //===速度环PID控制	 
  	
		if(1==Flag_Left||1==Flag_Right)    
			Turn_Pwm =turn(Encoder_Left,Encoder_Right,gyroz);        //===转向环PID控制
		else 
			Turn_Pwm=0.8*gyroz;
		
		Moto1 = Balance_Pwm-Velocity_Pwm-Turn_Pwm;                 //===计算左轮电机最终PWM
		Moto2 = Balance_Pwm-Velocity_Pwm+Turn_Pwm;                 //===计算右轮电机最终PWM
//		Moto1 = Balance_Pwm - balance_UP_KP*Velocity_Pwm - Turn_Pwm;                 //===计算左轮电机最终PWM
//		Moto2 = Balance_Pwm - balance_UP_KP*Velocity_Pwm + Turn_Pwm;
	  Xianfu_Pwm();  																					 //===PWM限幅
		Turn_Off(pitch);																 //===检查角度以及电压是否正常
		Set_Pwm(Moto1,Moto2);                                    //===赋值给PWM寄存器  
	}       	
} 

/**************************************************************************
函数功能：直立PD控制
入口参数：角度、机械平衡角度（机械中值）、角速度
返回  值：直立控制PWM
**************************************************************************/
int balance_UP(float Angle,float Mechanical_balance,float Gyro)
{  
   float Bias;
	 int balance;
	 Bias=Angle-Mechanical_balance;    							 //===求出平衡的角度中值和机械相关
	 balance=balance_UP_KP*Bias+balance_UP_KD*Gyro;  //===计算平衡控制的电机PWM  PD控制   kp是P系数 kd是D系数 
	 return balance;
}

/**************************************************************************
函数功能：速度PI控制
入口参数：电机编码器的值
返回  值：速度控制PWM
**************************************************************************/


int velocity(int encoder_left,int encoder_right)
{  
    static float Velocity,Encoder_Least,Encoder,Movement;
	  static float Encoder_Integral;
	
		if(Flag_Qian == 1)				
		{
			if (Performance == 1) Movement = -150;
			else Movement = -50;
		}
    else if(Flag_Hou == 1)//接收到蓝牙APP遥控数据		
		{
			if (Performance == 1) Movement = 150;
			else Movement = 50;//设定速度
		}
	
		//当超声波的距离低于10cm时,即10cm处存在障碍物,将超声波标志位置并且赋积分值使其后退,这里做了个简单的P比例计算
		else if(UltrasonicWave_Distance<10&&UltrasonicWave_Distance>3)
		{
			flag_UltrasonicWave=1;
			Movement=UltrasonicWave_Distance*50;		
		}
		
		else//没有接受到任何的数据
		{	
			flag_UltrasonicWave=0;
			Movement=0;
		}
   // 速度PI控制器
		Encoder_Least =(Encoder_Left +  Encoder_Right)-0;                    //获取最新速度偏差==测量速度（左右编码器之和）-目标速度（此处为零） 
		Encoder *= 0.8;		                                                //一阶低通滤波器       
		Encoder += Encoder_Least*0.2;	                                    //===一阶低通滤波器    
		Encoder_Integral += Encoder;                                       //===积分出位移 积分时间：10ms
		Encoder_Integral = Encoder_Integral - Movement;                       //===接收遥控器数据，控制前进后退
		if(Encoder_Integral>10000)  	Encoder_Integral=10000;             //===积分限幅
		if(Encoder_Integral<-10000)		Encoder_Integral=-10000;            //===积分限幅	
		Velocity = Encoder * Velocity_Kp + Encoder_Integral * Velocity_Ki;        //===速度控制	
	  if(pitch<-35||pitch>35) Encoder_Integral=0;     						//===电机关闭后清除积分
	  return Velocity;
}


/*
int velocity(int encoder_left,int encoder_right)
{
	static int PWM_out,Encoder_Err,Encoder_S,EnC_Err_Lowout,EnC_Err_Lowout_last;//【2】
	float a=0.7;//【3】
	
	//1.计算速度偏差
	Encoder_Err=(encoder_left+encoder_right)-0;//舍去误差
	//2.对速度偏差进行低通滤波
	//low_out=(1-a)*Ek+a*low_out_last;
	EnC_Err_Lowout=(1-a)*Encoder_Err+a*EnC_Err_Lowout_last;//使得波形更加平滑，滤除高频干扰，防止速度突变。
	EnC_Err_Lowout_last=EnC_Err_Lowout;//防止速度过大的影响直立环的正常工作。
	//3.对速度偏差积分，积分出位移
	Encoder_S+=EnC_Err_Lowout;//【4】
	//4.积分限幅
	Encoder_S=Encoder_S>10000?10000:(Encoder_S<(-10000)?(-10000):Encoder_S);
	//5.速度环控制输出计算
	PWM_out=Velocity_Kp*EnC_Err_Lowout+Velocity_Ki*Encoder_S;//【5】
	return PWM_out;
}
*/

/**************************************************************************
函数功能：转向PD控制
入口参数：电机编码器的值、Z轴角速度
返回  值：转向控制PWM
**************************************************************************/
int turn(int encoder_left,int encoder_right,float gyro)//转向控制
{
	 static float Turn_Target,Turn,Encoder_temp,Turn_Convert=0.9,Turn_Count;
	 float Turn_Amplitude = 50,Kp=20,Kd=0;     
	  //=============遥控左右旋转部分=======================//
	  //这一部分主要是根据旋转前的速度调整速度的起始速度，增加小车的适应性
  	if(1==Flag_Left||1==Flag_Right)                      
		{
			if(++Turn_Count==1)
			Encoder_temp=myabs(encoder_left+encoder_right);      
			Turn_Convert=55/Encoder_temp;
			if(Turn_Convert<0.6)Turn_Convert=0.6;
			if(Turn_Convert>3)Turn_Convert=3;
		}	
	  else
		{
			Turn_Convert=0.9;
			Turn_Count=0;
			Encoder_temp=0;
		}			
		if(1==Flag_Left)	           Turn_Target+=Turn_Convert;
		else if(1==Flag_Right)	     Turn_Target-=Turn_Convert; 
		else Turn_Target=0;
    if(Turn_Target>Turn_Amplitude)  Turn_Target=Turn_Amplitude;    //===转向	速度限幅
	  if(Turn_Target<-Turn_Amplitude) Turn_Target=-Turn_Amplitude;
		if(Flag_Qian==1||Flag_Hou==1)  Kd=0.5;        
		else Kd=0;   //转向的时候取消陀螺仪的纠正 有点模糊PID的思想
  	//=============转向PD控制器=======================//
		Turn=-Turn_Target*Kp-gyro*Kd;                 //===结合Z轴陀螺仪进行PD控制
	  return Turn;
}
