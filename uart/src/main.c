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

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置系统中断优先级分组2
	delay_init();									//延时函数初始化
	LED_Init(); 									//LED初始化
	BEEP_Init();									//蜂鸣器初始化
	EXTIX_Init();									//初始化外部中断输入 
	uart_init(115200);

	//USART1_Init();
	while (1)
		{
		if (t == 200)
			{
			LED2				= !LED2;			//LED2 1秒闪烁一次提示系统正在运行
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


