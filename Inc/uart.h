/*
 * uart.h
 *
 *  Created on: Nov 20, 2026
 *      Author: HP
 */


#ifndef UART_H
#define UART_H

#include <stdint.h>

void USART2_Init(void);

void USART2_WriteChar(uint8_t data);

void USART2_WriteString(const char *string);

uint8_t USART2_ReadChar(void);

#endif
