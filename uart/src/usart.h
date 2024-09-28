

#ifndef __USART1_H
#define __USART1_H
#include "stdio.h"	
#include "common.h" 

//////////////////////////////////////////////////////////////////////////////////	 
#define		USART_REC_NUM		0x100 	
#define		USART_MASK			0xff
extern u8				uartReceive[USART_REC_NUM], uartSend[USART_REC_NUM]; //send and receive buffer
extern int 			uartRecCount, uartRecIdx, uartSndIdx, uartSndCount, uartRecEnd;


void uart_init(u32 bound);
void Usart_SendString(char * str);

#endif


