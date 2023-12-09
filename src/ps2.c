#include "ps2.h"

#include "timer.h"

#define PS2_CLOCK_WAIT_TIMEOUT (5000) // [us]

bool ps2_clock(void) {
    return PINC & (1 << PORTC1);
}

bool ps2_data(void) {
    return PINC & (1 << PORTC0);
}

bool ps2_waitClock(enum EdgeMode mode) {
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

bool ps2_readBit(bool *bit) {
    // wait for falling clock, but return false in case of timeout
    if (!ps2_waitClock(EDGE_FALLING))
        return false;

    *bit = ps2_data();
    if (!ps2_waitClock(EDGE_RISING))
        return false;

    return true;
}

void ps2_init(void) {
    // setup PS/2 serial GPIO
    DDRC = (0 << PORTC0) | (0 << PORTC1); // set PORTC[0-1] as input
    PORTC = (1 << PORTC0) |
            (1 << PORTC1); // enable pull-up resistors for PORTC[0-1]
}

bool ps2_read(uint8_t *data) {
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

int ps2_readSequence(uint8_t *sequence, size_t len) {
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