#include "uart.h"

#include <avr/io.h>

#define CEDA_KEYBOARD_BAUDRATE (1200)
#define CEDA_KEYBOARD_BRR      ((CPU_FREQ / 16 / CEDA_KEYBOARD_BAUDRATE) - 1)

void uart_init(void) { // setup UART (1200 8N1)
    UBRR0L = (CEDA_KEYBOARD_BRR >> 0) & 0xff;
    UBRR0H = (CEDA_KEYBOARD_BRR >> 8) & 0xff;
    UCSR0B = (1 << TXEN0); // enable TX
}

void uart_putc(char c) {
    // wait for empty TX buffer
    while (!(UCSR0A & (1 << UDRE0)))
        ;

    UDR0 = c;
}
