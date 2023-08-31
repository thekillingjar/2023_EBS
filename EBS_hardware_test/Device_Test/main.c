#include "stm32f10x.h"
#include "bsp_adc.h"
#include "bsp_basic.h"
#include "bsp_usart.h"
#include "delay.h"
#include "bsp_can.h"
int16_t i;
#define ADC_TEST
u8 msg1[10];
void GPIO_Config1() {
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
	GPIO_ResetBits(GPIOB, GPIO_Pin_6);
	GPIO_ResetBits(GPIOB, GPIO_Pin_7);
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
	                  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
	GPIO_ResetBits(GPIOC, GPIO_Pin_14);
	GPIO_ResetBits(GPIOC, GPIO_Pin_15);
}
int main() {
	
	RCC_Config();
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	ADC_GPIO_Config();
	DMA1_init();
	ADC1_init();
	USART_GPIO_Config();
	USART3_Config();
	CAN_Config(CAN_SJW_1tq, CAN_BS1_9tq, CAN_BS2_8tq, 4);
	
#ifdef CAN_TEST
	u3_printf("CAN_TEST\r\n");
	msg1[0] = 0x0a;
	while (1) {
		u3_printf("--------------\r\n");
		u3_printf("send can\r\n");
		u3_printf("%d\r\n", CAN1_Send_Msg(msg1, 1));
		u3_printf("--------------\r\n");
		delay_ms(1000);
		
	}
#endif
#ifdef ADC_TEST
	u3_printf("ADC_TEST\r\n");
	while (1) {
		u3_printf("-------------------\r\n");
		for (i = 1; i <= 4; ++i) {
			u3_printf("PA%d: %d\r\n", i, ADC_ConvertedValue[i]);
		}
		u3_printf("PA%d: %d\r\n", 5, ADC_ConvertedValue[i]);
		u3_printf("-------------------\r\n");
		delay_ms(1000);
	}
#endif
#ifdef GPIO_TEST
	GPIO_Config1();
	while(1){}
#endif
}

