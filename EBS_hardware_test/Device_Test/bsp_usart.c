#include "stm32f10x.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "stdarg.h"
#include "bsp_usart.h"

uint8_t  USART3_RX_BUF[USART3_MAX_RECV_LEN];
uint8_t  USART3_TX_BUF[USART3_MAX_SEND_LEN];
int16_t USART3_RX_STA;  

void USART_GPIO_Config(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	

}

void USART3_NVIC_Config(void) {
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
}

void USART3_Config(void) {
	USART_InitTypeDef USART3_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	USART3_InitStructure.USART_BaudRate = 115200;
	USART3_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART3_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART3_InitStructure.USART_Parity = USART_Parity_No;
	USART3_InitStructure.USART_StopBits = USART_StopBits_1;
	USART3_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &USART3_InitStructure);	
	
	USART3_NVIC_Config();
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART3, ENABLE);
}


void u3_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART3_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART3_TX_BUF);		
	for(j=0;j<i;j++)							
	{
	  while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
		USART_SendData(USART3,USART3_TX_BUF[j]); 
	} 
}

