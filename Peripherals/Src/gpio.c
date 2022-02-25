/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */
#include "gpio.h"
#include "util.h"

static void I2C_GPIO_Init(void);
static void UART2_GPIO_Init(void);
static void GreenLED_Init(void);
static void BluePushbutton_Init(void);
static void USB_GPIO_Init(void);
static void LCD_GPIO_Init(void);

void GPIO_Init() {
    GreenLED_Init();
    BluePushbutton_Init();
    UART2_GPIO_Init();
    I2C_GPIO_Init();
    USB_GPIO_Init();
    LCD_GPIO_Init();

    // enable port H clock (crystal oscillators are here and it's supposedly useful)
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOHEN;
}

uint8_t GPIO_ReadPin(GPIO_TypeDef *gpio_port, uint8_t pin_no) {
    uint32_t mask = 0x1UL << (GPIO_IDR_ID0_Pos + pin_no);
    return (gpio_port->IDR & mask) != 0;
}

void GPIO_SetPin(GPIO_TypeDef *gpio_port, uint8_t pin_no) {
    uint32_t mask = 0x1UL << (GPIO_ODR_OD0_Pos + pin_no);
    gpio_port->ODR |= mask;
}

void GPIO_ClearPin(GPIO_TypeDef *gpio_port, uint8_t pin_no) {
    uint32_t mask = 0x1UL << (GPIO_ODR_OD0_Pos + pin_no);
    gpio_port->ODR &= ~mask;
}

void GPIO_TogglePin(GPIO_TypeDef *gpio_port, uint8_t pin_no) {
    uint32_t mask = 0x1UL << (GPIO_ODR_OD0_Pos + pin_no);
    gpio_port->ODR ^= mask;
}

// initialize the green LED (PA5)
static void GreenLED_Init(void) {
    // Enable GPIOA Clock
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    // Initialize Green LED
    GPIOA->MODER &= ~GPIO_MODER_MODE5_1; // Set the mode of PA5 to Output
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT5; // Set the output type of PA5 to Push-Pull
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5; // Set PA5 to No Pull-Up, No Pull-Down
}

static void BluePushbutton_Init(void) {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

    GPIOC->MODER &= ~GPIO_MODER_MODE13; // Set mode of PC13 to Input
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD13; // Set PC13 to no pull-up and no pull-down
}

//initialize PA12 AF10 (USB D+), PA11 AF10 (USB D-), and later (once I get a voltage regulator) PA9 input (USB VBUS)
static void USB_GPIO_Init(void) {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    GPIOA->MODER &= ~(GPIO_MODER_MODE11 | GPIO_MODER_MODE12);
    GPIOA->MODER |= (GPIO_MODER_MODE11_1 | GPIO_MODER_MODE12_1);
    GPIOA->AFR[1] &= ~(GPIO_AFRH_AFSEL11 | GPIO_AFRH_AFSEL12);
    GPIOA->AFR[1] |= (GPIO_AFRH_AFSEL11_3 | GPIO_AFRH_AFSEL11_1 |
            GPIO_AFRH_AFSEL12_3 | GPIO_AFRH_AFSEL12_1);

    // configure PA11 and PA12 to no PUPD
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD11 | GPIO_PUPDR_PUPD12);

    // configure output push-pull
    GPIOA->OTYPER &= ~(GPIO_OTYPER_OT11 | GPIO_OTYPER_OT12);

    // configure very high output speed
    GPIOA->OSPEEDR |= (GPIO_OSPEEDR_OSPEED11 | GPIO_OSPEEDR_OSPEED12);
}

// configure PB8 (D15) and PB9 (D14) to communicate over I2C 1
// PB8 AF4 is I2C1_SCL and PB9 AF4 is I2C1_SDA
static void I2C_GPIO_Init(void) {
    // enable clock for GPIO B
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

    // configure PB8 and PB9 to alt func. mode 4
    GPIOB->MODER &= ~(GPIO_MODER_MODE8 | GPIO_MODER_MODE9);
    GPIOB->MODER |= (GPIO_MODER_MODE8_1 | GPIO_MODER_MODE9_1);
    GPIOB->AFR[1] &= ~(GPIO_AFRH_AFSEL8 | GPIO_AFRH_AFSEL9);
    GPIOB->AFR[1] |= (GPIO_AFRH_AFSEL8_2 | GPIO_AFRH_AFSEL9_2);

    // configure PB8 & 9 to output pull-up
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD8 | GPIO_PUPDR_PUPD9);
    GPIOB->PUPDR |= (GPIO_PUPDR_PUPD8_0 | GPIO_PUPDR_PUPD9_0);

    // set output type to open-drain
    GPIOB->OTYPER |= (GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9);

    // set very high output speed
    GPIOB->OSPEEDR |= (GPIO_OSPEEDR_OSPEED8 | GPIO_OSPEEDR_OSPEED9);
}

// Configure PA2 and PA3 to operate as UART transmitters and receivers.
// PA2 has USART2_TX as AF7
// PA3 has USART2_RX as AF7
static void UART2_GPIO_Init(void) {
    // enable clock for gpio pins A2 and A3
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    // configure both to operate in AF7 mode (0111)
    GPIOA->MODER &= ~(GPIO_MODER_MODE2 | GPIO_MODER_MODE3);
    GPIOA->MODER |= (GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1);
    GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL2 | GPIO_AFRL_AFSEL3);
    GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL2_3 | GPIO_AFRL_AFSEL3_3);

    // configure both pins to pull up their I/O pins (01)
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD2 | GPIO_PUPDR_PUPD3);
    GPIOA->PUPDR |= (GPIO_PUPDR_PUPD2_0 | GPIO_PUPDR_PUPD3_0);

    // configure both pins to have push-pull output type (0)
    GPIOA->OTYPER &= ~(GPIO_OTYPER_OT2 | GPIO_OTYPER_OT3);

    // set both pins' output speed to "very high"
    GPIOA->OSPEEDR |= (GPIO_OSPEEDR_OSPEED2 | GPIO_OSPEEDR_OSPEED3);

}


// configure
static void LCD_GPIO_Init(void) {
    RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN);

    // configure AF5 for MOSI and SCK pins
    GPIOA->MODER &= ~(GPIO_MODER_MODE5 | GPIO_MODER_MODE7);
    GPIOA->MODER |= (GPIO_MODER_MODE5_1 | GPIO_MODER_MODE7_1);
    GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL5 | GPIO_AFRL_AFRL7);
    GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL5_2 | GPIO_AFRL_AFSEL7_2);
    GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL5_0 | GPIO_AFRL_AFSEL7_0);

    // configure general purpose output mode for CS, DC, and RST pins
    GPIOA->MODER &= ~GPIO_MODER_MODE9; // RST (PA9)
    GPIOA->MODER |= GPIO_MODER_MODE9_0;
    GPIOB->MODER &= ~GPIO_MODER_MODE6; // CS (PB6)
    GPIOB->MODER |= GPIO_MODER_MODE6_0;
    GPIOC->MODER &= ~GPIO_MODER_MODE7; // DC (PC7)
    GPIOC->MODER |= GPIO_MODER_MODE7_0;

    // set no PUPD
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD5 | GPIO_PUPDR_PUPD7 | GPIO_PUPDR_PUPD9);
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD6;
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD7;

    // set output type to push-pull
    LCD_DC_GPIO->OTYPER &= ~GPIO_OTYPER_OT7;
    LCD_CS_GPIO->OTYPER &= ~GPIO_OTYPER_OT6;
    LCD_MOSI_GPIO->OTYPER &= ~GPIO_OTYPER_OT7;
    LCD_SCK_GPIO->OTYPER &= ~GPIO_OTYPER_OT5;
    LCD_RST_GPIO->OTYPER &= ~GPIO_OTYPER_OT9;

    // set very high output speed
    GPIOA->OSPEEDR |= (GPIO_OSPEEDR_OSPEED5 | GPIO_OSPEEDR_OSPEED7 | GPIO_OSPEEDR_OSPEED9);
    GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED6;
    GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED7;
}