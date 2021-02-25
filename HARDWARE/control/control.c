#include "control.h"	

/**************************************************************************
�������ܣ����еĿ��ƴ��붼��������
         5ms��ʱ�ж���MPU6050��INT���Ŵ���
         �ϸ�֤���������ݴ����ʱ��ͬ��	
				 ��MPU6050�Ĳ���Ƶ�������У����ó�100HZ�����ɱ�֤6050��������10ms����һ�Ρ�
				 imv_mpu.h�ļ���26�еĺ궨������޸�(#define DEFAULT_MPU_HZ  (100))
**************************************************************************/
int Balance_Pwm,Velocity_Pwm,Turn_Pwm;
uint8_t Performance = 0;  // ����ģʽ��־λ


float Mechanical_angle = -0.3;   //0

float balance_UP_KP = 310; 	 // С��ֱ����PD����  500ʱ���ִ���ȶ���   300
float balance_UP_KD = 1;    // ����Ϊ��        1.7���ָ�Ƶ��    1  

#define VKP -76.5
float Velocity_Kp = VKP;     // С���ٶȻ�PI����   ����Ϊ��
float Velocity_Ki = VKP * 0.005;// ki = (1/200)kp


// float Velocity_Kp=-90;     // С���ٶȻ�PI����
// float Velocity_Ki=-0.45;

u8 UltrasonicWave_Voltage_Counter=0;


void EXTI9_5_IRQHandler(void) 
{    
	if(PBin(5)==0)		
	{		
		EXTI->PR=1<<5;                                           //===���LINE5�ϵ��жϱ�־λ   
		mpu_dmp_get_data(&pitch,&roll,&yaw);										 //===�õ�ŷ���ǣ���̬�ǣ�������
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);								 //===�õ�����������
		Encoder_Left = -Read_Encoder(2);                           //===��ȡ��������ֵ����Ϊ�����������ת��180�ȵģ����Զ�����һ��ȡ������֤�������һ��
		Encoder_Right = Read_Encoder(4);                           //===��ȡ��������ֵ
		UltrasonicWave_Voltage_Counter++;
		if(UltrasonicWave_Voltage_Counter==10)									 //===100ms��ȡһ�γ������������Լ���ѹ
		{
			UltrasonicWave_Voltage_Counter=0;
			UltrasonicWave_StartMeasure();												 //===��ȡ��������ֵ
		}
		Balance_Pwm =balance_UP(pitch,Mechanical_angle,gyroy);   //===ֱ����PID����	
		Velocity_Pwm = velocity(Encoder_Left,Encoder_Right);       //===�ٶȻ�PID����	 
  	
		if(1==Flag_Left||1==Flag_Right)    
			Turn_Pwm =turn(Encoder_Left,Encoder_Right,gyroz);        //===ת��PID����
		else 
			Turn_Pwm=0.8*gyroz;
		
		Moto1 = Balance_Pwm-Velocity_Pwm-Turn_Pwm;                 //===�������ֵ������PWM
		Moto2 = Balance_Pwm-Velocity_Pwm+Turn_Pwm;                 //===�������ֵ������PWM
//		Moto1 = Balance_Pwm - balance_UP_KP*Velocity_Pwm - Turn_Pwm;                 //===�������ֵ������PWM
//		Moto2 = Balance_Pwm - balance_UP_KP*Velocity_Pwm + Turn_Pwm;
	  Xianfu_Pwm();  																					 //===PWM�޷�
		Turn_Off(pitch);																 //===���Ƕ��Լ���ѹ�Ƿ�����
		Set_Pwm(Moto1,Moto2);                                    //===��ֵ��PWM�Ĵ���  
	}       	
} 

/**************************************************************************
�������ܣ�ֱ��PD����
��ڲ������Ƕȡ���еƽ��Ƕȣ���е��ֵ�������ٶ�
����  ֵ��ֱ������PWM
**************************************************************************/
int balance_UP(float Angle,float Mechanical_balance,float Gyro)
{  
   float Bias;
	 int balance;
	 Bias=Angle-Mechanical_balance;    							 //===���ƽ��ĽǶ���ֵ�ͻ�е���
	 balance=balance_UP_KP*Bias+balance_UP_KD*Gyro;  //===����ƽ����Ƶĵ��PWM  PD����   kp��Pϵ�� kd��Dϵ�� 
	 return balance;
}

/**************************************************************************
�������ܣ��ٶ�PI����
��ڲ����������������ֵ
����  ֵ���ٶȿ���PWM
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
    else if(Flag_Hou == 1)//���յ�����APPң������		
		{
			if (Performance == 1) Movement = 150;
			else Movement = 50;//�趨�ٶ�
		}
	
		//���������ľ������10cmʱ,��10cm�������ϰ���,����������־λ�ò��Ҹ�����ֵʹ�����,�������˸��򵥵�P��������
		else if(UltrasonicWave_Distance<10&&UltrasonicWave_Distance>3)
		{
			flag_UltrasonicWave=1;
			Movement=UltrasonicWave_Distance*50;		
		}
		
		else//û�н��ܵ��κε�����
		{	
			flag_UltrasonicWave=0;
			Movement=0;
		}
   // �ٶ�PI������
		Encoder_Least =(Encoder_Left +  Encoder_Right)-0;                    //��ȡ�����ٶ�ƫ��==�����ٶȣ����ұ�����֮�ͣ�-Ŀ���ٶȣ��˴�Ϊ�㣩 
		Encoder *= 0.8;		                                                //һ�׵�ͨ�˲���       
		Encoder += Encoder_Least*0.2;	                                    //===һ�׵�ͨ�˲���    
		Encoder_Integral += Encoder;                                       //===���ֳ�λ�� ����ʱ�䣺10ms
		Encoder_Integral = Encoder_Integral - Movement;                       //===����ң�������ݣ�����ǰ������
		if(Encoder_Integral>10000)  	Encoder_Integral=10000;             //===�����޷�
		if(Encoder_Integral<-10000)		Encoder_Integral=-10000;            //===�����޷�	
		Velocity = Encoder * Velocity_Kp + Encoder_Integral * Velocity_Ki;        //===�ٶȿ���	
	  if(pitch<-35||pitch>35) Encoder_Integral=0;     						//===����رպ��������
	  return Velocity;
}


/*
int velocity(int encoder_left,int encoder_right)
{
	static int PWM_out,Encoder_Err,Encoder_S,EnC_Err_Lowout,EnC_Err_Lowout_last;//��2��
	float a=0.7;//��3��
	
	//1.�����ٶ�ƫ��
	Encoder_Err=(encoder_left+encoder_right)-0;//��ȥ���
	//2.���ٶ�ƫ����е�ͨ�˲�
	//low_out=(1-a)*Ek+a*low_out_last;
	EnC_Err_Lowout=(1-a)*Encoder_Err+a*EnC_Err_Lowout_last;//ʹ�ò��θ���ƽ�����˳���Ƶ���ţ���ֹ�ٶ�ͻ�䡣
	EnC_Err_Lowout_last=EnC_Err_Lowout;//��ֹ�ٶȹ����Ӱ��ֱ����������������
	//3.���ٶ�ƫ����֣����ֳ�λ��
	Encoder_S+=EnC_Err_Lowout;//��4��
	//4.�����޷�
	Encoder_S=Encoder_S>10000?10000:(Encoder_S<(-10000)?(-10000):Encoder_S);
	//5.�ٶȻ������������
	PWM_out=Velocity_Kp*EnC_Err_Lowout+Velocity_Ki*Encoder_S;//��5��
	return PWM_out;
}
*/

/**************************************************************************
�������ܣ�ת��PD����
��ڲ����������������ֵ��Z����ٶ�
����  ֵ��ת�����PWM
**************************************************************************/
int turn(int encoder_left,int encoder_right,float gyro)//ת�����
{
	 static float Turn_Target,Turn,Encoder_temp,Turn_Convert=0.9,Turn_Count;
	 float Turn_Amplitude = 50,Kp=20,Kd=0;     
	  //=============ң��������ת����=======================//
	  //��һ������Ҫ�Ǹ�����תǰ���ٶȵ����ٶȵ���ʼ�ٶȣ�����С������Ӧ��
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
    if(Turn_Target>Turn_Amplitude)  Turn_Target=Turn_Amplitude;    //===ת��	�ٶ��޷�
	  if(Turn_Target<-Turn_Amplitude) Turn_Target=-Turn_Amplitude;
		if(Flag_Qian==1||Flag_Hou==1)  Kd=0.5;        
		else Kd=0;   //ת���ʱ��ȡ�������ǵľ��� �е�ģ��PID��˼��
  	//=============ת��PD������=======================//
		Turn=-Turn_Target*Kp-gyro*Kd;                 //===���Z�������ǽ���PD����
	  return Turn;
}
