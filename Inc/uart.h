
#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdbool.h>

/* UART receive buffer configuration */
#define UART_RX_BUFFER_SIZE 64U

/* USART2 initialization */
void USART2_Init(void);

/* UART transmission functions */
void USART2_WriteChar(uint8_t data);
void USART2_WriteString(const char *string);

/* Interrupt-driven reception */
bool USART2_ReadChar(uint8_t *data);

/* Buffer status */
uint32_t USART2_GetRxOverflowCount(void);

#endif
