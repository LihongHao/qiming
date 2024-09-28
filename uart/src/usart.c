
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
* ��	 ��: void uart1_init(u32 bound)
* ��	 �ܣ�USART2��ʼ��
* ��ڲ�����bound��������
* ���ز�������
* ˵	 ���� 
****************************************************************************/
void uart_init(u32 bound)
{ //GPIO�˿�����

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //ʹ��USART2ʱ�� 

	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); //GPIOA9����ΪUSART2
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); //GPIOA10����ΪUSART2

	//USART2�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//��ʼ��PA9��PA10

	//USART2 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound; 	//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No; //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure);		//��ʼ������1	


	USART_ClearFlag(USART2, USART_FLAG_TC);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	//��������ж�

	//USART2 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; //����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure); 				//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	USART_Cmd(USART2, ENABLE);						//Enable USART2 by setting the UE (USART Enable) bit in the USART_CR1 register.
}



/****************************************************************************
* ��	 ��: int fputc(int ch, FILE *f)
* ��	 �ܣ��ض�����printf���������  
* ��ڲ�����
* ���ز�����
* ˵	 ������printf()֮��ĺ�����ʹ���˰�����ģʽ��ʹ�ñ�׼��ᵼ�³����޷�
			����,�����ǽ������:ʹ��΢��,��Ϊʹ��΢��Ļ�,����ʹ�ð�����ģʽ. 
			���ڹ������Եġ�Target��-����Code Generation���й�ѡ��Use MicroLIB����
			���Ժ�Ϳ���ʹ��printf��sprintf������  
****************************************************************************/
int fputc(int ch, FILE * f) //�ض�����printf���������	
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


