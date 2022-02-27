/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#ifndef NUCLEOPONG_SPI_H
#define NUCLEOPONG_SPI_H

#include "stm32l4xx.h"

#ifdef __cplusplus
extern "C" {
#endif

void SPI_Init(void);
void SPI_Send_Data(SPI_TypeDef *SPIx, const uint8_t *write_data, uint32_t length);


#ifdef __cplusplus
}
#endif

#endif //NUCLEOPONG_SPI_H
