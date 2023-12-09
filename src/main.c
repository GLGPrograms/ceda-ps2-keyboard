#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stddef.h>

#include "compiler.h"
#include "timer.h"

#define CEDA_KEYBOARD_BAUDRATE (1200)
#define CEDA_KEYBOARD_BRR      ((CPU_FREQ / 16 / CEDA_KEYBOARD_BAUDRATE) - 1)

static inline bool ps2_clock(void) {
    return PINC & (1 << PORTC1);
}

static inline bool ps2_data(void) {
    return PINC & (1 << PORTC0);
}

enum EdgeMode {
    EDGE_RISING = 0,
    EDGE_FALLING = 1,
};

#define PS2_CLOCK_WAIT_TIMEOUT (1000) // [us]
static inline bool ps2_waitClock(enum EdgeMode mode) {
    const uint32_t start = timer_clock();
    bool timeout = false;

    while (ps2_clock() == mode) {
        const uint32_t now = timer_clock();
        if (now >= start + TIMER_US_TO_TICK(PS2_CLOCK_WAIT_TIMEOUT)) {
            timeout = true;
            break;
        }
    }

    return !timeout;
}

static inline bool ps2_readBit(bool *bit) {
    // wait for falling clock, but return false in case of timeout
    if (!ps2_waitClock(EDGE_FALLING))
        return false;

    *bit = ps2_data();
    if (!ps2_waitClock(EDGE_RISING))
        return false;

    return true;
}

static void ps2_init(void) {
    // setup PS/2 serial GPIO
    DDRC = (0 << PORTC0) | (0 << PORTC1); // set PORTC[0-1] as input
    PORTC = (1 << PORTC0) |
            (1 << PORTC1); // enable pull-up resistors for PORTC[0-1]
}

static bool ps2_read(uint8_t *data) {
    // wait for start bit
    for (bool start_bit = true; start_bit == true;) {
        const bool valid = ps2_readBit(&start_bit);
        if (!valid)
            return false;
    }

    // read payload
    *data = 0;
    for (uint8_t i = 0; i < 8; ++i) {
        bool bit;
        const bool valid = ps2_readBit(&bit);
        if (!valid)
            return false;
        bit = !!bit; // make sure bit is either 0 or 1
        *data = (*data >> 1) | (bit << 7);
    }

    // read parity and stop bit
    bool parity_bit, stop_bit, valid;

    valid = ps2_readBit(&parity_bit);
    if (!valid)
        return false;

    valid = ps2_readBit(&stop_bit);
    if (!valid)
        return false;

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

static int ps2_readSequence(uint8_t *sequence, size_t len) {
    size_t i = 0;
    for (; i < len; ++i) {
        const bool valid = ps2_read(&sequence[i]);
        if (!valid)
            break;
    }
    return i;
}

#if 0
{
    do
        (!ps2_read(&sequence[0]));

    // not an escape sequence
    if (sequence[0] != 0xE0 && sequence[1] != 0xF0)
        return 1;

    while (!ps2_read(&sequence[1]))
        ;

    // it was just a keyup, ignore it
    if (sequence[0] == 0xF0)
        return -1;

    // don't know what that was, let's try to ignore it
    if (sequence[0] != 0xE0)
        return -1;

    // remember that this is just a keyup of an escaped key,
    // so that we don't report it in the end,
    // but we still consume the input buffer
    const bool escaped_keyup = sequence[1] == 0xF0;

    if (escaped_keyup)
        while (!ps2_read(&sequence[1]))
            ;

    const uint8_t ones[] = {
        0x11, // RIGHT ALT
        0x14, // RIGHT CTRL
        0x1F, // LEFT SUPER
        0x27, // RIGHT SUPER
        0x2F, // MENU
        0x4A, // NUM /
        0x5A, // NUM ENTER
        0x69, // END
        0x6B, // CURSOR LEFT
        0x6C, // BEGIN
        0x72, // CURSOR DOWN
        0x74, // CURSOR RIGHT
        0x75, // CURSOR UP
        0x70, // INS
        0x71, // DELETE
        0x7A, // PAGE DOWN
        0x7D, // PAGE UP
    };
#if 0
            // sequence starting with: 0xE0
      switch (sequence[1]){

      } }
#endif
}
#endif

static void uart_init(void) { // setup UART (1200 8N1)
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

#define KEY_SP (0x39)
static const uint8_t KEYTAB[] = {
    KEY_SP, // 00 (---)
    0x56,   // 01 (F9)
    KEY_SP, // 02 (---)
    0x52,   // 03 (F5)
    0x50,   // 04 (F3)
    0x4E,   // 05 (F1)
    0x4F,   // 06 (F2)
    0x59,   // 07 (F12)
    KEY_SP, // 08 (---)
    0x57,   // 09 (F10)
    0x55,   // 0A (F8)
    0x53,   // 0B (F6)
    0x51,   // 0C (F4)
    0x12,   // 0D (TAB)
    KEY_SP, // 0E (---)
    KEY_SP, // 0F (---)
    KEY_SP, // 10 (---)
    KEY_SP, // 11 (---)
    KEY_SP, // 12 (---)
    KEY_SP, // 13 (---)
    0x61,   // 14 (LEFT CTRL)
    0x20,   // 15 ('Q')
    0x03,   // 16 ('1')
    KEY_SP, // 17 (---)
    KEY_SP, // 18 (---)
    KEY_SP, // 19 (---)
    0x14,   // 1A ('Z')
    0x21,   // 1B ('S')
    0x13,   // 1C ('A')
    0x2E,   // 1D ('W')
    0x04,   // 1E ('2')
    KEY_SP, // 1F (---)
    KEY_SP, // 20 (---)
    0x30,   // 21 ('C')
    0x2F,   // 22 ('X')
    0x22,   // 23 ('D')
    0x15,   // 24 ('E')
    0x06,   // 25 ('4')
    0x05,   // 26 ('3')
    KEY_SP, // 27 (---)
    KEY_SP, // 28 (---)
    0x39,   // 29 (SPACE)
    0x31,   // 2A ('V')
    0x23,   // 2B ('F')
    0x17,   // 2C ('T')
    0x16,   // 2D ('R')
    0x07,   // 2E ('5')
    KEY_SP, // 2F (---)
    KEY_SP, // 30 (---)
    0x33,   // 31 ('N')
    0x32,   // 32 ('B')
    0x25,   // 33 ('H')
    0x24,   // 34 ('G')
    0x18,   // 35 ('Y')
    0x08,   // 36 ('6')
    KEY_SP, // 37 (---)
    KEY_SP, // 38 (---)
    KEY_SP, // 39 (---)
    0x29,   // 3A ('M')
    0x26,   // 3B ('J')
    0x19,   // 3C ('U')
    0x09,   // 3D ('7')
    0x0A,   // 3E ('8')
    KEY_SP, // 3F (---)
    KEY_SP, // 40 (---)
    KEY_SP, // 41 (---)
    0x27,   // 42 ('K')
    0x1A,   // 43 ('I')
    0x1B,   // 44 ('O')
    0x0C,   // 45 ('0')
    0x0B,   // 46 ('9')
    KEY_SP, // 47 (---)
    KEY_SP, // 48 (---)
    KEY_SP, // 49 (---)
    KEY_SP, // 4A (---)
    0x28,   // 4B ('L')
    KEY_SP, // 4C (---)
    0x1C,   // 4D ('P')
    KEY_SP, // 4E (---)
    KEY_SP, // 4F (---)
    KEY_SP, // 50 (---)
    KEY_SP, // 51 (---)
    KEY_SP, // 52 (---)
    KEY_SP, // 53 (---)
    KEY_SP, // 54 (---)
    KEY_SP, // 55 (---)
    KEY_SP, // 56 (---)
    KEY_SP, // 57 (---)
    KEY_SP, // 58 (---)
    KEY_SP, // 59 (---)
    0x2B,   // 5A (ENTER)
    KEY_SP, // 5B (---)
    KEY_SP, // 5C (---)
    KEY_SP, // 5D (---)
    KEY_SP, // 5E (---)
    KEY_SP, // 5F (---)
    KEY_SP, // 60 (---)
    KEY_SP, // 61 (---)
    KEY_SP, // 62 (---)
    KEY_SP, // 63 (---)
    KEY_SP, // 64 (---)
    KEY_SP, // 65 (---)
    0x0F,   // 66 (BACKSPACE)
    KEY_SP, // 67 (---)
    KEY_SP, // 68 (---)
    KEY_SP, // 69 (---)
    KEY_SP, // 6A (---)
    KEY_SP, // 6B (---)
    KEY_SP, // 6C (---)
    KEY_SP, // 6D (---)
    KEY_SP, // 6E (---)
    KEY_SP, // 6F (---)
    KEY_SP, // 70 (---)
    KEY_SP, // 71 (---)
    KEY_SP, // 72 (---)
    KEY_SP, // 73 (---)
    KEY_SP, // 74 (---)
    KEY_SP, // 75 (---)
    0x01,   // 76 (ESC)
    KEY_SP, // 77 (---)
    0x58,   // 78 (F11)
    KEY_SP, // 79 (---)
    KEY_SP, // 7A (---)
    KEY_SP, // 7B (---)
    KEY_SP, // 7C (---)
    KEY_SP, // 7D (---)
    KEY_SP, // 7E (---)
    KEY_SP, // 7F (---)
    KEY_SP, // 80 (---)
    KEY_SP, // 81 (---)
    KEY_SP, // 82 (---)
    0x54,   // 83 (F7)
    KEY_SP, // 84 (---)
    KEY_SP, // 85 (---)
    KEY_SP, // 86 (---)
    KEY_SP, // 87 (---)
    KEY_SP, // 88 (---)
    KEY_SP, // 89 (---)
    KEY_SP, // 8A (---)
    KEY_SP, // 8B (---)
    KEY_SP, // 8C (---)
    KEY_SP, // 8D (---)
    KEY_SP, // 8E (---)
    KEY_SP, // 8F (---)
    KEY_SP, // 90 (---)
    KEY_SP, // 91 (---)
    KEY_SP, // 92 (---)
    KEY_SP, // 93 (---)
    KEY_SP, // 94 (---)
    KEY_SP, // 95 (---)
    KEY_SP, // 96 (---)
    KEY_SP, // 97 (---)
    KEY_SP, // 98 (---)
    KEY_SP, // 99 (---)
    KEY_SP, // 9A (---)
    KEY_SP, // 9B (---)
    KEY_SP, // 9C (---)
    KEY_SP, // 9D (---)
    KEY_SP, // 9E (---)
    KEY_SP, // 9F (---)
    KEY_SP, // A0 (---)
    KEY_SP, // A1 (---)
    KEY_SP, // A2 (---)
    KEY_SP, // A3 (---)
    KEY_SP, // A4 (---)
    KEY_SP, // A5 (---)
    KEY_SP, // A6 (---)
    KEY_SP, // A7 (---)
    KEY_SP, // A8 (---)
    KEY_SP, // A9 (---)
    KEY_SP, // AA (---)
    KEY_SP, // AB (---)
    KEY_SP, // AC (---)
    KEY_SP, // AD (---)
    KEY_SP, // AE (---)
    KEY_SP, // AF (---)
    KEY_SP, // B0 (---)
    KEY_SP, // B1 (---)
    KEY_SP, // B2 (---)
    KEY_SP, // B3 (---)
    KEY_SP, // B4 (---)
    KEY_SP, // B5 (---)
    KEY_SP, // B6 (---)
    KEY_SP, // B7 (---)
    KEY_SP, // B8 (---)
    KEY_SP, // B9 (---)
    KEY_SP, // BA (---)
    KEY_SP, // BB (---)
    KEY_SP, // BC (---)
    KEY_SP, // BD (---)
    KEY_SP, // BE (---)
    KEY_SP, // BF (---)
    KEY_SP, // C0 (---)
    KEY_SP, // C1 (---)
    KEY_SP, // C2 (---)
    KEY_SP, // C3 (---)
    KEY_SP, // C4 (---)
    KEY_SP, // C5 (---)
    KEY_SP, // C6 (---)
    KEY_SP, // C7 (---)
    KEY_SP, // C8 (---)
    KEY_SP, // C9 (---)
    KEY_SP, // CA (---)
    KEY_SP, // CB (---)
    KEY_SP, // CC (---)
    KEY_SP, // CD (---)
    KEY_SP, // CE (---)
    KEY_SP, // CF (---)
    KEY_SP, // D0 (---)
    KEY_SP, // D1 (---)
    KEY_SP, // D2 (---)
    KEY_SP, // D3 (---)
    KEY_SP, // D4 (---)
    KEY_SP, // D5 (---)
    KEY_SP, // D6 (---)
    KEY_SP, // D7 (---)
    KEY_SP, // D8 (---)
    KEY_SP, // D9 (---)
    KEY_SP, // DA (---)
    KEY_SP, // DB (---)
    KEY_SP, // DC (---)
    KEY_SP, // DD (---)
    KEY_SP, // DE (---)
    KEY_SP, // DF (---)
    KEY_SP, // E0 (---)
    KEY_SP, // E1 (---)
    KEY_SP, // E2 (---)
    KEY_SP, // E3 (---)
    KEY_SP, // E4 (---)
    KEY_SP, // E5 (---)
    KEY_SP, // E6 (---)
    KEY_SP, // E7 (---)
    KEY_SP, // E8 (---)
    KEY_SP, // E9 (---)
    KEY_SP, // EA (---)
    KEY_SP, // EB (---)
    KEY_SP, // EC (---)
    KEY_SP, // ED (---)
    KEY_SP, // EE (---)
    KEY_SP, // EF (---)
    KEY_SP, // F0 (---)
    KEY_SP, // F1 (---)
    KEY_SP, // F2 (---)
    KEY_SP, // F3 (---)
    KEY_SP, // F4 (---)
    KEY_SP, // F5 (---)
    KEY_SP, // F6 (---)
    KEY_SP, // F7 (---)
    KEY_SP, // F8 (---)
    KEY_SP, // F9 (---)
    KEY_SP, // FA (---)
    KEY_SP, // FB (---)
    KEY_SP, // FC (---)
    KEY_SP, // FD (---)
    KEY_SP, // FE (---)
    KEY_SP, // FF (---)
};

static bool mapper(uint8_t data, uint8_t *key, uint8_t *flags) {
    static uint8_t modifier = 0;

    // ignore keyup events
    if (data == 0xF0) {
        ps2_read(&data);
        return false;
    }

    *flags = 0xc0; // <- no flags
    *key = KEYTAB[data];

    return true;
}

int main(void) {
    ps2_init();
    uart_init();
    timer_init();

    // enable interrupts
    sei();

    for (;;) {
        uint8_t sequence[8];
        const int len = ps2_readSequence(sequence, countof(sequence));
        if (len == 0)
            continue;

        for (int i = 0; i < len; ++i) {
            uart_putc(sequence[i]);
        }
    }

#if 0
    for (;;) {
        uint8_t sequence[8];
        const int len = ps2_readSequence(&sequence);
        if (len == -1)
            continue;

        ps2_parseSequence(sequence);

        uint8_t key, flags;
        if (!mapper(data, &key, &flags))
            continue;

        uart_putc(key);
        uart_putc(flags);
    }
#endif
}
