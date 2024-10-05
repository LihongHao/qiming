
#include "string.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "exti.h"
#include "usart.h"
#include "dma.h"

uint32_t		extTriggered[5];



int main(void)
{
	u8				t	= 0;
	int 			count = 0;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����ϵͳ�ж����ȼ�����2
	delay_init();									//��ʱ������ʼ��
	LED_Init(); 									//LED��ʼ��
	BEEP_Init();									//��������ʼ��
	EXTIX_Init();									//��ʼ���ⲿ�ж����� 
	uart_init(115200);

	//,STEAM7,CH4,����Ϊ����1,�洢��ΪSendBuff,����Ϊ:sizeof(SEND_TestDate).
	USER_DMA_Config(DMA1_Stream6, DMA_Channel_4, (u32) & (USART2->DR), (u32) uartSend, sizeof(uartSend));


	while (1)
		{

		if (t == 200)
			{
			LED2				= !LED2;			//LED2 1����˸һ����ʾϵͳ��������
			count++;
			t					= 0;
			sprintf((char *) uartSend, "LED flip: %d\r\n", count);
			USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE); //ʹ�ܴ���1��DMA����	   

			USER_DMA_Enable(DMA1_Stream6, strlen(uartSend)); //��ʼһ��DMA����  

			}

		if (uartRecEnd && (uartRecIdx < uartRecCount))
			{
			if (strcmp((char *) (uartReceive + uartRecIdx), "reset") == 0)
				{
				count				= 0;
				}

			uartRecIdx			= uartRecCount;
			uartRecEnd			= 0;
			}

		t++;
		delay_ms(5);
		}
}


