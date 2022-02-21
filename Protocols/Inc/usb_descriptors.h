/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NUCLEOPONG_USB_DESCRIPTORS_H
#define NUCLEOPONG_USB_DESCRIPTORS_H

#include <stdint.h>

#define USB_DEV_DESC_SIZE                               sizeof(USBDevice_Desc_TypeDef)
#define USB_DEV_DESC_TYPE                               0x06U
#define  USB_DESC_TYPE_DEVICE                           0x01U

#define HID_DESC_TYPE                                   0x21U

#define HID_MOUSE_REPORT_DESC_SIZE                      74U

#define USB_MAX_EP0_SIZE                                64U

// stuff I can't use my own of
#define USBDEV_VID                                      1155 // STMicroelectronics
#define USBDEV_PID_FS                                   22315

#define USBDEV_IDX_MFC_STR                              0x01U
#define USBDEV_IDX_PRODUCT_STR                          0x02U
#define USBDEV_IDX_SERIAL_STR                           0x03U
#define USBDEV_MAX_NUM_CONFIGURATION                    1U



// see page 66 of HID 1.11 for documentation on each field
typedef struct __attribute__((aligned(4))) {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t bcdUSB;
    uint8_t bDeviceClass;
    uint8_t bDeviceSubclass;
    uint8_t bDeviceProtocol;
    uint8_t bMaxPacketSize;
    uint16_t idVendor;
    uint16_t idProduct;
    uint16_t bcdDevice;
    uint8_t iManufacturer;
    uint8_t iProduct;
    int8_t iSerialNumber;
    uint8_t bNumConfigurations;
} USBDevice_Desc_TypeDef;

// see page 67 of HID 1.11
typedef struct __attribute__((aligned(4))) {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t wTotalLength;
    uint8_t bNumInterfaces;
    uint8_t bConfigurationValue;
    uint8_t iConfiguration;
    uint8_t bmAttributes;
    int8_t MaxPower;
} USBConfig_Desc_TypeDef;

// HID 1.11 page 70
typedef struct __attribute__((aligned(4))) {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bInterfaceNumber;
    uint8_t bAlternateSetting;
    uint8_t bNumEndpoints;
    uint8_t bInterfaceClass;
    uint8_t bInterfaceSubClass;
    uint8_t bInterfaceProtocol;
    uint8_t iInterface;
} HIDMouse_InterfaceDesc_TypeDef;

// HID 1.11 page 70
typedef struct __attribute__((aligned(4))) {
    uint8_t bLength;
    uint8_t bHIDDescriptorType;
    uint16_t bcdHID;
    uint8_t CountryCode;
    uint8_t bNumDescriptors;
    uint8_t bReportDescriptorType;
    uint16_t wItemLength;
} HIDMouse_HIDDesc_TypeDef;

// HID 1.11 page 70
typedef struct __attribute__((aligned(4))) {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bEndpointAddress;
    uint8_t bmAttributes;
    uint16_t wMaxPacketSize;
    uint8_t bInterval;
} HIDMouse_EndpointDesc_TypeDef;

// HID Device config descriptor hierarchy
typedef struct __attribute__((aligned(4))) {
    USBConfig_Desc_TypeDef CfgDesc;
    HIDMouse_InterfaceDesc_TypeDef InterfaceDesc;
    HIDMouse_HIDDesc_TypeDef HIDDesc;
    HIDMouse_EndpointDesc_TypeDef EndpointDesc;
} HIDMouse_ConfigDesc_TypeDef;


// I was going to type all of these fields into a struct
// but then I didn't want to. So. I'm just gonna paste STM's in the source
//typedef struct __attribute__((aligned(4))) {
//
//} HIDMouse_ReportDesc_TypeDef;

#ifdef __cplusplus
}
#endif

#endif //NUCLEOPONG_USB_DESCRIPTORS_H
