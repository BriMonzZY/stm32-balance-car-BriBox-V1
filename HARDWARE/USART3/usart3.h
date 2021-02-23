#ifndef __USRAT3_H
#define __USRAT3_H 
#include "sys.h"	  	

#define  DEBUG_USARTx 		USART3


// extern uint16_t Usart3_Receive;
extern uint16_t uart_receive;
void uart3_init(u32 bound);
void USART3_IRQHandler(void);
void Uart3SendByte(char byte);   //串口发送一个字节
void Uart3SendBuf(char *buf, u16 len);
void Uart3SendStr(char *str);

void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);

#endif

