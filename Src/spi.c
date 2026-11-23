/*
 * spi.c
 *
 *  Created on: Nov 23, 2026
 *      Author: HP
 */



#include <stdint.h>
#include "spi.h"

/* RCC registers */
#define RCC_AHB2ENR   (*(volatile uint32_t *)0x4002104C)
#define RCC_APB2ENR   (*(volatile uint32_t *)0x40021060)

/* GPIOA registers */
#define GPIOA_MODER   (*(volatile uint32_t *)0x48000000)
#define GPIOA_OTYPER  (*(volatile uint32_t *)0x48000004)
#define GPIOA_OSPEEDR (*(volatile uint32_t *)0x48000008)
#define GPIOA_PUPDR   (*(volatile uint32_t *)0x4800000C)
#define GPIOA_AFRL    (*(volatile uint32_t *)0x48000020)

/* SPI1 registers */
#define SPI1_CR1      (*(volatile uint32_t *)0x40013000)
#define SPI1_CR2      (*(volatile uint32_t *)0x40013004)
#define SPI1_SR       (*(volatile uint32_t *)0x40013008)
#define SPI1_DR       (*(volatile uint32_t *)0x4001300C)

/* SPI status flags */
#define SPI_SR_RXNE   (1U << 0)
#define SPI_SR_TXE    (1U << 1)
#define SPI_SR_BSY    (1U << 7)

void SPI1_Init(void)
{
    /* Enable GPIOA and SPI1 clocks */
    RCC_AHB2ENR |= (1U << 0);
    RCC_APB2ENR |= (1U << 12);

    /* Configure PA5, PA6, PA7 as Alternate Function */
    GPIOA_MODER &= ~(0x3FU << 10);
    GPIOA_MODER |=  (0x2AU << 10);

    /* Select AF5 for PA5, PA6, PA7 */
    GPIOA_AFRL &= ~(0xFFFU << 20);
    GPIOA_AFRL |=  (0x555U << 20);

    /* Push-pull, high speed, no pull-up/down */
    GPIOA_OTYPER &= ~(7U << 5);
    GPIOA_OSPEEDR |= (0x3FU << 10);
    GPIOA_PUPDR &= ~(0x3FU << 10);

    /* Disable SPI before configuration */
    SPI1_CR1 = 0U;
    SPI1_CR2 = 0U;

    /* Master mode, clock /16, software NSS */
    SPI1_CR1 = (1U << 2) |
               (3U << 3) |
               (1U << 8) |
               (1U << 9);

    /* 8-bit data frame, 8-bit RX FIFO threshold */
    SPI1_CR2 = (7U << 8) | (1U << 12);

    /* Enable SPI */
    SPI1_CR1 |= (1U << 6);
}

uint8_t SPI1_Transfer(uint8_t data)
{
    /* Wait until transmit FIFO has space */
    while (!(SPI1_SR & SPI_SR_TXE))
    {
    }

    /* Transmit one byte */
    *((volatile uint8_t *)&SPI1_DR) = data;

    /* Wait for received byte */
    while (!(SPI1_SR & SPI_SR_RXNE))
    {
    }

    /* Read received byte */
    uint8_t received = *((volatile uint8_t *)&SPI1_DR);

    /* Wait until transfer finishes */
    while (SPI1_SR & SPI_SR_BSY)
    {
    }

    return received;
}
