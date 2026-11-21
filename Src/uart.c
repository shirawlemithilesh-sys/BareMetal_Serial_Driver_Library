
#include "uart.h"

/* =========================================================
   RCC REGISTERS
   ========================================================= */

#define RCC_AHB2ENR       (*(volatile uint32_t *)0x4002104C)
#define RCC_APB1ENR1      (*(volatile uint32_t *)0x40021058)

/* =========================================================
   GPIOA REGISTERS
   ========================================================= */

#define GPIOA_MODER       (*(volatile uint32_t *)0x48000000)
#define GPIOA_OTYPER      (*(volatile uint32_t *)0x48000004)
#define GPIOA_PUPDR       (*(volatile uint32_t *)0x4800000C)
#define GPIOA_AFRL        (*(volatile uint32_t *)0x48000020)

/* =========================================================
   USART2 REGISTERS
   ========================================================= */

#define USART2_CR1        (*(volatile uint32_t *)0x40004400)
#define USART2_CR2        (*(volatile uint32_t *)0x40004404)
#define USART2_CR3        (*(volatile uint32_t *)0x40004408)
#define USART2_BRR        (*(volatile uint32_t *)0x4000440C)
#define USART2_ISR        (*(volatile uint32_t *)0x4000441C)
#define USART2_ICR        (*(volatile uint32_t *)0x40004420)
#define USART2_RDR        (*(volatile uint32_t *)0x40004424)
#define USART2_TDR        (*(volatile uint32_t *)0x40004428)

/* =========================================================
   NVIC REGISTERS
   ========================================================= */

#define NVIC_ISER1        (*(volatile uint32_t *)0xE000E104)
#define NVIC_ICPR1        (*(volatile uint32_t *)0xE000E284)

/* =========================================================
   CLOCK CONFIGURATION
   ========================================================= */

#define GPIOA_CLOCK_ENABLE    (1U << 0)
#define USART2_CLOCK_ENABLE   (1U << 17)

#define PCLK1_FREQUENCY       4000000U
#define USART2_BAUDRATE       115200U

/* =========================================================
   USART CONTROL BITS
   ========================================================= */

#define USART_CR1_UE          (1U << 0)
#define USART_CR1_RE          (1U << 2)
#define USART_CR1_TE          (1U << 3)
#define USART_CR1_RXNEIE      (1U << 5)

#define USART_ISR_RXNE        (1U << 5)
#define USART_ISR_TC          (1U << 6)
#define USART_ISR_TXE         (1U << 7)

#define USART_ISR_ERROR_MASK  (0x0FU)
#define USART_ICR_ERROR_CLEAR (0x0FU)

/* =========================================================
   GPIO CONFIGURATION
   ========================================================= */

#define USART2_TX_PIN         (1U << 2)
#define USART2_RX_PIN         (1U << 3)

/* =========================================================
   NVIC CONFIGURATION
   USART2 IRQ number = 38
   ISER1 bit = 38 - 32 = 6
   ========================================================= */

#define USART2_IRQ_NUMBER     38U
#define USART2_NVIC_BIT       (1U << (USART2_IRQ_NUMBER - 32U))

/* =========================================================
   CIRCULAR RECEIVE BUFFER
   ========================================================= */

static volatile uint8_t rx_buffer[UART_RX_BUFFER_SIZE];

static volatile uint32_t rx_head = 0U;
static volatile uint32_t rx_tail = 0U;

static volatile uint32_t rx_overflow_count = 0U;

/* =========================================================
   USART2 INITIALIZATION
   ========================================================= */

void USART2_Init(void)
{
    /* Enable GPIOA clock */

    RCC_AHB2ENR |= GPIOA_CLOCK_ENABLE;

    /* Enable USART2 clock */

    RCC_APB1ENR1 |= USART2_CLOCK_ENABLE;

    /* Read back clock registers */

    (void)RCC_AHB2ENR;
    (void)RCC_APB1ENR1;

    /* Configure PA2 and PA3 as alternate function */

    GPIOA_MODER &= ~((3U << 4) | (3U << 6));

    GPIOA_MODER |= ((2U << 4) | (2U << 6));

    /* Configure AF7 for USART2 */

    GPIOA_AFRL &= ~((0xFU << 8) | (0xFU << 12));

    GPIOA_AFRL |= ((7U << 8) | (7U << 12));

    /* Configure push-pull */

    GPIOA_OTYPER &= ~(USART2_TX_PIN | USART2_RX_PIN);

    /* No pull-up or pull-down */

    GPIOA_PUPDR &= ~((3U << 4) | (3U << 6));

    /* Disable USART before configuration */

    USART2_CR1 = 0U;
    USART2_CR2 = 0U;
    USART2_CR3 = 0U;

    /* Configure baud rate */

    USART2_BRR =
        (PCLK1_FREQUENCY + (USART2_BAUDRATE / 2U))
        / USART2_BAUDRATE;

    /* Clear pending USART errors */

    USART2_ICR = USART_ICR_ERROR_CLEAR;

    /* Reset software buffer */

    rx_head = 0U;
    rx_tail = 0U;
    rx_overflow_count = 0U;

    /* Clear pending USART2 NVIC interrupt */

    NVIC_ICPR1 = USART2_NVIC_BIT;

    /* Enable USART2 interrupt in NVIC */

    NVIC_ISER1 = USART2_NVIC_BIT;

    /* Enable transmitter, receiver and RX interrupt */

    USART2_CR1 = USART_CR1_TE |
                 USART_CR1_RE |
                 USART_CR1_RXNEIE |
                 USART_CR1_UE;
}

/* =========================================================
   USART2 INTERRUPT SERVICE ROUTINE
   ========================================================= */

void USART2_IRQHandler(void)
{
    uint32_t status;
    uint8_t received_data;

    /* Read USART status register */

    status = USART2_ISR;

    /* Check if receive data is available */

    if (status & USART_ISR_RXNE)
    {
        /* Read received character from RDR */

        received_data = (uint8_t)USART2_RDR;

        /* Calculate next buffer position */

        uint32_t next_head =
            (rx_head + 1U) % UART_RX_BUFFER_SIZE;

        /* Check whether buffer is full */

        if (next_head != rx_tail)
        {
            /* Store received character */

            rx_buffer[rx_head] = received_data;

            /* Update head position */

            rx_head = next_head;
        }
        else
        {
            /* Buffer full: count dropped character */

            rx_overflow_count++;
        }
    }

    /* Clear USART error flags if present */

    if (status & USART_ISR_ERROR_MASK)
    {
        USART2_ICR = USART_ICR_ERROR_CLEAR;
    }
}

/* =========================================================
   NON-BLOCKING UART READ
   ========================================================= */

bool USART2_ReadChar(uint8_t *data)
{
    /* Validate pointer */

    if (data == 0)
    {
        return false;
    }

    /* Check if buffer is empty */

    if (rx_head == rx_tail)
    {
        return false;
    }

    /* Retrieve character */

    *data = rx_buffer[rx_tail];

    /* Move tail position */

    rx_tail = (rx_tail + 1U) % UART_RX_BUFFER_SIZE;

    return true;
}

/* =========================================================
   UART TRANSMISSION
   ========================================================= */

void USART2_WriteChar(uint8_t data)
{
    /* Wait until transmit register is empty */

    while (!(USART2_ISR & USART_ISR_TXE))
    {
    }

    /* Write character */

    USART2_TDR = data;
}

/* =========================================================
   UART STRING TRANSMISSION
   ========================================================= */

void USART2_WriteString(const char *string)
{
    while (*string != '\0')
    {
        USART2_WriteChar((uint8_t)*string);

        string++;
    }

    /* Wait until transmission completes */

    while (!(USART2_ISR & USART_ISR_TC))
    {
    }
}

/* =========================================================
   GET RX BUFFER OVERFLOW COUNT
   ========================================================= */

uint32_t USART2_GetRxOverflowCount(void)
{
    return rx_overflow_count;
}
