
#include <stdint.h>

#include "gpio.h"
#include "uart.h"

/* Cortex-M4F FPU initialization */
#define SCB_CPACR (*(volatile uint32_t *)0xE000ED88)

static void FPU_Init(void)
{
    SCB_CPACR |= (0xFU << 20);

    __asm volatile ("dsb");
    __asm volatile ("isb");
}

/* Software delay */
static void delay(void)
{
    for (volatile uint32_t i = 0; i < 500000U; i++)
    {
        __asm volatile ("nop");
    }
}


int main(void)
{
    FPU_Init();

    GPIO_Init();

    USART2_Init();

    USART2_WriteString("UART ECHO TEST READY!\r\n");
    USART2_WriteString("Type something below:\r\n");

    while (1)
    {
        /* Receive one character */
        uint8_t received = USART2_ReadChar();

        /* Echo received character back */
        if (received == '\r')
        {
            USART2_WriteString("\r\n");
        }
        else
        {
            USART2_WriteChar(received);
        }
    }
}
