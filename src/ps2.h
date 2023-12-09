#ifndef CEDA_KEYBOARD_PS2_H
#define CEDA_KEYBOARD_PS2_H

#include <avr/io.h>
#include <stdbool.h>
#include <stddef.h>

bool ps2_clock(void);

bool ps2_data(void);

enum EdgeMode {
    EDGE_RISING = 0,
    EDGE_FALLING = 1,
};

bool ps2_waitClock(enum EdgeMode mode);

bool ps2_readBit(bool *bit);

void ps2_init(void);

bool ps2_read(uint8_t *data);

int ps2_readSequence(uint8_t *sequence, size_t len);

#endif // CEDA_KEYBOARD_PS2_H
