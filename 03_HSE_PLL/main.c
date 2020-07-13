/* CMSIS */
#include "stm32f0xx.h"

/* User */
#include "RCC.h"

/******************************************************************************/
/* Private definitions ********************************************************/
/******************************************************************************/
#define LED_TOGGLE_DELAY 1000000u

/******************************************************************************/
/* Private function prototypes ************************************************/
/******************************************************************************/
static void InitGpio(void);
static void ToggleLed(void);
static void DummyDelay(int delay);

/******************************************************************************/
/* Main ***********************************************************************/
/******************************************************************************/
int main(void) {
  RCC_InitSystemClock();
  InitGpio();

  while (1) {
    ToggleLed();
    DummyDelay(LED_TOGGLE_DELAY);
  }
}

/******************************************************************************/
/* Private functions **********************************************************/
/******************************************************************************/
static void InitGpio(void) {
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;   /* Enable GPIOB clock */
  GPIOB->MODER |= GPIO_MODER_MODER3_0; /* GPIOB pin 3 in push-pull mode */
}

static void ToggleLed(void) {
  GPIOB->ODR ^= GPIO_ODR_3;            /* Toggle GPIOB pin 3 */
}

static void DummyDelay(int delay) {
  while (delay-- > 0) {
    continue;
  }
}
