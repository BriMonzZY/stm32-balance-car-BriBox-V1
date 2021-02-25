#include "usart3.h"

uint16_t uart_receive;//����������ر���


// ʮ������ת10����
//#define N 100
//int i3;
/*
int main(){
	int htoi(char []);
	char a[N];
	gets(a);
	if(htoi(a))
		printf("%d\n",htoi(a));
	return 0;
}
*/

/*
int htoi(char a[]){
	int len,t,sum=0;
	len=strlen(a);
	for(i3=0;i3<len;i3++){
		if(a[i3]>='0'&&a[i3]<='9'||a[i3]>='a'&&a[i3]<='f'||a[i3]>='A'&&a[i3]<='F'){
			if(a[i3]>='0'&&a[i3]<='9')
				t=a[i3]-'0';
			else if(a[i3]>='a'&&a[i3]<='f')
				t=a[i3]-'a'+10;//ʮ������aת��Ϊʮ������10,�Դ����Ƶ�f
			else
				t=a[i3]-'A'+10;
			sum=sum*16+t;
		}
	}
	return sum;
}
*/

/**************************************************************************
�������ܣ�����3��ʼ��
��ڲ����� bound:������
����  ֵ����
**************************************************************************/
void uart3_init(u32 bound)
{  	 
	  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��UGPIOBʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//ʹ��USART3ʱ��
	//USART3_TX  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOB, &GPIO_InitStructure);
   
  //USART3_RX	  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);

   //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART3, &USART_InitStructure);     //��ʼ������3
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���3 
}
void Uart3SendByte(char byte)   //���ڷ���һ���ֽ�
{
        USART_SendData(USART3, byte);        //ͨ���⺯��  ��������
        while( USART_GetFlagStatus(USART3,USART_FLAG_TC)!= SET);  
        //�ȴ�������ɡ�   ��� USART_FLAG_TC �Ƿ���1��    //���⺯�� P359 ����
 }

void Uart3SendBuf(char *buf, u16 len)
{
	u16 i;
	for(i=0; i<len; i++)Uart3SendByte(*buf++);
}
void Uart3SendStr(char *str)
{
	u16 i,len;
	len = strlen(str);
	for(i=0; i<len; i++)Uart3SendByte(*str++);
}
/**************************************************************************
�������ܣ�����3�����ж�
��ڲ�������
����  ֵ����
**************************************************************************/
void USART3_IRQHandler(void)
{	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //���յ�����
	{	  
		static	uint16_t uart_receive = 0;
		
		uart_receive = USART_ReceiveData(USART3); 
		if     (uart_receive==0x00)			Velocity_Kp += 0.1;
		else if(uart_receive==0x01)			Velocity_Kp -= 0.1;
		else if(uart_receive==0x02)			balance_UP_KP += 1;
		else if(uart_receive==0x03)			Mechanical_angle += 0.1;
		else if(uart_receive==0x04)			{Flag_Qian = 1; Flag_Hou = 0;}   // ��ǰ��
		else if(uart_receive==0x14)			Flag_Qian = 0;  // ֹͣ��ǰ��
		else if(uart_receive==0x05)			balance_UP_KP -= 1;										
		else if(uart_receive==0x06)	  	{Flag_Left = 1; Flag_Right = 0;}  // ����
		else if(uart_receive==0x16)	  	Flag_Left = 0;  // ֹͣ����
		else if(uart_receive==0x07)			Mechanical_angle -= 0.1;
		else if(uart_receive==0x08)	  	{Flag_Right = 1; Flag_Left = 0;}  //����
		else if(uart_receive==0x18)	  	Flag_Right = 0;  //ֹͣ����
		else if(uart_receive==0x10)	  	balance_UP_KD += 0.1;
		else if(uart_receive==0x11)	  	{Flag_Hou = 1; Flag_Qian = 0;}  // �����
		else if(uart_receive==0x21)	  	Flag_Hou = 0;  // ֹͣ�����
		else if(uart_receive==0x12)	  	balance_UP_KD -= 0.1;
		
		else if(uart_receive==0x50)	  	Performance = 0;  // ����ģʽ�ر�
		else if(uart_receive==0x51)	  	Performance = 1;  // ����ģʽ��
	}  											 
} 









//����һ���ַ�
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	//����һ���ֽ����ݵ�USART
	USART_SendData(pUSARTx,ch);
		
	//�ȴ��������ݼĴ���Ϊ��
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}


//�����ַ���
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  //�ȴ��������
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}


//����һ��16λ��
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	//ȡ���߰�λ
	temp_h = (ch&0XFF00)>>8;
	//ȡ���Ͱ�λ//
	temp_l = ch&0XFF;
	
	//���͸߰�λ//
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	//���͵Ͱ�λ//
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}


/*�ض�������bug

///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		// ����һ���ֽ����ݵ����� //
		USART_SendData(DEBUG_USARTx, (uint8_t) ch);
		
		// �ȴ�������� //
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		// �ȴ������������� //
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USARTx);
}

*/
