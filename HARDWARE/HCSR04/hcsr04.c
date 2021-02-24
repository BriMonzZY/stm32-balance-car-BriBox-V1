#include "hcsr04.h"

uint16_t msHcCount = 0;//ms计数

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: Delay_Ms_Ms
** 功能描述: 延时1MS (可通过仿真来判断他的准确度)            
** 参数描述：time (ms) 注意time<65535
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Delay_Ms(uint16_t time)  //延时函数
{ 
    uint16_t i,j;
    for(i=0;i<time;i++)
          for(j=0;j<10260;j++);
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: Delay_Ms_Us
** 功能描述: 延时1us (可通过仿真来判断他的准确度)
** 参数描述：time (us) 注意time<65535                 
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Delay_Us(uint16_t time)  //延时函数
{ 
    uint16_t i,j;
    for(i=0;i<time;i++)
          for(j=0;j<9;j++);
}


void Hcsr04Init(void)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(HCSR04_CLK, ENABLE);
	 
	//IO初始化
	GPIO_InitStructure.GPIO_Pin =HCSR04_TRIG;       //发送电平引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(HCSR04_PORT, HCSR04_TRIG);
	 
	GPIO_InitStructure.GPIO_Pin = HCSR04_ECHO;     //返回电平引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);  
	GPIO_ResetBits(HCSR04_PORT, HCSR04_ECHO);      
}

//////////////////////////////
//定时器中断服务函数
//////////////////////////////

void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源.
		msHcCount++;
	}
}


//打开定时器

static void OpenTimerForHc(void)
{
	TIM_SetCounter(TIM3,0);//清除计数
	msHcCount = 0;
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
}


//关闭定时器
static void CloseTimerForHc()
{
	TIM_Cmd(TIM3, DISABLE);  //使能TIMx外设
}



// 获取定时时间
uint32_t GetEchoTimer(void)
{
	uint32_t t = 0;
	t = msHcCount * 1000;//得到MS
	t += TIM_GetCounter(TIM3);//得到US
	TIM3 -> CNT = 0;  //将TIM2计数寄存器的计数值清零
	Delay_Ms(50);
	return t;
}


//一次获取超声波测距数据 两次测距之间需要相隔一段时间，隔断回响信号
//为了消除余震的影响，取五次数据的平均值进行加权滤波。
float Hcsr04GetLength(void)
{
	uint32_t t = 0;
	int i = 0;
	float lengthTemp = 0;
	float sum = 0;
	while(i != 5) {
		TRIG_Send = 1;      //发送口高电平输出
		Delay_Us(20);
		TRIG_Send = 0;
		while(ECHO_Reci == 0);      //等待接收口高电平输出
		OpenTimerForHc();        //打开定时器
		i = i + 1;
		while(ECHO_Reci == 1);
		CloseTimerForHc();        //关闭定时器
		t = GetEchoTimer();        //获取时间,分辨率为1US
		lengthTemp = ((float)t/58.0);//cm
		sum = lengthTemp + sum ;
	}
	lengthTemp = sum/5.0;  // 取均值
	return lengthTemp;
}
