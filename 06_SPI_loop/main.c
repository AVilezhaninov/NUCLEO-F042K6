/* CMSIS */
#include "stm32f0xx.h"

/* User */
#include "RCC.h"

/******************************************************************************/
/* Private definitions ********************************************************/
/******************************************************************************/
#define SPI1_IRQ_PRIORITY   5u

/******************************************************************************/
/* Private function prototypes ************************************************/
/******************************************************************************/
static void InitSpi1(void);
static void Spi1CsSet(void);
static void Spi1CsReset(void);

/******************************************************************************/
/* Interrupts *****************************************************************/
/******************************************************************************/
void SPI1_IRQHandler(void) {
  uint8_t spi_data = 0u;

  if ((SPI1->SR & SPI_SR_RXNE) == SPI_SR_RXNE) {
    spi_data = SPI1->DR;
    Spi1CsSet();
  }
}

/******************************************************************************/
/* Main ***********************************************************************/
/******************************************************************************/
int main(void) {
  uint8_t spi_data = 0u;    /* Data to send through SPI */

  RCC_InitSystemClock();
  InitSpi1();

  while (1) {
    Spi1CsReset();
    SPI1->DR = spi_data++;  /* Send data through SPI1 */
    RCC_StupidDelay(1000u);
  }
}

/******************************************************************************/
/* Private functions **********************************************************/
/******************************************************************************/
static void InitSpi1(void) {
  /* Init SPI's GPIO */
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;    /* Enable PORTA clock */
  GPIOA->MODER |= GPIO_MODER_MODER5_1;  /* PA5 in SPI1_SCK AF */
  GPIOA->MODER |= GPIO_MODER_MODER6_1;  /* PA6 in SPI1_MISO AF */
  GPIOA->MODER |= GPIO_MODER_MODER7_1;  /* PA7 in SPI1_MOSI AF */
  GPIOA->MODER |= GPIO_MODER_MODER4_0;  /* PA4 SPI CS push-pull mode */

  /* SPI1 init */
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; /* Enable SPI1 clock */
  SPI1->CR1 &= ~SPI_CR1_BR_0;         /* Fpclk/2 */
  SPI1->CR1 |= SPI_CR1_MSTR;          /* SPI in master mode */
  SPI1->CR1 |= SPI_CR1_SSM;           /* SSM = 1 */
  SPI1->CR1 |= SPI_CR1_SSI;           /* SSI = 1 */
  SPI1->CR2 |= SPI_CR2_RXNEIE;        /* Rx interrupt enable */

  NVIC_SetPriority(SPI1_IRQn, SPI1_IRQ_PRIORITY); /* Set IRQ priority */
  NVIC_EnableIRQ(SPI1_IRQn);                      /* Enable interrupt */

  SPI1->CR1 |= SPI_CR1_SPE;           /* Enable SPI */
}

static void Spi1CsSet(void) {
  GPIOA->BSRR |= GPIO_BSRR_BS_4;
}

static void Spi1CsReset(void) {
  GPIOA->BSRR |= GPIO_BSRR_BR_4;
}
