/* usart_io.c
 *
 * This file provides implementations of low-level, weakly-linked I/O functions for the STM32 nucleo MCU
 * in order to enable stdio.h library functions to operate over the USART protocol
 *
 * Must be linked with syscalls.c provided by STMicroelectronics
 *
 * Copyright (c) 2022 Ryan Wenger.
 * This file is provided AS-IS with no warranty.
 */

#include "usart.h"

/* We can redefine the lowest-level I/O functions, __io_getchar() and __io_putchar(), which are called
 * by the actual read/write system calls, and are simply left as unimplemented extern functions. Additionally, the
 * read/write syscalls themselves are weak-linked by glibc, meaning their default implementations can be overriden.
 */

int __io_putchar(int ch) {
    USART_Write(USART2, (char *) &ch, 1);
    return 0;
}

int __io_getchar(void ) {
    char rx_data = USART_Read(USART2);
    return rx_data;
}

/* Default implementation at syscalls.c:65
 *
 * The problem with the original is that scanf actually passes 1024 as the len parameter (even when the format string
 * is set to read a 1-width char), which causes the UART to try to read 1024 bytes rather than 1. Obviously
 * this means that the user has to actually send all 1024 bytes, which doesn't happen.
 *
 * The fix is to interpret a newline character as the end of user input, and simply fill the remaining buffer with
 * junk so we don't have to wait on the USART to receive the remaining bytes.
 */
int _read(int file, char *ptr, int len) {
    volatile char *ptr_initial = ptr;
    int DataIdx;

    for (DataIdx = 0; DataIdx < len; DataIdx++) {
        *ptr = __io_getchar();
        if (*ptr == '\n' || *ptr == '\r') {
            ++ptr;
            break;
        }
        ++ptr;
    }

    // fill the rest of the buffer with junk characters so we don't need to wait on USART receiver
    while (++DataIdx < len)
        *ptr++ = '\0';

    return len;
}