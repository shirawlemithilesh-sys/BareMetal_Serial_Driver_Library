/*
 * gpio.c
 *
 *  Created on: Nov 20, 2026
 *      Author: HP
 */


#include "gpio.h"

/* RCC register */
#define RCC_AHB2ENR   (*(volatile uint32_t *)0x4002104C)

/* GPIOA registers */
#define GPIOA_MODER   (*(volatile uint32_t *)0x48000000)
#define GPIOA_BSRR    (*(volatile uint32_t *)0x48000018)

/* GPIOA clock enable bit */
#define GPIOA_EN      (1U << 0)

/* PA5 pin definitions */
#define LED_PIN       (1U << 5)

void GPIO_Init(void)
{
    /* Enable GPIOA peripheral clock */
    RCC_AHB2ENR |= GPIOA_EN;

    /* Read back to ensure clock enable is effective */
    (void)RCC_AHB2ENR;

    /* Configure PA5 as general-purpose output */
    GPIOA_MODER &= ~(3U << 10);
    GPIOA_MODER |=  (1U << 10);

    /* Initially switch LED OFF */
    GPIO_LED_OFF();
}

void GPIO_LED_ON(void)
{
    /* Set PA5 HIGH */
    GPIOA_BSRR = LED_PIN;
}

void GPIO_LED_OFF(void)
{
    /* Reset PA5 LOW */
    GPIOA_BSRR = (LED_PIN << 16);
}

