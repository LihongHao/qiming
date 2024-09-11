
#include "led.h"
#include "beep.h"
#include "key.h"


/*******************下面代码是通过位带操作实现IO口控制***************************/
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置系统中断优先级分组2
	delay_init();									//初始化延时函数
	LED_Init(); 									//初始化LED端口
	BEEP_Init();									//初始化蜂鸣器端口
	KEY_Init();

	uint32_t		digit, count = 0;

	while (1)
		{
		key_scan(0);

		if (keydown_data == KEY0_DATA) //key0按下后马上执行相应代码
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


