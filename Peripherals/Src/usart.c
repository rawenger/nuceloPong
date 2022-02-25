/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#include "usart.h"

#define USART_BAUD_RATE     9600
#define CLOCK_FREQ          80000000U

void UART2_Init(void) {
    // Enable the USART2 clock in the peripheral clock register.
    RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;

    // Select the system clock as the USART2 clock source in the peripheral independent clock configuration register.
    // (01)
    RCC->CCIPR &= ~RCC_CCIPR_USART2SEL;
    RCC->CCIPR |= RCC_CCIPR_USART2SEL_0;
}



void USARTx_Init(USART_TypeDef* USARTx) {
    // ensure USART is disabled before setting configuration
    USARTx->CR1 &= ~USART_CR1_UE;
    while (USARTx->CR1 & USART_CR1_UE);

    // set word length to 8 bits and number of stop bits to 1
    // might need to change this -- see section 36.8.1 of the manual
    USARTx->CR1 &= ~USART_CR1_M;
//    USARTx->CR1 |= USART_CR1_M1;
    USARTx->CR2 &= ~USART_CR2_STOP;
    // set oversampling mode to oversample by 16
    USARTx->CR1 &= ~USART_CR1_OVER8;

    // Set the baud rate to 9600
    // baud rate = f_CLK / USARTDIV
    // --> BRR = USARTDIV = f_CLK / baud rate
    USARTx->BRR = CLOCK_FREQ / USART_BAUD_RATE;

    // enable Rx and Tx
    USARTx->CR1 |= (USART_CR1_TE | USART_CR1_RE);

    // enable USART
    USARTx->CR1 |= USART_CR1_UE;

}

char USART_Read (USART_TypeDef * USARTx) {
    // SR_RXNE (Read data register not empty) bit is set by hardware
    while (!(USARTx->ISR & USART_ISR_RXNE));  // Wait until RXNE (RX not empty) bit is set
    // USART resets the RXNE flag automatically after reading DR
    return ((char)(USARTx->RDR & 0xFF));
    // Reading USART_DR automatically clears the RXNE flag
}

void USART_Write(USART_TypeDef * USARTx, char *buffer, uint32_t nBytes) {
    int i;
    // TXE is cleared by a write to the USART_DR register.
    // TXE is set by hardware when the content of the TDR
    // register has been transferred into the shift register.
    for (i = 0; i < nBytes; i++) {
        while (!(USARTx->ISR & USART_ISR_TXE));   	// wait until TXE (TX empty) bit is set
        // Writing USART_DR automatically clears the TXE flag
        USARTx->TDR = buffer[i] & 0xFF;
        USART_Delay(300);
    }
    while (!(USARTx->ISR & USART_ISR_TC));   		  // wait until TC bit is set
    USARTx->ISR &= ~USART_ISR_TC;
}

void USART_Delay(uint32_t us) {
    volatile uint32_t time = 100*us/7;
    while(--time);
}
