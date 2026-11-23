/*
 * i2c.c
 *
 *  Created on: Nov 23, 2026
 *      Author: HP
 */



#include <stdint.h>
#include "i2c.h"

/* RCC registers */
#define RCC_AHB2ENR     (*(volatile uint32_t *)0x4002104C)
#define RCC_APB1ENR1    (*(volatile uint32_t *)0x40021058)
#define RCC_CCIPR       (*(volatile uint32_t *)0x40021088)

/* GPIOB registers */
#define GPIOB_MODER     (*(volatile uint32_t *)0x48000400)
#define GPIOB_OTYPER    (*(volatile uint32_t *)0x48000404)
#define GPIOB_OSPEEDR   (*(volatile uint32_t *)0x48000408)
#define GPIOB_PUPDR     (*(volatile uint32_t *)0x4800040C)
#define GPIOB_AFRH      (*(volatile uint32_t *)0x48000424)

/* I2C1 registers */
#define I2C1_CR1        (*(volatile uint32_t *)0x40005400)
#define I2C1_CR2        (*(volatile uint32_t *)0x40005404)
#define I2C1_TIMINGR    (*(volatile uint32_t *)0x40005410)
#define I2C1_ISR        (*(volatile uint32_t *)0x40005418)
#define I2C1_ICR        (*(volatile uint32_t *)0x4000541C)
#define I2C1_TXDR       (*(volatile uint32_t *)0x40005428)

/* I2C status flags */
#define I2C_ISR_TXIS    (1U << 1)
#define I2C_ISR_NACKF   (1U << 4)
#define I2C_ISR_STOPF   (1U << 5)
#define I2C_ISR_BUSY    (1U << 15)

void I2C1_Init(void)
{
    /* Enable GPIOB clock */
    RCC_AHB2ENR |= (1U << 1);

    /* Enable I2C1 clock */
    RCC_APB1ENR1 |= (1U << 21);

    /* Select PCLK1 as I2C1 clock source */
    RCC_CCIPR &= ~(3U << 12);

    /* Configure PB8 and PB9 as alternate function */
    GPIOB_MODER &= ~(0xFU << 16);
    GPIOB_MODER |=  (0xAU << 16);

    /* Configure PB8 and PB9 as open-drain */
    GPIOB_OTYPER |= (3U << 8);

    /* Medium GPIO speed */
    GPIOB_OSPEEDR &= ~(0xFU << 16);
    GPIOB_OSPEEDR |=  (0x5U << 16);

    /* No internal pull-up or pull-down */
    GPIOB_PUPDR &= ~(0xFU << 16);

    /* Select AF4 for PB8 and PB9 */
    GPIOB_AFRH &= ~(0xFFU);
    GPIOB_AFRH |=  (0x44U);

    /* Disable I2C before configuration */
    I2C1_CR1 &= ~(1U << 0);

    /*
     * 100 kHz standard mode
     * Assumes I2C kernel clock = PCLK1 = 4 MHz.
     */
    I2C1_TIMINGR = 0x00420F13U;

    /* Enable I2C1 peripheral */
    I2C1_CR1 |= (1U << 0);
}

/* Write one byte to a 7-bit I2C slave address.
   Returns 1 = ACK, 0 = NACK/error. */

uint8_t I2C1_WriteByte(uint8_t address, uint8_t data)
{
    uint32_t timeout = 1000000U;

    /* Wait until bus is free */
    while (I2C1_ISR & I2C_ISR_BUSY)
    {
        if (--timeout == 0U)
            return 0;
    }

    /* Clear previous STOP and NACK flags */
    I2C1_ICR = (1U << 4) | (1U << 5);

    /*
     * Configure:
     * 7-bit slave address
     * Write operation
     * 1 byte
     * Automatic STOP
     */
    I2C1_CR2 = ((uint32_t)address << 1) |
               (1U << 16) |
               (1U << 25) |
               (1U << 13);

    timeout = 1000000U;

    /* Wait for transmit-ready or NACK */
    while (!(I2C1_ISR & I2C_ISR_TXIS))
    {
        if (I2C1_ISR & I2C_ISR_NACKF)
        {
            I2C1_ICR = (1U << 4) | (1U << 5);
            return 0;
        }

        if (--timeout == 0U)
            return 0;
    }

    /* Send data */
    I2C1_TXDR = data;

    timeout = 1000000U;

    /* Wait for STOP or NACK */
    while (!(I2C1_ISR & I2C_ISR_STOPF))
    {
        if (I2C1_ISR & I2C_ISR_NACKF)
        {
            I2C1_ICR = (1U << 4) | (1U << 5);
            return 0;
        }

        if (--timeout == 0U)
            return 0;
    }

    /* Clear STOP flag */
    I2C1_ICR = (1U << 5);

    return 1;
}
