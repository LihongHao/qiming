
#include "led.h"
#include "beep.h"
#include "key.h"
#include "exti.h"



int main(void)
{
	u8				t	= 0;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置系统中断优先级分组2
	delay_init();									//延时函数初始化
	LED_Init(); 									//LED初始化
	BEEP_Init();									//蜂鸣器初始化
	EXTIX_Init();									//初始化外部中断输入 


	while (1)
		{
		if (t == 200)
			{
			LED2				= !LED2;			//LED2 1秒闪烁一次提示系统正在运行
			t					= 0;
			}

		t++;
		delay_ms(5);
		}
}


