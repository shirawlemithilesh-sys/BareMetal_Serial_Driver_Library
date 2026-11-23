/*
 * i2c.h
 *
 *  Created on: Nov 23, 2026
 *      Author: HP
 */

#ifndef I2C_H
#define I2C_H

#include <stdint.h>

void I2C1_Init(void);
uint8_t I2C1_WriteByte(uint8_t address, uint8_t data);

#endif
