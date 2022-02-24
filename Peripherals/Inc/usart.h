/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#ifndef NUCLEOPONG_USART_H
#define NUCLEOPONG_USART_H

#include "stm32l476xx.h"

#ifdef __cplusplus
extern "C" {
#endif

// USART2: UART Communication with minicom
#define USART_Init()    { UART2_Init(); \
                            USARTx_Init(USART2); }

void UART2_Init(void);

void USARTx_Init(USART_TypeDef* USARTx);

void USART2_IRQHandler(void);

void USART_Write(USART_TypeDef * USARTx, char *buffer, uint32_t nBytes);
char USART_Read(USART_TypeDef * USARTx);
void USART_Delay(uint32_t us);

#ifdef __cplusplus
}
#endif
#endif //NUCLEOPONG_USART_H
