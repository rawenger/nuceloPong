/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#include "spi.h"

void SPI_Init(void){
    // (a) Enable the SPI clock
    RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN;

    // (b) Set the RCC SPI reset bit, then clear it to reset the SPI2 peripheral
    RCC->APB1RSTR1 |= RCC_APB1RSTR1_SPI2RST;
    while (!(RCC->APB1RSTR1 & RCC_APB1RSTR1_SPI2RST)) ;
    RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_SPI2RST;

    // (c) Disable the SPI enable bit. The peripheral must be configured while it is disabled.
    SPI2->CR1 &= ~SPI_CR1_SPE;

    // (d) Configure the peripheral for full-duplex communication.
    SPI2->CR1 &= ~SPI_CR1_RXONLY;

    // (e) Configure the peripheral for 2-line unidirectional data mode.
    SPI2->CR1 &= ~SPI_CR1_BIDIMODE;

    // (f) Disable output in bidirectional mode.
    SPI2->CR1 &= ~SPI_CR1_BIDIOE;

    // (g) Configure the frame format as MSB first.
    SPI2->CR1 &= ~SPI_CR1_LSBFIRST;

    // (h) Configure the frame format to 8-bit mode.
    SPI2->CR2 |= SPI_CR2_DS;
    SPI2->CR2 &= ~SPI_CR2_DS_3;

    // (i) Use Motorola SPI mode.
    SPI2->CR2 &= ~SPI_CR2_FRF;

    // (j) Configure the clock to low polarity.
    SPI2->CR1 &= ~SPI_CR1_CPOL;

    // (k) Configure the clock to first clock transition.
    SPI2->CR1 &= ~SPI_CR1_CPHA;

    // (l) Set the baud rate prescaler to 16.
    SPI2->CR1 |= SPI_CR1_BR;
    SPI2->CR1 &= ~SPI_CR1_BR_2;

    // (m) Disable hardware CRC calculation.
    SPI2->CR1 &= ~SPI_CR1_CRCEN;

    // (n) Set SPI2 to master mode.
    SPI2->CR1 |= SPI_CR1_MSTR;

    // (o) Enable software SSM.
    SPI2->CR1 |= SPI_CR1_SSM;

    // (p) Disable NSS pulse generation.
    // maybe need to enable tho
    SPI2->CR2 &= ~SPI_CR2_NSSP;

    // (q) Set the FIFO threshold to 1/4 (required for 8-bit mode).
    SPI2->CR2 |= SPI_CR2_FRXTH;

    // (r) Enable the SPI peripheral.
    SPI2->CR1 |= SPI_CR1_SPE;
}

// TODO implement multibyte transfer
void SPI_Transfer_Byte(SPI_TypeDef *SPIx, uint8_t write_data) {
    // (a) Wait for the Transmit Buffer Empty flag to become set.
    while (!(SPIx->SR & SPI_SR_TXE)) ;

    // (b) Write data to the SPIx->DR register to begin transmission. Note that the register memory address
    // must be cast to (volatile uint8 t*) and dereferenced when using the 8-bit format.
    volatile uint8_t *dr = (uint8_t *) &(SPIx->DR);
    *dr = write_data;

    // (c) Wait for the Busy to become unset for the transmission to complete.
    while (SPIx->SR & SPI_SR_BSY) ;

    // (d) Wait for the Receive Not Empty flag to set for the data to be received.
//    while (!(SPIx->SR & SPI_SR_RXNE)) ;

    // (e) Read received data from the SPIx->DR register. Again note that the register memory address must be
    // cast to (volatile uint8 t*) and dereferenced when using the 8-bit format.
//    *read_data = *dr;
}