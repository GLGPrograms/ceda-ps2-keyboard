#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stddef.h>

#include "compiler.h"
#include "mapper.h"
#include "ps2.h"
#include "timer.h"
#include "uart.h"

int main(void) {
    uart_init();
    timer_init();
    ps2_init();

    // enable interrupts
    sei();

    for (;;) {
        // read a scancode sequence from PS/2
        uint8_t sequence[8];
        const int len = ps2_readSequence(sequence, countof(sequence));
        if (len == 0)
            continue;

        // parse sequence into (key, flags) for Sanco
        uint8_t key, flags;
        const bool parsed = mapper_parse(sequence, len, &key, &flags);
        if (!parsed)
            continue;

        // send (key, flags) to Sanco
        uart_putc(key);
        uart_putc(flags);

// send scan codes on serial port for debugging purposes
#if 0
        // send length
        uart_putc(len);

        // send scan code
        for (int i = 0; i < len; ++i) {
            uart_putc(sequence[i]);
        }
#endif
    }
}
