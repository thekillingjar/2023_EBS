#include "bsp_basic.h"

void RCC_Config(void) {
	ErrorStatus HSEStatus;
	RCC_DeInit(); // 初始化为缺省值
	RCC_HSEConfig(RCC_HSE_ON); // 使能外部的高速时钟
	//while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET); // 等待外部高速时钟使能就绪
	HSEStatus=  RCC_WaitForHSEStartUp();
	if ( HSEStatus ==SUCCESS ) {
		//使能预取指 
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		//设置成两个等待周期
		FLASH_SetLatency(FLASH_Latency_2);
		
		//使HCLK=SYSCLK=72M
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		
		RCC_PCLK1Config(RCC_HCLK_Div2);
		
		RCC_PCLK2Config(RCC_HCLK_Div1);
		
		//PLLCLK为72MHz
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
		RCC_PLLCmd(ENABLE);
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		
		while (RCC_GetSYSCLKSource() != 0x08); // 等待配置
	}
	//可以增加超时判断，如果超时更换时钟源
}