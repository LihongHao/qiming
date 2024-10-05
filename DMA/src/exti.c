
#include "exti.h"
#include "led.h" 
#include "key.h"
#include "beep.h"

extern uint32_t extTriggered[5];

/////////////////////////////////////////////////////////////////////////////////	 
//�ⲿ�жϳ�ʼ������  ��ʼ��PF6-9io��Ϊ�ж�����
void EXTIX_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	KEY_Init(); 									//������Ӧ��IO�ڳ�ʼ��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); //ʹ��SYSCFGʱ��

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource6); //PF6 ���ӵ��ж���6
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource7); //PF7 ���ӵ��ж���7
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource8); //PF8 ���ӵ��ж���8
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource9); //PF9 ���ӵ��ж���9

	// �����ⲿ�ж��� EXTI_Line6,7,8,9  
	EXTI_InitStructure.EXTI_Line = EXTI_Line6 | EXTI_Line7 | EXTI_Line8 | EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;		//�ж���ʹ��
	EXTI_Init(&EXTI_InitStructure); 				//����

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; //�ⲿ�ж�5-9  �պ� PF6-9 �� EXTI9_5_IRQn ����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; //�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); 				//����   
}


//�ⲿ�ж���5-9�������
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


