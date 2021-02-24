//////////////////////////////////
//   
//     超声波相关函数
//
//////////////////////////////////////


#include "hc2.h"

uint32_t time;         //用于采集超声波来回时间的计数
uint32_t delay_u;
	

//////////////////////////////////////////////////
//
//     函数：   ultrasonic_Init()
//     输入：   无
//     输出：   无
//     功能：   超声波管脚初始化
//     说明：   无
//
////////////////////////////////////////////////
void ultrasonic_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //trio
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  //echo
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);
}

///////////////////////////////////////////////////
//
//     函数：   TIME3_Init()
//     输入：   无
//     输出：   无
//     功能：   初始化定时器3，该定时器用于采集超声波
//              来回的时间
//     说明：   该函数设置了定时器TIM3的更新中断，
//              每100us中断一次，经测试，该间隔时间
//              下测得的距离更准确
//
////////////////////////////////////////////////////
/*
void TIME3_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_TimeBaseInitStructure.TIM_Period = 99;                       //计数次数
	TIM_TimeBaseInitStructure.TIM_Prescaler = 17;                    //预分频数
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
  TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);                       //打开定时器3的更新中断
}
*/

//////////////////////////////////////////////////
//
//     函数：   TIME2_Init()
//     输入：   无
//     输出：   无
//     功能：   初始化定时器2，用于精准延时
//     说明：   该函数设置了定时器的更新中断，
//              每10us中断一次
//
/////////////////////////////////////////////////
/*
void TIME2_Init(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision=0;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=39;                        //计数次数为40
	TIM_TimeBaseInitStructure.TIM_Prescaler=17;                     //预分频系数为18
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);                        //打开定时器更新中断
}
*/

//////////////////////////////////////////////////
//
//     函数：   delay_usx()
//     输入：   time1（延时多少个10us）
//     输出：   无
//     功能：   用于延时 X*10 微秒
//     说明：   打开TIM2后，TIM2每10us中断一次，每次
//              中断，delay_u都减一，达到延时目的
//
////////////////////////////////////////////////////
void delay_usx(u32 time1)
{
	delay_u=time1;
	TIM_Cmd(TIM2,ENABLE);
	while(delay_u > 0);
	TIM_Cmd(TIM2,DISABLE);
}

///////////////////////////////////////////////////
//
//     函数：   GetDistance()
//     输入：   无
//     输出：   无
//     功能：   计算采集到的距离并进行显示
//     说明：   1、工作原理：trig拉高10us后，打开TIM3
//                 来采集超声波来回的时间，然后通过
//                 计算公式求得距离
//              2、求得的距离是六次距离的平均值
//
/////////////////////////////////////////////////////
void GetDistance(void)
{
	int i;
	
  for(i=5;i>0;i--)                                    //循环六次采集距离
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);
		GPIO_SetBits(GPIOA,GPIO_Pin_2);                     //拉高trig
		delay_usx(1);                                       //保持10us
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);                   //拉低trig
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==0);  //等待echo管脚输出高电平后打开TIM3计时
		TIM_Cmd(TIM3,ENABLE);                               //打开TIM3，开始获取超声波来回的时间
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==1);  //等待来回时间的结束
		TIM_Cmd(TIM3, DISABLE);                              //关闭TIM3，结束时间的采集
		count=TIM_GetCounter(TIM3);                         //获取TIM3的该次计数值，即读取TIM3寄存器CNT
		Distance0=calculer(count);	                        //调用函数计算距离
		Distance1=Distance1+Distance0;                      //距离距离的累加，用于后续计算距离平均值
		delay_usx(5000);                                    
	}	
		
	Distance1 = Distance1/6;                                //计算平均值
	// Display(Distance1);                                   //调用函数来通过指示灯反馈距离范围
	// printf("Distance:%d.",Distance1/100);                 //输出距离
	// printf("%dcm\n",Distance1%100);
	delay_usx(6000);
}

///////////////////////////////////////////////////////
//
//     函数：   calculer()
//     输入：   count（TIM3该次的计数值）
//     输出：   Distance（计算得到的距离，）
//     功能：   计算距离的核心公式
//     说明：   1、计算得到的距离单位是cm，并且放大了100倍
//              2、计算公式是：TIM3的计数次数*18/72000000*340/2*100*100
//                 其中18为TIM3的预分频系数，72000000为系统的主频率
//                 340为声速，除以2是因为来回，100是声速的米换算成厘米
//                 另一个100是放大100倍
//
////////////////////////////////////////////////////////
u32 calculer(u32 count)
{
  u32 Distance;
	Distance=((float)(count+100*time)*0.425);   //距离计算公式化简后的形式
	time=0;                                     //清空TIM3的中断次数
	
	return Distance;
	
}

//////////////////////////////////////////////////////
//
//     函数：   Display()
//     输入：   Distance（超声波采集到的距离大小）
//     输出：   无
//     功能：   将距离范围用指示灯显示
//     说明：   该函数通过指示灯将超声波采集到的距离
//              范围反馈出来
//
//////////////////////////////////////////////////////

/*
void Display(u32 Distance)
{
  if(Distance > 20000)                               //如果距离大于2m，则LED1亮
	{
	  GPIO_SetBits(GPIOB,GPIO_Pin_1|GPIO_Pin_5);
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	}
	else if((Distance>10000)&&(Distance<=20000))     //如果距离大于1m，小于2m，则LED2亮
	{
	  GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_5);
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);
	}
	else                                             //如果距离小于1m，则LED3亮
	{
	  GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	}
}
*/

/////////////////////////////////////////////////////
//
//     函数：   TIM3_IRQHandler()
//     输入：   无
//     输出：   无
//     功能：   TIM3中断函数
//     说明：   每100us进入该函数，获取超声波来回时间间隔内
//              TIM3中断的次数，用于计算时间和最后的距离
//
/////////////////////////////////////////////////////
void TIM3_IRQHandler(void)
{
  if(TIM_GetFlagStatus(TIM3, TIM_FLAG_Update)!=RESET)
	{
	  time++;                                    //中断标志自加1
		TIM_ClearFlag(TIM3,TIM_FLAG_Update);	     //清除中断标志
	}
}

///////////////////////////////////////////////////
//
//     函数：   TIM2_IRQHandler()
//     输入：   无
//     输出：   无
//     功能：   TIM2中断函数
//     说明：   每10us进入该函数，每进入该函数，
//              delay_u自减1，当减到0时，延时结束
//              
/////////////////////////////////////////////////////
void TIM2_IRQHandler(void)
{
  if(TIM_GetFlagStatus(TIM2, TIM_FLAG_Update)!=RESET)
	{
		delay_u--;                               //延时标志自减1
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);     //清空中断标志
	}
}

