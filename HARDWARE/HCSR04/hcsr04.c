#include "hcsr04.h"

uint16_t msHcCount = 0;//ms����

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: Delay_Ms_Ms
** ��������: ��ʱ1MS (��ͨ���������ж�����׼ȷ��)            
** ����������time (ms) ע��time<65535
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Delay_Ms(uint16_t time)  //��ʱ����
{ 
    uint16_t i,j;
    for(i=0;i<time;i++)
          for(j=0;j<10260;j++);
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: Delay_Ms_Us
** ��������: ��ʱ1us (��ͨ���������ж�����׼ȷ��)
** ����������time (us) ע��time<65535                 
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Delay_Us(uint16_t time)  //��ʱ����
{ 
    uint16_t i,j;
    for(i=0;i<time;i++)
          for(j=0;j<9;j++);
}


void Hcsr04Init(void)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(HCSR04_CLK, ENABLE);
	 
	//IO��ʼ��
	GPIO_InitStructure.GPIO_Pin =HCSR04_TRIG;       //���͵�ƽ����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
	GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(HCSR04_PORT, HCSR04_TRIG);
	 
	GPIO_InitStructure.GPIO_Pin = HCSR04_ECHO;     //���ص�ƽ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);  
	GPIO_ResetBits(HCSR04_PORT, HCSR04_ECHO);      
}

//////////////////////////////
//��ʱ���жϷ�����
//////////////////////////////

void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ.
		msHcCount++;
	}
}


//�򿪶�ʱ��

static void OpenTimerForHc(void)
{
	TIM_SetCounter(TIM3,0);//�������
	msHcCount = 0;
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
}


//�رն�ʱ��
static void CloseTimerForHc()
{
	TIM_Cmd(TIM3, DISABLE);  //ʹ��TIMx����
}



// ��ȡ��ʱʱ��
uint32_t GetEchoTimer(void)
{
	uint32_t t = 0;
	t = msHcCount * 1000;//�õ�MS
	t += TIM_GetCounter(TIM3);//�õ�US
	TIM3 -> CNT = 0;  //��TIM2�����Ĵ����ļ���ֵ����
	Delay_Ms(50);
	return t;
}


//һ�λ�ȡ������������� ���β��֮����Ҫ���һ��ʱ�䣬���ϻ����ź�
//Ϊ�����������Ӱ�죬ȡ������ݵ�ƽ��ֵ���м�Ȩ�˲���
float Hcsr04GetLength(void)
{
	uint32_t t = 0;
	int i = 0;
	float lengthTemp = 0;
	float sum = 0;
	while(i != 5) {
		TRIG_Send = 1;      //���Ϳڸߵ�ƽ���
		Delay_Us(20);
		TRIG_Send = 0;
		while(ECHO_Reci == 0);      //�ȴ����տڸߵ�ƽ���
		OpenTimerForHc();        //�򿪶�ʱ��
		i = i + 1;
		while(ECHO_Reci == 1);
		CloseTimerForHc();        //�رն�ʱ��
		t = GetEchoTimer();        //��ȡʱ��,�ֱ���Ϊ1US
		lengthTemp = ((float)t/58.0);//cm
		sum = lengthTemp + sum ;
	}
	lengthTemp = sum/5.0;  // ȡ��ֵ
	return lengthTemp;
}
