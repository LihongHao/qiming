
#include "led.h"
#include "beep.h"
#include "key.h"


/*******************���������ͨ��λ������ʵ��IO�ڿ���***************************/
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����ϵͳ�ж����ȼ�����2
	delay_init();									//��ʼ����ʱ����
	LED_Init(); 									//��ʼ��LED�˿�
	BEEP_Init();									//��ʼ���������˿�
	KEY_Init();

	uint32_t		digit, count = 0;

	while (1)
		{
		key_scan(0);

		if (keydown_data == KEY0_DATA) //key0���º�����ִ����Ӧ����
			{
			digit= 7;
			}

		delay_ms(5);
		count	+= 1;

		if (count > 200)
			{
			count	= 0;
			digit	+= 1;
			LED0	= (digit & 0x1) ? 0: 1;
			LED1	= (digit & 0x2) ? 0: 1;
			LED2	= (digit & 0x4) ? 0: 1;

			}
		}
}


