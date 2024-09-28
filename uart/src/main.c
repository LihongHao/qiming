#include "string.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "exti.h"
#include "usart1.h"

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

	//USART1_Init();
	while (1)
		{
		if (t == 200)
			{
			LED2				= !LED2;			//LED2 1����˸һ����ʾϵͳ��������
			count++;
			t					= 0;
			printf("LED flip: %d\r\n", count);
			}

		if (uartRecEnd&&(uartRecIdx<uartRecCount))
			{
			if (strcmp((char*)(uartReceive + uartRecIdx), "reset"))
				{
				count				= 0;
				uartRecEnd =0;
				uartRecIdx =uartRecCount;
				}
			}

		t++;
		delay_ms(5);
		}
}


