#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include "stm32f10x.h"

#define ADC1_DR_Address 0x40012400

extern volatile uint16_t ADC_ConvertedValue[5];

void DMA1_init(void);
void ADC_GPIO_Config(void);
void ADC1_init(void);

#endif
