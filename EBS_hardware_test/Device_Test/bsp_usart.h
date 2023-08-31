#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "stm32f10x.h"

#define USART3_MAX_RECV_LEN		1024
#define USART3_MAX_SEND_LEN		1024
#define USART3_RX_EN 			1

extern uint8_t  USART3_RX_BUF[USART3_MAX_RECV_LEN];
extern uint8_t  USART3_TX_BUF[USART3_MAX_SEND_LEN];
extern int16_t USART3_RX_STA;  

void USART_GPIO_Config(void);
void USART3_Config(void);
void u3_printf(char* fmt,...);
#endif
