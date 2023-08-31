/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    can.h
  * @brief   This file contains all the function prototypes for
  *          the can.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CAN_H__
#define __CAN_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern CAN_HandleTypeDef hcan;

/* USER CODE BEGIN Private defines */
typedef struct {

	CAN_RxHeaderTypeDef can_rx_temp;
	uint8_t can_rx_arry[8];

}CAN_s_Rx_Temp;

typedef struct {
	CAN_TxHeaderTypeDef can_tx_temp;
	uint8_t can_tx_arry[8];
	uint32_t mailbox;
}CAN_s_Tx_Temp;

extern CAN_s_Rx_Temp can_s_rx_temp;
extern CAN_s_Tx_Temp can_s_tx_temp;
/* USER CODE END Private defines */

void MX_CAN_Init(void);

/* USER CODE BEGIN Prototypes */
void CAN_Send_Msg_Init(uint32_t a_StdId, uint32_t a_ExtId, uint32_t a_IDE, uint32_t a_RTR, uint8_t len);
void CAN_Send_Msg(uint32_t a_StdId, uint32_t a_ExtId, uint8_t len);
void CAN_Filter_Init(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __CAN_H__ */

