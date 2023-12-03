#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>

#define CEDA_KEYBOARD_BAUDRATE (1200)
#define CEDA_KEYBOARD_BRR      ((CPU_FREQ / 16 / CEDA_KEYBOARD_BAUDRATE) - 1)

static inline bool ps2_clock(void) {
    return PINC & (1 << PORTC1);
}

static inline bool ps2_data(void) {
    return PINC & (1 << PORTC0);
}

static inline void ps2_waitFallingClock(void) {
    while (ps2_clock() == 1)
        ;
}

static inline void ps2_waitRisingClock(void) {
    while (ps2_clock() == 0)
        ;
}

static inline bool ps2_readBit(void) {
    ps2_waitFallingClock();
    const bool bit = ps2_data();
    ps2_waitRisingClock();
    return bit;
}

static void ps2_init(void) {
    // setup PS/2 serial GPIO
    DDRC = (0 << PORTC0) | (0 << PORTC1); // set PORTC[0-1] as input
    PORTC = (1 << PORTC0) |
            (1 << PORTC1); // enable pull-up resistors for PORTC[0-1]
}

static bool ps2_read(uint8_t *data) {
    // wait for start bit
    for (bool start_bit = true; start_bit == true;)
        start_bit = ps2_readBit();

    // read payload
    *data = 0;
    for (uint8_t i = 0; i < 8; ++i) {
        const uint8_t bit = !!ps2_readBit();
        *data = (*data >> 1) | (bit << 7);
    }

    // read parity and stop bit
    const bool parity = ps2_readBit();
    const bool stop_bit = ps2_readBit();

    // TODO(giomba): fix parity and frame check
#if 0
        // check framing and parity errors
        if (stop_bit == 1)
            return false;

        uint8_t ones = !!parity;
        for (uint8_t i = 0; i < 8; ++i) {
            if (*data & (1 << i))
                ++ones;
        }

        return (ones % 2 == 1); // odd parity
#endif

    return true;
}

static void uart_init(void) {
    // setup UART (1200 8N1)
    UBRR0L = (CEDA_KEYBOARD_BRR >> 0) & 0xff;
    UBRR0H = (CEDA_KEYBOARD_BRR >> 8) & 0xff;
    UCSR0B = (1 << TXEN0); // enable TX
}

static void uart_putc(char c) {
    // wait for empty TX buffer
    while (!(UCSR0A & (1 << UDRE0)))
        ;

    UDR0 = c;
}

int main(void) {
    ps2_init();
    uart_init();

    for (;;) {
        uint8_t data;
        if (ps2_read(&data)) {
            uart_putc(data);
        }
    }
}
