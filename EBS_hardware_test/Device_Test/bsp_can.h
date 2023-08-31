#ifndef __BSP_CAN_H
#define __BSP_CAN_H

#include "stm32f10x.h"
void CAN_GPIO_Config(void);
u8 CAN1_Send_Msg(u8* msg, u8 len);
void CAN_Config(u8 tsjw, u8 tbs1, u8 tbs2, u16 brp);
#endif
