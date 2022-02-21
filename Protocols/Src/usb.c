/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#include "usb.h"

static void USB_Core_Init(void);
static void USB_Device_Init(void);

void USB_Init(void) {
    USB_Core_Init();
}

static void USB_Core_Init(void) {
    // reset USB?
    RCC->AHB2RSTR |= RCC_AHB2RSTR_OTGFSRST;
    while (!(RCC->AHB2RSTR & RCC_AHB2RSTR_OTGFSRST)) ;
    RCC->AHB2RSTR &= ~RCC_AHB2RSTR_OTGFSRST;

    RCC->AHB2ENR |= RCC_AHB2ENR_OTGFSEN;

    // enable power interface clock
    RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;

    /* The application must perform the core initialization sequence. If the cable is connected during power-up,
     * the current mode of operation bit in the OTG_GINTSTS (CMOD bit in OTG_GINTSTS) reflects the mode. The
     * OTG_FS controller enters host mode when an “A” plug is connected or device mode when a “B” plug is connected.
     * This section explains the initialization of the OTG_FS controller after power-on. The application must
     * follow the initialization sequence irrespective of host or device mode operation. All core global registers
     * are initialized according to the core’s configuration:
     * 1. Program the following fields in the OTG_GAHBCFG register:
     *      – Global interrupt mask bit GINTMSK = 1
     *      – Rx FIFO non-empty (RXFLVL bit in OTG_GINTSTS)
     *      – Periodic Tx FIFO empty level
     * 2. Program the following fields in the OTG_GUSBCFG register:
     *      – HNP capable bit
     *      – SRP capable bit
     *      – OTG_FS timeout calibration field
     *      – USB turnaround time field
     * 3. The software must unmask the following bits in the OTG_GINTMSK register:
     *      - OTG interrupt mask
     *      - Mode mismatch interrupt mask
     * 4. The software can read the CMOD bit in OTG_GINTSTS to determine whether the OTG_FS controller is
     *   operating in host or device mode.
     */
    USB_OTG_FS->GAHBCFG |= USB_OTG_GAHBCFG_GINT;
    USB_OTG_FS->GINTSTS |= USB_OTG_GINTSTS_RXFLVL;
    USB_OTG_FS->GINTSTS &= ~USB_OTG_GAHBCFG_PTXFELVL;

    USB_OTG_FS->GUSBCFG &= ~USB_OTG_GUSBCFG_HNPCAP;
    USB_OTG_FS->GUSBCFG &= ~USB_OTG_GUSBCFG_SRPCAP;
    USB_OTG_FS->GUSBCFG &= ~USB_OTG_GUSBCFG_TOCAL;
    USB_OTG_FS->GUSBCFG &= ~USB_OTG_GUSBCFG_TRDT;
    USB_OTG_FS->GUSBCFG |= (USB_OTG_GUSBCFG_TRDT_2 | USB_OTG_GUSBCFG_TRDT_0); // TRDT = 5: 48 MHz

    USB_OTG_FS->GINTMSK &= ~USB_OTG_GINTMSK_OTGINT;
    USB_OTG_FS->GINTMSK &= ~USB_OTG_GINTMSK_MMISM;

    // confirm device mode
    USB_OTG_FS->GINTSTS &= ~USB_OTG_GINTSTS_CMOD;
    while (USB_OTG_FS->GINTSTS & USB_OTG_GINTSTS_CMOD) ;

}

static void USB_Device_Init(void) {
    /* The application must perform the following steps to initialize the core as a device on power- up or after
     *  a mode change from host to device.
     * 1. Program the following fields in the OTG_DCFG register:
     *      – Device speed
     *      – Non-zero-length status OUT handshake
     * 2. Program the OTG_GINTMSK register to unmask the following interrupts:
     *      – USB reset
     *      – Enumeration done
     *      – Early suspend
     *      – USB suspend
     *      – SOF
     * 3. Program the VBUSBSEN bit in the OTG_GCCFG register to enable VBUS sensing in “B” device mode and
     *  supply the 5 volts across the pull-up resistor on the DP line.
     * 4. Wait for the USBRST interrupt in OTG_GINTSTS. It indicates that a reset has been detected on the USB
     *  that lasts for about 10 ms on receiving this interrupt.
     *
     * Wait for the ENUMDNE interrupt in OTG_GINTSTS. This interrupt indicates the end of reset on the USB.
     * On receiving this interrupt, the application must read the OTG_DSTS register to determine the enumeration
     * speed and perform the steps listed in Endpoint initialization on enumeration completion on page 1608.
     * At this point, the device is ready to accept SOF packets and perform control transfers on control endpoint 0.
     */
}