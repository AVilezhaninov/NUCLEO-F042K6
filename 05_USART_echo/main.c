/* CMSIS */
#include "stm32f0xx.h"

/* User */
#include "RCC.h"

/******************************************************************************/
/* Private definitions ********************************************************/
/******************************************************************************/
#define USART1_BAUDRATE       115200u
#define USART1_IRQ_PRIORITY   5u

#define USART2_BAUDRATE       115200u
#define USART2_IRQ_PRIORITY   5u

/******************************************************************************/
/* Private function prototypes ************************************************/
/******************************************************************************/
static void InitUsart1(const uint32_t baudrate, const uint32_t irq_priority);
static void InitUsart2(const uint32_t baudrate, const uint32_t irq_priority);
static void SetUsartBaudRate(USART_TypeDef *const usart,
                             const uint32_t baudrate);

/******************************************************************************/
/* Interrupts *****************************************************************/
/******************************************************************************/
/**
 * USART1 interrupt handler
 * Receive data from USART1 and send it back.
 */
void USART1_IRQHandler() {
  uint8_t received_data;

  if ((USART1->ISR & USART_ISR_RXNE) == USART_ISR_RXNE) {
    received_data = USART1->RDR; /* Receive data */
    USART1->TDR = received_data; /* Send it back */
  }
}

/**
 * USART2 interrupt handler
 * Receive data from USART2 and send it back.
 */
void USART2_IRQHandler() {
  uint8_t received_data;

  if ((USART2->ISR & USART_ISR_RXNE) == USART_ISR_RXNE) {
    received_data = USART2->RDR; /* Receive data */
    USART2->TDR = received_data; /* Send it back */
  }
}

/******************************************************************************/
/* Main ***********************************************************************/
/******************************************************************************/
int main(void) {
  InitSystemClock();
  InitUsart1(USART1_BAUDRATE, USART1_IRQ_PRIORITY);
  InitUsart2(USART2_BAUDRATE, USART2_IRQ_PRIORITY);

  while (1) {
    continue;
  }
}

/******************************************************************************/
/* Private functions **********************************************************/
/******************************************************************************/
/**
 * Init USART1
 * PA9->Tx, PA10->Rx
 */
static void InitUsart1(const uint32_t baudrate, const uint32_t irq_priority) {
  /* Init USART1 pins */
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;    /* Enable PORTA clock */
  GPIOA->AFR[1] |= (1u << 4u);          /* PA9 in USART1_Tx AF1 */
  GPIOA->AFR[1] |= (1u << 8u);          /* PA10 in USART1_Rx AF1 */
  GPIOA->MODER |= GPIO_MODER_MODER9_1;  /* PA9 in USART1_Tx AF */
  GPIOA->MODER |= GPIO_MODER_MODER10_1; /* PA10 in USART1_Rx AF */

  /* Init USART1 */
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN; /* Enable USART1 clock */
  USART1->CR1 |= USART_CR1_RE;          /* Receive enable */
  USART1->CR1 |= USART_CR1_TE;          /* Transmit enable */
  USART1->CR1 |= USART_CR1_RXNEIE;      /* Receive interrupt enable */
  SetUsartBaudRate(USART1, baudrate);   /* Set baud rate */

  NVIC_SetPriority(USART1_IRQn, irq_priority); /* Set USART1 IRQ priority */
  NVIC_EnableIRQ(USART1_IRQn);                 /* Enable USART1 interrupt */

  USART1->CR1 |= USART_CR1_UE;          /* Enable USART1 */
}

/**
 * Init USART2
 * PA2->Tx, PA3->Rx
 */
static void InitUsart2(const uint32_t baudrate, const uint32_t irq_priority) {
  /* Init USART2 pins */
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;    /* Enable PORTA clock */
  GPIOA->AFR[0] |= (1u << 8u);          /* PA2 in USART2_Tx AF1 */
  GPIOA->AFR[0] |= (1u << 12u);         /* PA3 in USART2_Rx AF1 */
  GPIOA->MODER |= GPIO_MODER_MODER2_1;  /* PA2 in USART2_Tx AF */
  GPIOA->MODER |= GPIO_MODER_MODER3_1;  /* PA3 in USART2_Rx AF */

  /* Init USART2 */
  RCC->APB1ENR |= RCC_APB1ENR_USART2EN; /* Enable USART2 clock */
  USART2->CR1 |= USART_CR1_RE;          /* Receive enable */
  USART2->CR1 |= USART_CR1_TE;          /* Transmit enable */
  USART2->CR1 |= USART_CR1_RXNEIE;      /* Receive interrupt enable */
  SetUsartBaudRate(USART2, baudrate);   /* Set baud rate */

  NVIC_SetPriority(USART2_IRQn, irq_priority); /* Set USART2 IRQ priority */
  NVIC_EnableIRQ(USART2_IRQn);                 /* Enable USART2 interrupt */

  USART2->CR1 |= USART_CR1_UE;          /* Enable USART2 */
}

/**
 * Set USART baud rate
 * @param usart    USART instance
 * @param baudrate Required baud rate
 */
static void SetUsartBaudRate(USART_TypeDef *const usart,
                             const uint32_t baudrate) {
  usart->BRR = GetPCLK1Frequency() / baudrate;
}

