/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#include "spi.h"

void SPI_Init(void){
    // (a) Enable the SPI clock
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    // (b) Set the RCC SPI reset bit, then clear it to reset the SPI1 peripheral
    RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST;
    while (!(RCC->APB2RSTR & RCC_APB2RSTR_SPI1RST)) ;
    RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1RST;

    // (c) Disable the SPI enable bit. The peripheral must be configured while it is disabled.
    SPI1->CR1 &= ~SPI_CR1_SPE;

    // (d) Configure the peripheral for full-duplex communication.
    SPI1->CR1 &= ~SPI_CR1_RXONLY;

    // (e) Configure the peripheral for 2-line unidirectional data mode.
    SPI1->CR1 &= ~SPI_CR1_BIDIMODE;

    // (f) Disable output in bidirectional mode.
    SPI1->CR1 &= ~SPI_CR1_BIDIOE;
//    SPI1->CR1 |= SPI_CR1_BIDIOE;

    // (g) Configure the frame format as MSB first.
    SPI1->CR1 &= ~SPI_CR1_LSBFIRST;

    // (h) Configure the frame format to 8-bit mode.
    SPI1->CR2 |= SPI_CR2_DS;
    SPI1->CR2 &= ~SPI_CR2_DS_3;

    // (i) Use Motorola SPI mode.
    SPI1->CR2 &= ~SPI_CR2_FRF;

    // (j) Configure the clock to low polarity.
    SPI1->CR1 &= ~SPI_CR1_CPOL;

    // (k) Configure the clock to first clock transition.
    SPI1->CR1 &= ~SPI_CR1_CPHA;

    // (l) Set the baud rate prescaler to 2.
    SPI1->CR1 &= ~SPI_CR1_BR;

    // (m) Disable hardware CRC calculation.
    SPI1->CR1 &= ~SPI_CR1_CRCEN;

    // (n) Set SPI1 to master mode.
    SPI1->CR1 |= SPI_CR1_MSTR;

    // (o) Enable software SSM and internal slave select (idk if the 2nd is necessary).
    SPI1->CR1 |= SPI_CR1_SSM;
    SPI1->CR1 |= SPI_CR1_SSI;

    // (p) Disable NSS pulse generation.
    // maybe need to enable tho
//    SPI1->CR2 &= ~SPI_CR2_NSSP;
    SPI1->CR2 |= SPI_CR2_NSSP;

    // (q) Set the FIFO threshold to 1/4 (required for 8-bit mode).
    SPI1->CR2 |= SPI_CR2_FRXTH;

    // (r) Enable the SPI peripheral.
    SPI1->CR1 |= SPI_CR1_SPE;
}

void SPI_Send_Data(SPI_TypeDef *SPIx, const uint8_t *write_data, uint32_t length) {
    // don't wait on the buffer if there's no transmission
    if (length == 0)
        return;

    // wait for transmission buffer to empty
    while (!(SPIx->SR & SPI_SR_TXE)) ;

    // write to the data register in packed mode if this is a multibyte transmission
    for (; length > 1; length -= sizeof(uint16_t)) {
        while (!(SPIx->SR & SPI_SR_TXE)) ;
        SPIx->DR = *((uint16_t *) write_data);
        write_data += sizeof(uint16_t);
    }

    // write the last (or only) byte if necessary
    if (length > 0)
        *((volatile uint8_t *) &SPIx->DR) = *write_data;

    // don't return until the SPI is no longer busy; otherwise weird stuff happens sometimes
    while (SPIx->SR & SPI_SR_BSY) ;
}