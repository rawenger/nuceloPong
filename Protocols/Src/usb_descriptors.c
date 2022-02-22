/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#include "usb_descriptors.h"

USBDevice_Desc_TypeDef Device_Desc =
        {
                0x12,
                USB_DESC_TYPE_DEVICE,
                0x0002,
                0x00,
                0x00,
                0x00,
                USB_MAX_EP0_SIZE,
                USBDEV_VID,
                USBDEV_PID_FS,
                0x0002,
                USBDEV_IDX_MFC_STR,
                USBDEV_IDX_PRODUCT_STR,
                USBDEV_IDX_SERIAL_STR,
                USBDEV_MAX_NUM_CONFIGURATION
        };

HIDMouse_DeviceQualifierDesc_TypeDef MouseHID_DevQualifier_Desc =
        {
                sizeof(HIDMouse_DeviceQualifierDesc_TypeDef),
                USB_DESC_TYPE_DEVICE_QUALIFIER,
                0x0002,
                0x00,
                0x00,
                0x00,
                0x40,
                0x01,
                0x00,
        };

HIDMouse_HIDDesc_TypeDef MouseHID_HIDDesc =
        {
                0x09,
                HID_DESC_TYPE,
                0x1101,
                0x00,
                0x01,
                0x22,
                HID_MOUSE_REPORT_DESC_SIZE,
        };


static uint8_t MouseHID_ReportDesc[HID_MOUSE_REPORT_DESC_SIZE]  __attribute__((aligned(4))) =
        {
                0x05,   0x01,
                0x09,   0x02,
                0xA1,   0x01,
                0x09,   0x01,

                0xA1,   0x00,
                0x05,   0x09,
                0x19,   0x01,
                0x29,   0x03,

                0x15,   0x00,
                0x25,   0x01,
                0x95,   0x03,
                0x75,   0x01,

                0x81,   0x02,
                0x95,   0x01,
                0x75,   0x05,
                0x81,   0x01,

                0x05,   0x01,
                0x09,   0x30,
                0x09,   0x31,
                0x09,   0x38,

                0x15,   0x81,
                0x25,   0x7F,
                0x75,   0x08,
                0x95,   0x03,

                0x81,   0x06,
                0xC0,   0x09,
                0x3c,   0x05,
                0xff,   0x09,

                0x01,   0x15,
                0x00,   0x25,
                0x01,   0x75,
                0x01,   0x95,

                0x02,   0xb1,
                0x22,   0x75,
                0x06,   0x95,
                0x01,   0xb1,

                0x01,   0xc0
        };