
#include <stdint.h>

#include "gpio.h"
#include "uart.h"
#include "spi.h"

/* Cortex-M4F FPU initialization */

#define SCB_CPACR (*(volatile uint32_t *)0xE000ED88)

static void FPU_Init(void)
{
    SCB_CPACR |= (0xFU << 20);

    __asm volatile ("dsb");
    __asm volatile ("isb");
}

/* Transmit an unsigned integer without printf */

static void USART2_WriteUInt32(uint32_t value)
{
    char digits[10];
    uint8_t index = 0U;

    if (value == 0U)
    {
        USART2_WriteChar('0');
        return;
    }

    while (value > 0U)
    {
        digits[index++] = (char)('0' + (value % 10U));
        value /= 10U;
    }

    while (index > 0U)
    {
        USART2_WriteChar((uint8_t)digits[--index]);
    }
}


int main(void)
{
    uint8_t received;
    uint8_t spi_received;

    FPU_Init();

    GPIO_Init();

    USART2_Init();
    SPI1_Init();

    spi_received = SPI1_Transfer(0xA5);

    USART2_WriteString("\r\n");
    USART2_WriteString("================================\r\n");
    USART2_WriteString(" STM32L476RG UART TEST\r\n");
    USART2_WriteString(" Interrupt-driven reception\r\n");
    USART2_WriteString("================================\r\n");
    USART2_WriteString("Type something below:\r\n");


    USART2_WriteString("SPI1 Loopback RX (decimal): ");
    USART2_WriteUInt32(spi_received);
    USART2_WriteString("\r\n");
    while (1)
    {
        if (USART2_ReadChar(&received))
        {
            if (received == '\r')
            {
                USART2_WriteString("\r\n");

                USART2_WriteString("RX Buffer Overflow Count: ");

                USART2_WriteUInt32(
                    USART2_GetRxOverflowCount()
                );

                USART2_WriteString("\r\n");

                USART2_WriteString("Type something below:\r\n");
            }
            else if (received != '\n')
            {
                USART2_WriteChar(received);
            }


        }
    }
}
