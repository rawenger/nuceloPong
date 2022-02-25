/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#include "i2c.h"

extern void Error_Handler(void);

// Inter-integrated Circuit Interface (I2C)
// up to 100 Kbit/s in the standard mode,
// up to 400 Kbit/s in the fast mode, and
// up to 3.4 Mbit/s in the high-speed mode.

// Recommended external pull-up resistance is
// 4.7 kOmh for low speed,
// 3.0 kOmh for the standard mode, and
// 1.0 kOmh for the fast mode

//===============================================================================
//                        I2C GPIO Initialization
//===============================================================================


#define I2C_TIMINGR_PRESC_POS	__builtin_ctz(I2C_TIMINGR_PRESC) // 28
#define I2C_TIMINGR_SCLDEL_POS	__builtin_ctz(I2C_TIMINGR_SCLDEL) // 20
#define I2C_TIMINGR_SDADEL_POS	__builtin_ctz(I2C_TIMINGR_SDADEL) // 16
#define I2C_TIMINGR_SCLH_POS	__builtin_ctz(I2C_TIMINGR_SCLH) // 8
#define I2C_TIMINGR_SCLL_POS	__builtin_ctz(I2C_TIMINGR_SCLL) // 0

// Required parameters for TC74
//#define T_I2CCLK            125 // 1/10 ns
//#define TC74_CLK            200 // based on max prescaler value
//
//#define TC74_MIN_SETUP      1000 // min. 1000 ns
//#define TC74_MIN_HOLD       1400 // min. 1250 ns
//#define TC74_MIN_CLK_HI     4000 // min. 4000 ns
//#define TC74_MIN_CLK_LO     4800 // min. 4700 ns
//
//#define I2C1_T_PRESC        TC74_CLK // ns
//#define I2C1_PRESC          ((10 * I2C1_T_PRESC) / T_I2CCLK - 1) // ns
//#define I2C1_SCLDEL         (TC74_MIN_SETUP / I2C1_T_PRESC - 1) // ns
//#define I2C1_SDADEL         (TC74_MIN_HOLD / I2C1_T_PRESC - 1) // ns
//#define I2C1_SCLH           (TC74_MIN_CLK_HI / I2C1_T_PRESC - 1) // ns
//#define I2C1_SCLL           (TC74_MIN_CLK_LO / I2C1_T_PRESC - 1) // ns

//===============================================================================
//                          I2C Initialization
//===============================================================================
void I2C_Init(void){
    uint32_t OwnAddr = 0x52;

    // Enable the clock for I2C1 in the peripheral clock enable register
    RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN;
    // Set the 80 MHz PCLK as the clock source for I2C1 in the peripherals independent clock
    // configuration register
    RCC->CCIPR &= ~RCC_CCIPR_I2C1SEL;
    // Reset I2C1 by setting bits in the peripheral reset register. After doing so, clear the
    // bits so that I2C1 does not remain in a reset state.
    RCC->APB1RSTR1 |= RCC_APB1RSTR1_I2C1RST;
    RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_I2C1RST;

    /* Configure the I2C registers for communication with the temperature sensor */
    // Disable the I2C peripheral lines so we can write configuration bits
    I2C1->CR1 &= ~I2C_CR1_PE;
    // Enable the analog noise filter
    I2C1->CR1 &= ~I2C_CR1_ANFOFF;
    // Disable the digital noise filter
    I2C1->CR1 &= ~I2C_CR1_DNF;
    // Enable error interrupts
    I2C1->CR1 |= I2C_CR1_ERRIE;
    // Enable clock stretching
    I2C1->CR1 &= ~I2C_CR1_NOSTRETCH;
    // Set the master to operate in 7-bit addressing mode
    I2C1->CR2 &= ~I2C_CR2_ADD10;
    // Enable automatic end mode and NACK generation
    I2C1->CR2 |= I2C_CR2_AUTOEND;
    I2C1->CR2 |= I2C_CR2_NACK;

    /* Set the values in the timing register.
     * This guarantees correct data hold and setup times that are used in master/peripheral modes.
     */
    I2C1->TIMINGR = (0xdfffffffU & 0xf0ffffffU);

    /* Set your own address in the own address registers */
    // disable own address (required in order to modify it)
    I2C1->OAR1 &= ~I2C_OAR1_OA1EN;
    I2C1->OAR2 &= ~I2C_OAR2_OA2EN;
    // set own address to 7-bit mode
    I2C1->OAR1 &= ~I2C_OAR1_OA1MODE;
    // set own address to be used (bits 9, 8, and 0 are don't cares in 7-bit mode)
    I2C1->OAR1 = OwnAddr << 1;
    // enable own address 1
    I2C1->OAR1 |= I2C_OAR1_OA1EN;

    /* Re-enable I2C */
    I2C1->CR1 |= I2C_CR1_PE;
}

//===============================================================================
//                           I2C Start
// Master generates START condition:
//    -- Slave address: 7 bits
//    -- Automatically generate a STOP condition after all bytes have been transmitted
// Direction = 0: Master requests a write transfer
// Direction = 1: Master requests a read transfer
//===============================================================================
int8_t I2C_Start(I2C_TypeDef * I2Cx, uint32_t DevAddress, uint8_t Size, uint8_t Direction) {

    // Direction = 0: Master requests a write transfer
    // Direction = 1: Master requests a read transfer

    uint32_t tmpreg = 0;

    // This bit is set by software, and cleared by hardware after the Start followed by the address
    // sequence is sent, by an arbitration loss, by a timeout error detection, or when PE = 0.
    tmpreg = I2Cx->CR2;

    tmpreg &= (uint32_t)~((uint32_t)(I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | I2C_CR2_RD_WRN | I2C_CR2_START | I2C_CR2_STOP));

    if (Direction == READ_FROM_SLAVE)
        tmpreg |= I2C_CR2_RD_WRN;  // Read from Slave
    else
        tmpreg &= ~I2C_CR2_RD_WRN; // Write to Slave

    tmpreg |= (uint32_t)(((uint32_t)DevAddress & I2C_CR2_SADD) | (((uint32_t)Size << 16 ) & I2C_CR2_NBYTES));

    tmpreg |= I2C_CR2_START;

    I2Cx->CR2 = tmpreg;

    return 0;  // Success
}

//===============================================================================
//                           I2C Stop
//===============================================================================
void I2C_Stop(I2C_TypeDef * I2Cx){
    // Master: Generate STOP bit after the current byte has been transferred
    I2Cx->CR2 |= I2C_CR2_STOP;
    // Wait until STOPF flag is reset
    while( (I2Cx->ISR & I2C_ISR_STOPF) == 0 );
}

//===============================================================================
//                           Wait for the bus is ready
//===============================================================================
void I2C_WaitLineIdle(I2C_TypeDef * I2Cx){
    // Wait until I2C bus is ready
    while( (I2Cx->ISR & I2C_ISR_BUSY) == I2C_ISR_BUSY );	// If busy, wait
}

//===============================================================================
//                           I2C Send Data
//===============================================================================
int8_t I2C_SendData(I2C_TypeDef * I2Cx, uint8_t DeviceAddress, const uint8_t *pData, uint8_t Size) {
    int i;

    if (Size <= 0 || pData == NULL) return -1;

    I2C_WaitLineIdle(I2Cx);

    if (I2C_Start(I2Cx, DeviceAddress, Size, WRITE_TO_SLAVE) < 0 ) return -1;

    // Send Data
    // Write the first data in DR register
    // while((I2Cx->ISR & I2C_ISR_TXE) == 0);
    // I2Cx->TXDR = pData[0] & I2C_TXDR_TXDATA;

    for (i = 0; i < Size; i++) {
        // TXE is set by hardware when the I2C_TXDR register is empty. It is cleared when the next
        // data to be sent is written in the I2C_TXDR register.
        // while( (I2Cx->ISR & I2C_ISR_TXE) == 0 );

        // TXIS bit is set by hardware when the I2C_TXDR register is empty and the data to be
        // transmitted must be written in the I2C_TXDR register. It is cleared when the next data to be
        // sent is written in the I2C_TXDR register.
        // The TXIS flag is not set when a NACK is received.
        while((I2Cx->ISR & I2C_ISR_TXIS) == 0 );
        I2Cx->TXDR = pData[i] & I2C_TXDR_TXDATA;  // TXE is cleared by writing to the TXDR register.
    }

    // Wait until TC flag is set
    while((I2Cx->ISR & I2C_ISR_TC) == 0 && (I2Cx->ISR & I2C_ISR_NACKF) == 0);

    if( (I2Cx->ISR & I2C_ISR_NACKF) != 0 ) return -1;

    I2C_Stop(I2Cx);
    return 0;
}


//===============================================================================
//                           I2C Receive Data
//===============================================================================
int8_t I2C_ReceiveData(I2C_TypeDef * I2Cx, uint8_t DeviceAddress, uint8_t *pData, uint8_t Size) {
    int i;

    if(Size <= 0 || pData == NULL) return -1;

    I2C_WaitLineIdle(I2Cx);

    I2C_Start(I2Cx, DeviceAddress, Size, READ_FROM_SLAVE); // 0 = sending data to the slave, 1 = receiving data from the slave

    for (i = 0; i < Size; i++) {
        // Wait until RXNE flag is set
        while( (I2Cx->ISR & I2C_ISR_RXNE) == 0 );
        pData[i] = I2Cx->RXDR & I2C_RXDR_RXDATA;
    }

    // Wait until TCR flag is set
    while((I2Cx->ISR & I2C_ISR_TC) == 0);

    I2C_Stop(I2Cx);

    return 0;
}
