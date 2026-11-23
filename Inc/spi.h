/*
 * spi.h
 *
 *  Created on: Nov 23, 2026
 *      Author: HP
 */


#ifndef SPI_H
#define SPI_H

#include <stdint.h>

void SPI1_Init(void);
uint8_t SPI1_Transfer(uint8_t data);

#endif

