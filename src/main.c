#include <avr/interrupt.h>
#include <avr/io.h>

#define CEDA_KEYBOARD_BAUDRATE (1200)
#define CEDA_KEYBOARD_BRR      ((CPU_FREQ / 16 / CEDA_KEYBOARD_BAUDRATE) - 1)

int main(void) {
    // set baud rate
    UBRR0L = (CEDA_KEYBOARD_BRR >> 0) & 0xff;
    UBRR0H = (CEDA_KEYBOARD_BRR >> 8) & 0xff;

    // enable UART transmission
    UCSR0B = (1 << TXEN0);

    // send data
    for (uint8_t c = 'A';; ++c) {
        while (!(UCSR0A & (1 << UDRE0)))
            ;

        UDR0 = c;
        if (c == 'Z')
            c = 'A';
    }
}
