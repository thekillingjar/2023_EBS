/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    can.c
  * @brief   This file provides code for the configuration
  *          of the CAN instances.
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
/* Includes ------------------------------------------------------------------*/
#include "can.h"

/* USER CODE BEGIN 0 */
#include "main.h"
#include "ebs.h"
#include "usart.h"
#include "stdio.h"
#include "config.h"
CAN_s_Rx_Temp can_s_rx_temp;
CAN_s_Tx_Temp can_s_tx_temp;
/* USER CODE END 0 */

CAN_HandleTypeDef hcan;

/* CAN init function */
void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */

  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 4;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_9TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_8TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = ENABLE;
  hcan.Init.AutoWakeUp = ENABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */

  /* USER CODE END CAN_Init 2 */

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN GPIO Configuration
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 1, 2);
    HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN GPIO Configuration
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void CAN_Filter_Init(void) {
	CAN_FilterTypeDef filter1;
	filter1.FilterBank = 0;
	filter1.FilterMode = CAN_FILTERMODE_IDLIST;
	filter1.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	filter1.FilterActivation = CAN_FILTER_ENABLE;
	filter1.FilterScale = CAN_FILTERSCALE_32BIT;
	filter1.FilterIdHigh = 0x20; // 0x001
	filter1.FilterIdLow = 0x0000;
	filter1.FilterMaskIdHigh = 0x2AC0;
	filter1.FilterMaskIdLow = 0x0000;

	if (HAL_CAN_ConfigFilter(&hcan, &filter1) != HAL_OK)
		Error_Handler();
}

void CAN_Send_Msg_Init(uint32_t a_StdId, uint32_t a_ExtId, uint32_t a_IDE, uint32_t a_RTR, uint8_t len) {
	can_s_tx_temp.can_tx_temp.StdId = a_StdId;
	can_s_tx_temp.can_tx_temp.ExtId = a_ExtId;
	can_s_tx_temp.can_tx_temp.IDE   = a_IDE;
	can_s_tx_temp.can_tx_temp.RTR   = a_RTR;
	can_s_tx_temp.can_tx_temp.DLC   = len;
	can_s_tx_temp.can_tx_temp.TransmitGlobalTime = DISABLE;
}
void CAN_Send_Msg(uint32_t a_StdId, uint32_t a_ExtId, uint8_t len) {
	uint8_t brake1_control = 0, brake2_control = 0, sdc_as;
	uint8_t FreeTxNum = 0;
	CAN_Send_Msg_Init(a_StdId, a_ExtId, CAN_ID_STD, CAN_RTR_DATA, len);
	switch(a_StdId) {
	case 0x002:

		brake1_control = HAL_GPIO_ReadPin(brake_Control1_PORT, brake_Control1_PIN);
		brake2_control = HAL_GPIO_ReadPin(brake_Control2_PORT, brake_Control2_PIN);
		sdc_as = HAL_GPIO_ReadPin(SDC_AS_PORT, SDC_AS_PIN);
#ifdef EBS_TEST
		ebs_ready = 1;
#endif
		can_s_tx_temp.can_tx_arry[0] = EBS_error | (brake1_control << 1) | (brake2_control << 2) | (sdc_as << 3) | (ebs_ready << 4) ;
		can_s_tx_temp.can_tx_arry[1] = pre_AS_state | (error_state_transition << 4);
		can_s_tx_temp.can_tx_arry[2] = bp_lose[1] | (bp_lose[2] << 1) | (bp_lose[3] << 2) | (bp_lose[4] << 3)
				| (bp_lose[5] << 4) | (bp_lose[6] << 5);
		can_s_tx_temp.can_tx_arry[3] = air1_insufficient | (air2_insufficient << 1) | (air1_overpressure << 2) | (air2_overpressure << 3)
				| (air1_path_failure << 4) | (air2_path_failure << 5) | (circuit_error << 6) | (timeout << 7);

		break;
	case 0x402: // 油压传感器
		can_s_tx_temp.can_tx_arry[0] = bp[3] & 0xFF;
		can_s_tx_temp.can_tx_arry[1] = (bp[3] & 0xFF00) >> 8;
		can_s_tx_temp.can_tx_arry[2] = bp[4] & 0xFF;
		can_s_tx_temp.can_tx_arry[3] = (bp[4] & 0xFF00) >> 8;
		can_s_tx_temp.can_tx_arry[4] = bp[5] & 0xFF;
		can_s_tx_temp.can_tx_arry[5] = (bp[5] & 0xFF00) >> 8;
		can_s_tx_temp.can_tx_arry[6] = bp[6] & 0xFF;
		can_s_tx_temp.can_tx_arry[7] = (bp[6] & 0xFF00) >> 8;

		break;
	case 0x482: // 气压传感器
		can_s_tx_temp.can_tx_arry[0] = bp[1] & 0xFF;
		can_s_tx_temp.can_tx_arry[1] = (bp[1] & 0xFF00) >> 8;
		can_s_tx_temp.can_tx_arry[2] = bp[2] & 0xFF;
		can_s_tx_temp.can_tx_arry[3] = (bp[2] & 0xFF00) >> 8;
	}
	//u3_printf("123132\r\n");
	do {
		FreeTxNum=HAL_CAN_GetTxMailboxesFreeLevel(&hcan);
	} while(0 == FreeTxNum);
	if (HAL_CAN_AddTxMessage(&hcan, &can_s_tx_temp.can_tx_temp, can_s_tx_temp.can_tx_arry, &can_s_tx_temp.mailbox) != HAL_OK)
		Error_Handler();
}
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcanx) {
	if (hcanx->Instance == hcan.Instance) {
		if (HAL_CAN_GetRxMessage(hcanx, CAN_RX_FIFO0, &can_s_rx_temp.can_rx_temp, can_s_rx_temp.can_rx_arry) != HAL_OK)
			Error_Handler();
		if (can_s_rx_temp.can_rx_temp.StdId == 0x156) {
			ASMS = (can_s_rx_temp.can_rx_arry[0] >> 3) & 1;
			if (pre_AS_state != AS_Emergency && new_state == 0xFF) {
				new_state = can_s_rx_temp.can_rx_arry[0] & 7; // get can_rx_arry[0][2:0]
				//u3_printf("AS_state:%d\r\n", new_state);
			}
		}
		//HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
	}
}
/* USER CODE END 1 */
