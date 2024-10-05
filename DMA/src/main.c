
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

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置系统中断优先级分组2
	delay_init();									//延时函数初始化
	LED_Init(); 									//LED初始化
	BEEP_Init();									//蜂鸣器初始化
	EXTIX_Init();									//初始化外部中断输入 
	uart_init(115200);

	//,STEAM7,CH4,外设为串口1,存储器为SendBuff,长度为:sizeof(SEND_TestDate).
	USER_DMA_Config(DMA1_Stream6, DMA_Channel_4, (u32) & (USART2->DR), (u32) uartSend, sizeof(uartSend));


	while (1)
		{

		if (t == 200)
			{
			LED2				= !LED2;			//LED2 1秒闪烁一次提示系统正在运行
			count++;
			t					= 0;
			sprintf((char *) uartSend, "LED flip: %d\r\n", count);
			USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE); //使能串口1的DMA发送	   

			USER_DMA_Enable(DMA1_Stream6, strlen(uartSend)); //开始一次DMA传输  

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


