#include "key.h"

void KEY_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //ʹ��PC�˿�ʱ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;	           //�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         //��������
  GPIO_Init(GPIOC, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOC
} 
/**************************************************************************
�������ܣ�����ɨ��
��ڲ�������
����  ֵ������״̬ 0���޶��� 1������ 
**************************************************************************/
int KEY_Scan(void)
{
			static u8 flag_key=1;//�������ɿ���־
			if(flag_key&&KEY==0)
			{
				flag_key=0;
				return 1;	// ��������
			}
			else if(1==KEY)			
			{
				flag_key=1;
				return 0;//�ް�������
			}
}