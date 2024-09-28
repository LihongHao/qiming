
#include "usart1.h"
#include "string.h"
#include "stdlib.h"  
#include "common.h"


#define USART2_CR1				(USART2_BASE +0xC)

/*********************************************************************************
**********************************************************************************
*********************************************************************************/
u8				uartReceive[USART_REC_NUM], uartSend[USART_REC_NUM]; //send and receive buffer
int 			uartRecCount, uartRecIdx, uartSndIdx, uartSndCount, uartRecEnd;

/****************************************************************************
* 名	 称: void uart1_init(u32 bound)
* 功	 能：USART2初始化
* 入口参数：bound：波特率
* 返回参数：无
* 说	 明： 
****************************************************************************/
void uart_init(u32 bound)
{ //GPIO端口设置

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //使能USART2时钟 

	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); //GPIOA9复用为USART2
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); //GPIOA10复用为USART2

	//USART2端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//初始化PA9，PA10

	//USART2 初始化设置
	USART_InitStructure.USART_BaudRate = bound; 	//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No; //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
	USART_Init(USART2, &USART_InitStructure);		//初始化串口1	


	USART_ClearFlag(USART2, USART_FLAG_TC);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	//开启相关中断

	//USART2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; //串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure); 				//根据指定的参数初始化VIC寄存器、
	USART_Cmd(USART2, ENABLE);						//Enable USART2 by setting the UE (USART Enable) bit in the USART_CR1 register.
}



/****************************************************************************
* 名	 称: int fputc(int ch, FILE *f)
* 功	 能：重定向，让printf输出到串口  
* 入口参数：
* 返回参数：
* 说	 明：因printf()之类的函数，使用了半主机模式。使用标准库会导致程序无法
			运行,以下是解决方法:使用微库,因为使用微库的话,不会使用半主机模式. 
			请在工程属性的“Target“-》”Code Generation“中勾选”Use MicroLIB“这
			样以后就可以使用printf，sprintf函数了  
****************************************************************************/
int fputc(int ch, FILE * f) //重定向，让printf输出到串口	
{
	uartSndCount		= (uartSndCount + 1) &USART_MASK;
	uartSend[uartSndCount] = ch;
	BIT_ADDR(USART2_CR1, 6) = 1;
	return ch;
}


//the USART2 interrupt handler
void USART2_IRQHandler(void)
{
	USART_TypeDef * USART = (USART_TypeDef *) USART2_BASE;
	uint16_t		status = USART->SR;

	if (status & USART_SR_RXNE)
		{
		uint8_t 		rec = USART->DR;

		uartRecCount		= (uartRecCount + 1) &USART_MASK;

		if (rec != 0x0d)
			{
			uartReceive[uartRecCount] = rec;
			USART->DR			= rec;
			}
		else 
			{
			uartRecEnd			= 1;
			uartReceive[uartRecCount] = 0;
			}
		}
	else if (status & USART_SR_TC)
		{
		if (uartSndIdx != uartSndCount)
			{
			uartSndIdx			= (uartSndIdx + 1) &USART_MASK;
			USART->DR			= uartSend[uartSndIdx];
			}
		else 
			BIT_ADDR(USART2_CR1, 6) = 0;
		}

	//USART->SR & ~1<<6;
}


void Usart_SendString(char * str)
{
	int 			k	= 0;
	uint8_t 		c;
	USART_TypeDef * USART = (USART_TypeDef *) USART2_BASE;

	c					= str[k];

	while (c != '\0')
		{
		uartSndCount		= (uartSndCount + 1) &USART_MASK;
		uartSend[uartSndCount] = c;
		c					= str[++k];
		}
}


