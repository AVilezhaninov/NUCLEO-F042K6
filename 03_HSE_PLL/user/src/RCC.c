#include "RCC.h"

/******************************************************************************/
/* Private definitions ********************************************************/
/******************************************************************************/
#define HSI_STARTUP_TIMEOUT   0x5000u

/******************************************************************************/
/* Exported functions *********************************************************/
/******************************************************************************/
void InitSystemClock() {
  uint32_t StartUpCounter = 0u;
  uint32_t HSEStatus = 0u;

  /* Enable HSI */
  RCC->CR |= RCC_CR_HSEON;

  /* Wait till HSI is ready and exit if timeout is reached */
  while ((HSEStatus == 0u) && (StartUpCounter < HSI_STARTUP_TIMEOUT)) {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    ++StartUpCounter;
  }

  if (HSEStatus == RCC_CR_HSERDY) {
    /* Flash 1 wait state */
    FLASH->ACR |= FLASH_ACR_LATENCY;
    /* Set PLL multiplication factor */
    RCC->CFGR |= RCC_CFGR_PLLMUL6;
    /* Select PLL input clock source */
    RCC->CFGR |= RCC_CFGR_PLLSRC_HSE_PREDIV;
    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;
    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY) {
      continue;
    }

    /* Select PLL as system clock source */
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {
      continue;
    }

    SystemCoreClockUpdate();
  } else {
    /* If HSE fails to start-up, the application will have wrong clock
    configuration. User can add here some code to deal with this error */
    while (1) {
      continue;
    }
  }
}

uint32_t GetHCLKFrequency() {
  return SystemCoreClock;
}

uint32_t GetPCLK1Frequency() {
  return (GetHCLKFrequency() >>
          APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE) >> RCC_CFGR_PPRE_Pos]);
}

/**
 * Blocking delay
 * @param delay
 */
void StupidDelay(volatile uint32_t delay) {
  while (delay-- > 0u) {
    continue;
  }
}
