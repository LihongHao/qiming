
#include "dma.h"																			


//////////////////////////////////////////////////////////////////////////////////	 

/****************************************************************************
* ��	 ��: void USER_DMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr)
* ��	 �ܣ�DMAx�ĸ�ͨ������
* ��ڲ�����DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
			chx:DMAͨ��ѡ��,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
					  par:�����ַ
					  mar:�洢����ַ
					  ndtr:���ݴ�����						
* ���ز�������
* ˵	 ������ߴ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸� 
****************************************************************************/
void USER_DMA_Config(DMA_Stream_TypeDef * DMA_Streamx, u32 chx, u32 par, u32 mar, u16 ndtr)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	if ((u32) DMA_Streamx > (u32) DMA2) //�õ���ǰstream������DMA2����DMA1
		{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); //DMA2ʱ��ʹ�� 

		}
	else 
		{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); //DMA1ʱ��ʹ�� 
		}

	DMA_DeInit(DMA_Streamx);

	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE)
		{
		} //�ȴ�DMA������ 

	/* ���� DMA Stream */

	DMA_InitStructure.DMA_Channel = chx;			//ͨ��ѡ��
	DMA_InitStructure.DMA_PeripheralBaseAddr = par; //DMA�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = mar;	//DMA �洢��0��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral; //�洢��������ģʽ
	DMA_InitStructure.DMA_BufferSize = ndtr;		//���ݴ����� 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //���������ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //�洢������ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //�������ݳ���:8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //�洢�����ݳ���:8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;	// ʹ����ͨģʽ 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //�е����ȼ�
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; //�洢��ͻ�����δ���
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //����ͻ�����δ���
	DMA_Init(DMA_Streamx, &DMA_InitStructure);		//��ʼ��DMA Stream

	DMA_ITConfig(DMA1_Stream6,  DMA_IT_TC, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn; //����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure); 				//����ָ���Ĳ�����ʼ��VIC�Ĵ�����  

}


/****************************************************************************
* ��	 ��: void USER_DMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
* ��	 �ܣ�����һ��DMA����
* ��ڲ�����DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7 
			ndtr:���ݴ����� 
* ���ز�������
* ˵	 ���� 
****************************************************************************/
void USER_DMA_Enable(DMA_Stream_TypeDef * DMA_Streamx, u16 ndtr)
{

	DMA_Cmd(DMA_Streamx, DISABLE);					//�ر�DMA���� 

	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE)
		{
		} //ȷ��DMA���Ա�����  

	DMA_SetCurrDataCounter(DMA_Streamx, ndtr);		//���ݴ�����  

	DMA_Cmd(DMA_Streamx, ENABLE);					//����DMA���� 
}



void DMA1_Stream6_IRQHandler(void)
{
	// Check if the transfer complete flag is set
	if (DMA1->HISR & DMA_HISR_TCIF6) // Check for transfer complete flag in high ISR
		{
		// Clear the transfer complete flag
		DMA1->HIFCR 		|= DMA_HIFCR_CTCIF6;

		// Handle the transfer complete event (e.g., disable the DMA stream)
		DMA1_Stream6->CR	&= ~DMA_SxCR_EN;		// Disable the stream after completion

	
		}

}

























