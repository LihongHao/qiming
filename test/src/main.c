
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

	while (1)
		{
		key_scan(0);

		if (keydown_data == KEY0_DATA) //key0按下后马上执行相应代码
			{
			LED0				= 0;
			LED1				= 0;
			LED2				= 0;
			}

		if (keyup_data == KEY1_DATA) //key1按下抬起之后执行相应代码
			{
			BEEP				= 1;
			}

		if (key_tem == KEY2_DATA && key_time > 200) //key2长按1秒后执行相应代码 由于延时5ms扫描一次按键所以5ms*200=1S
			{
			BEEP				= 0;
			LED0				= 1;
			}

		if (key_tem == KEY3_DATA && key_time > 400) //key3长按2秒后执行相应代码 由于延时5ms扫描一次按键所以5ms*400=2S
			{
			LED1				= 1;
			LED2				= 1;
			}

		delay_ms(5);
		}
}


