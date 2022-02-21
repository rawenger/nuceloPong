/*
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#include "usb_descriptors.h"

USBDevice_Desc_TypeDef Device_Desc =
        {
                0x12,                       /*bLength */
                USB_DESC_TYPE_DEVICE,       /*bDescriptorType*/
                0x0002,                       /*bcdUSB */
                0x00,                       /*bDeviceClass*/
                0x00,                       /*bDeviceSubClass*/
                0x00,                       /*bDeviceProtocol*/
                USB_MAX_EP0_SIZE,           /*bMaxPacketSize*/
                USBDEV_VID,                   /*idVendor*/
                USBDEV_PID_FS,        /*idProduct*/
                0x0002,                       /*bcdDevice rel. 2.00*/
                USBDEV_IDX_MFC_STR,           /*Index of manufacturer  string*/
                USBDEV_IDX_PRODUCT_STR,       /*Index of product string*/
                USBDEV_IDX_SERIAL_STR,        /*Index of serial number string*/
                USBDEV_MAX_NUM_CONFIGURATION  /*bNumConfigurations*/
        };

HIDMouse_HIDDesc_TypeDef MouseHID_Desc =
        {
                0x09,                                               /* bLength: HID Descriptor size */
                HID_DESC_TYPE,                                /* bDescriptorType: HID */
                0x1101,                                               /* bcdHID: HID Class Spec release number */
                0x00,                                               /* bCountryCode: Hardware target country */
                0x01,                                               /* bNumDescriptors: Number of HID class descriptors to follow */
                0x22,                                               /* bDescriptorType */
                HID_MOUSE_REPORT_DESC_SIZE,                         /* wItemLength: Total length of Report descriptor */
        };


static uint8_t HID_MOUSE_ReportDesc[HID_MOUSE_REPORT_DESC_SIZE]  __attribute__((aligned(4))) =
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