/* CMSIS */
#include "stm32f0xx.h"

/* FreeRTOS */
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

/* User */
#include "RCC.h"

/******************************************************************************/
/* Private definitions ********************************************************/
/******************************************************************************/
#define LED_TOGGLE_DELAY 250u

/******************************************************************************/
/* Private function prototypes ************************************************/
/******************************************************************************/
static void InitAll();
static void InitHardware();
static void InitGpio();
static void InitRTOSObjects();
static void ToggleLed();

/******************************************************************************/
/* Tasks **********************************************************************/
/******************************************************************************/
void LedTask(void *pvParameters) {
  (void)pvParameters;

  while (1) {
    ToggleLed();
    vTaskDelay(LED_TOGGLE_DELAY);
  }
}

/******************************************************************************/
/* Main ***********************************************************************/
/******************************************************************************/
int main() {
  InitAll();
  vTaskStartScheduler();

  while (1) {
    continue;
  }
}

/******************************************************************************/
/* Private functions **********************************************************/
/******************************************************************************/
static void InitAll() {
  InitHardware();
  InitRTOSObjects();
}

static void InitHardware() {
  RCC_InitSystemClock();
  InitGpio();
}

static void InitGpio(void) {
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;   /* Enable GPIOB clock */
  GPIOB->MODER |= GPIO_MODER_MODER3_0; /* GPIOB pin 3 in push-pull mode */
}

static void InitRTOSObjects(void) {
  /* Create tasks */
  xTaskCreate(LedTask, "", configMINIMAL_STACK_SIZE, NULL, 1u, NULL);
}

static void ToggleLed(void) {
  GPIOB->ODR ^= GPIO_ODR_3;            /* Toggle GPIOB pin 3 */
}
