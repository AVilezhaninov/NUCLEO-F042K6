#ifndef RCC_H_
#define RCC_H_

/* CMSIS */
#include "stm32f0xx.h"
#include "system_stm32f0xx.h"

/******************************************************************************/
/* Exported functions prototypes **********************************************/
/******************************************************************************/
void RCC_InitSystemClock();

uint32_t RCC_GetHCLKFrequency();
uint32_t RCC_GetPCLK1Frequency();

void RCC_StupidDelay(volatile uint32_t delay);

#endif /* RCC_H_ */
