#ifndef CEDA_KEYBOARD_UART_H
#define CEDA_KEYBOARD_UART_H

#include <avr/io.h>

void uart_init(void);

void uart_putc(char c);

#endif // CEDA_KEYBOARD_UART_H
