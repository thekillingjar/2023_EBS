#include "bsp_can.h"
#include "bsp_usart.h"
void CAN_GPIO_Config(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}

void CAN_NVIC_Config(void) {
	NVIC_InitTypeDef NVIC_InitStructure;
	
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn; // USB Device Low Priority or CAN1 RX0 Interrupts 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
}
void CAN_Filter_Config() {
	CAN_FilterInitTypeDef CAN_FilterInstructure;
	
	CAN_FilterInstructure.CAN_FilterNumber = 0;
	CAN_FilterInstructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
	CAN_FilterInstructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInstructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInstructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInstructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInstructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInstructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInstructure.CAN_FilterScale = CAN_FilterScale_32bit;
	
	CAN_FilterInit(&CAN_FilterInstructure);
}
void CAN_Config(u8 tsjw, u8 tbs1, u8 tbs2, u16 brp) {
	CAN_InitTypeDef CAN_InitStructure;
	
	CAN_GPIO_Config();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	
	CAN_InitStructure.CAN_ABOM = DISABLE; // 自动离线(Bus-Off)管理
	CAN_InitStructure.CAN_AWUM = DISABLE; // 自动唤醒模式
	CAN_InitStructure.CAN_TTCM = DISABLE; // 时间触发通信模式
	CAN_InitStructure.CAN_NART = DISABLE; // 禁止报文自动重传
	CAN_InitStructure.CAN_RFLM = DISABLE; // 接收FIFO锁定模式
	CAN_InitStructure.CAN_TXFP = DISABLE; // 发送FIFO优先级
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = tsjw;
	CAN_InitStructure.CAN_BS1 = tbs1;
	CAN_InitStructure.CAN_BS2 = tbs2;
	CAN_InitStructure.CAN_Prescaler = brp;
	CAN_Init(CAN1, &CAN_InitStructure);
	
	CAN_Filter_Config();
	CAN_NVIC_Config();
}


u8 CAN1_Send_Msg(u8* msg, u8 len) {
	u8 mbox;
	u16 i = 0;
	CanTxMsg TxMessage;
	TxMessage.StdId = 0x156;
	TxMessage.ExtId = 0x00;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.DLC = len;
	for (i = 0; i < len; ++i) 
		TxMessage.Data[i] = *(msg + i);
	mbox = CAN_Transmit(CAN1, &TxMessage);
	i = 0;
	while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;
	if (i >= 0xFFF) return 0; // 发送失败
	return 1;
}


void USB_LP_CAN1_RX0_IRQHandler(void)
{
	int i=0;
	CanRxMsg RxMessage;
	RxMessage.StdId=0x00;	 
	RxMessage.ExtId=0x00;	 
	RxMessage.IDE=0;		  
	RxMessage.RTR=0;		
	RxMessage.FMI=0;
	for(i=0;i<8;i++)
		RxMessage.Data[i]=0;
	u3_printf("receive\r\n");
	CAN_Receive(CAN1, 0, &RxMessage);
		
}