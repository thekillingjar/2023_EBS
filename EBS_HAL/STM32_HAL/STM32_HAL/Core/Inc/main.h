/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define ADC1_CHANNEL_CNT 6 // 采样通道�???????????
#define ADC1_CHANNEL_FRE 3 // 单个通道采样次数，用来取平均�???????????
extern uint32_t OIL_Max;
extern uint32_t OIL_Min;
extern uint32_t AIR_Max;
extern uint32_t AIR_Min;
extern int32_t air_e_max1;
extern int32_t air_e_max2;
extern uint32_t AIR_BRAKE_MIN;
extern uint32_t OIL_BRAKE_MIN;
extern uint32_t OIL_Release_Max;
extern uint16_t adc1_val_buf[ADC1_CHANNEL_CNT * ADC1_CHANNEL_FRE]; // 传�?�给DMA存放多�?�道采样值的数组
extern uint32_t adc1_ave_val[ADC1_CHANNEL_CNT];              // 计算多�?�道的平均采样�?�的过程数组
extern uint16_t value[ADC1_CHANNEL_CNT];                     // 多�?�道的平均采样�?�的数组
extern int air_temp[5];
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

#define AIR2    0
#define LF_OIL  1
#define LB_OIL  2
#define RF_OIL  3
#define RB_OIL  4
#define AIR1    5

#define CAN_0X002_SEND_CODE 1
#define CAN_0X402_SEND_CODE 2
#define CAN_0X482_SEND_CODE 4

#define TXS_Control_ON  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
#define TXS_Control_OFF HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);

#define brake1_ON  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
#define brake1_OFF HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

#define brake2_ON  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
#define brake2_OFF HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);

#define EBS_error_ON  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
#define EBS_error_OFF HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);

#define AS_mode_ON  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);
#define AS_mode_OFF HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);

#define watchdog_ON     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);
#define watchdog_OFF    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);
#define watchdog_TOGGLE HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_15);

#define brake_Control1_PIN  GPIO_PIN_9
#define brake_Control1_PORT GPIOB

#define brake_Control2_PIN  GPIO_PIN_8
#define brake_Control2_PORT GPIOB

#define SDC_AS_PIN  GPIO_PIN_6
#define SDC_AS_PORT GPIOB


/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
