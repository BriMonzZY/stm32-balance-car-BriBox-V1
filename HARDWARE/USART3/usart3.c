#include "usart3.h"

uint16_t uart_receive;//蓝牙接收相关变量


// 十六进制转10进制
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
				t=a[i3]-'a'+10;//十六进制a转化为十进制是10,以此类推到f
			else
				t=a[i3]-'A'+10;
			sum=sum*16+t;
		}
	}
	return sum;
}
*/

/**************************************************************************
函数功能：串口3初始化
入口参数： bound:波特率
返回  值：无
**************************************************************************/
void uart3_init(u32 bound)
{  	 
	  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能UGPIOB时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//使能USART3时钟
	//USART3_TX  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);
   
  //USART3_RX	  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);

   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART3, &USART_InitStructure);     //初始化串口3
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART3, ENABLE);                    //使能串口3 
}
void Uart3SendByte(char byte)   //串口发送一个字节
{
        USART_SendData(USART3, byte);        //通过库函数  发送数据
        while( USART_GetFlagStatus(USART3,USART_FLAG_TC)!= SET);  
        //等待发送完成。   检测 USART_FLAG_TC 是否置1；    //见库函数 P359 介绍
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
函数功能：串口3接收中断
入口参数：无
返回  值：无
**************************************************************************/
void USART3_IRQHandler(void)
{	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //接收到数据
	{	  
		static	uint16_t uart_receive = 0;
		
		uart_receive = USART_ReceiveData(USART3); 
		if     (uart_receive==0x00)			Velocity_Kp += 0.1;
		else if(uart_receive==0x01)			Velocity_Kp -= 0.1;
		else if(uart_receive==0x02)			balance_UP_KP += 1;
		else if(uart_receive==0x03)			Mechanical_angle += 0.1;
		else if(uart_receive==0x04)			{Flag_Qian = 1; Flag_Hou = 0;}   // 向前走
		else if(uart_receive==0x14)			Flag_Qian = 0;  // 停止向前走
		else if(uart_receive==0x05)			balance_UP_KP -= 1;										
		else if(uart_receive==0x06)	  	{Flag_Left = 1; Flag_Right = 0;}  // 向左
		else if(uart_receive==0x16)	  	Flag_Left = 0;  // 停止向左
		else if(uart_receive==0x07)			Mechanical_angle -= 0.1;
		else if(uart_receive==0x08)	  	{Flag_Right = 1; Flag_Left = 0;}  //向右
		else if(uart_receive==0x18)	  	Flag_Right = 0;  //停止向右
		else if(uart_receive==0x10)	  	balance_UP_KD += 0.1;
		else if(uart_receive==0x11)	  	{Flag_Hou = 1; Flag_Qian = 0;}  // 向后走
		else if(uart_receive==0x21)	  	Flag_Hou = 0;  // 停止向后走
		else if(uart_receive==0x12)	  	balance_UP_KD -= 0.1;
		
		else if(uart_receive==0x50)	  	Performance = 0;  // 性能模式关闭
		else if(uart_receive==0x51)	  	Performance = 1;  // 性能模式打开
	}  											 
} 









//发送一个字符
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	//发送一个字节数据到USART
	USART_SendData(pUSARTx,ch);
		
	//等待发送数据寄存器为空
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}


//发送字符串
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  //等待发送完成
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}


//发送一个16位数
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	//取出高八位
	temp_h = (ch&0XFF00)>>8;
	//取出低八位//
	temp_l = ch&0XFF;
	
	//发送高八位//
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	//发送低八位//
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}


/*重定向函数有bug

///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		// 发送一个字节数据到串口 //
		USART_SendData(DEBUG_USARTx, (uint8_t) ch);
		
		// 等待发送完毕 //
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		// 等待串口输入数据 //
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USARTx);
}

*/
