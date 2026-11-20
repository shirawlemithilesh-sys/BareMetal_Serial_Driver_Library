/*
 * uart.c
 *
 *  Created on: Nov 20, 2026
 *      Author: HP
 */




#include "uart.h"

/* RCC registers */
#define RCC_AHB2ENR    (*(volatile uint32_t *)0x4002104C)
#define RCC_APB1ENR1   (*(volatile uint32_t *)0x40021058)

/* GPIOA registers */
#define GPIOA_MODER    (*(volatile uint32_t *)0x48000000)
#define GPIOA_OTYPER   (*(volatile uint32_t *)0x48000004)
#define GPIOA_PUPDR    (*(volatile uint32_t *)0x4800000C)
#define GPIOA_AFRL     (*(volatile uint32_t *)0x48000020)

/* USART2 registers */
#define USART2_CR1     (*(volatile uint32_t *)0x40004400)
#define USART2_CR2     (*(volatile uint32_t *)0x40004404)
#define USART2_CR3     (*(volatile uint32_t *)0x40004408)
#define USART2_BRR     (*(volatile uint32_t *)0x4000440C)
#define USART2_ISR     (*(volatile uint32_t *)0x4000441C)
#define USART2_RDR     (*(volatile uint32_t *)0x40004424)
#define USART2_TDR     (*(volatile uint32_t *)0x40004428)

/* Clock configuration */
#define GPIOA_CLOCK_ENABLE       (1U << 0)
#define USART2_CLOCK_ENABLE      (1U << 17)

/*
 * Clock assumptions:
 * SYSCLK = MSI = 4 MHz after reset
 * AHB prescaler = 1
 * APB1 prescaler = 1
 * USART2 clock source = PCLK1
 */
#define PCLK1_FREQUENCY          4000000U
#define USART2_BAUDRATE          115200U

/* USART2 control bits */
#define USART_CR1_UE             (1U << 0)
#define USART_CR1_RE             (1U << 2)
#define USART_CR1_TE             (1U << 3)

/* USART status flags */
#define USART_ISR_RXNE           (1U << 5)
#define USART_ISR_TC             (1U << 6)
#define USART_ISR_TXE            (1U << 7)

/* GPIO pin definitions */
#define USART2_TX_PIN            (1U << 2)
#define USART2_RX_PIN            (1U << 3)

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
    GPIOA_MODER |=  ((2U << 4) | (2U << 6));

    /* Configure PA2 and PA3 for AF7 (USART2) */
    GPIOA_AFRL &= ~((0xFU << 8) | (0xFU << 12));
    GPIOA_AFRL |=  ((7U << 8) | (7U << 12));

    /* Configure both pins as push-pull */
    GPIOA_OTYPER &= ~(USART2_TX_PIN | USART2_RX_PIN);

    /* No pull-up or pull-down */
    GPIOA_PUPDR &= ~((3U << 4) | (3U << 6));

    /* Disable USART before configuration */
    USART2_CR1 = 0U;
    USART2_CR2 = 0U;
    USART2_CR3 = 0U;

    /* Configure baud rate for 115200, oversampling by 16 */
    USART2_BRR =
        (PCLK1_FREQUENCY + (USART2_BAUDRATE / 2U))
        / USART2_BAUDRATE;

    /* Enable transmitter, receiver and USART */
    USART2_CR1 = USART_CR1_TE |
                 USART_CR1_RE |
                 USART_CR1_UE;
}

void USART2_WriteChar(uint8_t data)
{
    /* Wait until transmit data register is empty */
    while (!(USART2_ISR & USART_ISR_TXE))
    {
        /* Wait */
    }

    /* Write character into transmit data register */
    USART2_TDR = data;
}

void USART2_WriteString(const char *string)
{
    while (*string != '\0')
    {
        USART2_WriteChar((uint8_t)*string);
        string++;
    }

    /* Wait until final character is completely transmitted */
    while (!(USART2_ISR & USART_ISR_TC))
    {
        /* Wait */
    }
}

uint8_t USART2_ReadChar(void)
{
    /* Wait until a character is received */
    while (!(USART2_ISR & USART_ISR_RXNE))
    {
        /* Wait */
    }

    /* Read received character */
    return (uint8_t)(USART2_RDR & 0xFFU);
}

