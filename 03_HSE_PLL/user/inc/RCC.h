#ifndef RCC_H_
#define RCC_H_

/* CMSIS */
#include "stm32f0xx.h"
#include "system_stm32f0xx.h"

/******************************************************************************/
/* Exported functions prototypes **********************************************/
/******************************************************************************/
void InitSystemClock();

uint32_t GetHCLKFrequency();
uint32_t GetPCLK1Frequency();

void StupidDelay(volatile uint32_t delay);

#endif /* RCC_H_ */
