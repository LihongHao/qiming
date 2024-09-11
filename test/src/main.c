
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

	while (1)
		{
		key_scan(0);

		if (keydown_data == KEY0_DATA) //key0���º�����ִ����Ӧ����
			{
			LED0				= 0;
			LED1				= 0;
			LED2				= 0;
			}

		if (keyup_data == KEY1_DATA) //key1����̧��֮��ִ����Ӧ����
			{
			BEEP				= 1;
			}

		if (key_tem == KEY2_DATA && key_time > 200) //key2����1���ִ����Ӧ���� ������ʱ5msɨ��һ�ΰ�������5ms*200=1S
			{
			BEEP				= 0;
			LED0				= 1;
			}

		if (key_tem == KEY3_DATA && key_time > 400) //key3����2���ִ����Ӧ���� ������ʱ5msɨ��һ�ΰ�������5ms*400=2S
			{
			LED1				= 1;
			LED2				= 1;
			}

		delay_ms(5);
		}
}


