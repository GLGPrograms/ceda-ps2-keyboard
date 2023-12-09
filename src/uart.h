#ifndef CEDA_KEYBOARD_UART_H
#define CEDA_KEYBOARD_UART_H

#include <avr/io.h>

/**
 * @brief Initialize the UART module.
 *
 */
void uart_init(void);

/**
 * @brief Send a char on the UART.
 *
 * @param c The char to send.
 */
void uart_putc(char c);

#endif // CEDA_KEYBOARD_UART_H
