
#include "led.h"
#include "beep.h"
#include "key.h"
#include "exti.h"



int main(void)
{
	u8				t	= 0;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����ϵͳ�ж����ȼ�����2
	delay_init();									//��ʱ������ʼ��
	LED_Init(); 									//LED��ʼ��
	BEEP_Init();									//��������ʼ��
	EXTIX_Init();									//��ʼ���ⲿ�ж����� 


	while (1)
		{
		if (t == 200)
			{
			LED2				= !LED2;			//LED2 1����˸һ����ʾϵͳ��������
			t					= 0;
			}

		t++;
		delay_ms(5);
		}
}


