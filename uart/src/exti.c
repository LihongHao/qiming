
#include "exti.h"
#include "led.h" 
#include "key.h"
#include "beep.h"

extern uint32_t extTriggered[5];

/////////////////////////////////////////////////////////////////////////////////	 
//外部中断初始化程序  初始化PF6-9io口为中断输入
void EXTIX_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	KEY_Init(); 									//按键对应的IO口初始化

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); //使能SYSCFG时钟

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource6); //PF6 连接到中断线6
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource7); //PF7 连接到中断线7
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource8); //PF8 连接到中断线8
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource9); //PF9 连接到中断线9

	// 配置外部中断线 EXTI_Line6,7,8,9  
	EXTI_InitStructure.EXTI_Line = EXTI_Line6 | EXTI_Line7 | EXTI_Line8 | EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;		//中断线使能
	EXTI_Init(&EXTI_InitStructure); 				//配置

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; //外部中断5-9  刚好 PF6-9 在 EXTI9_5_IRQn 里面
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; //子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 				//配置   
}


//外部中断线5-9服务程序
void EXTI9_5_IRQHandler(void)
{

	for (int i = 5; i <= 9; i++)
		{
		int 			bit = 0x1 << i;
		ITStatus		status;

		status				= EXTI_GetITStatus(bit);

		if (status == SET)
			{
			extTriggered[i - 5] += 1;
			EXTI_ClearITPendingBit(bit);
			}
		}

}


