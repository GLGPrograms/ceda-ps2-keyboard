#ifndef CEDA_KEYBOARD_PS2_H
#define CEDA_KEYBOARD_PS2_H

#include <avr/io.h>
#include <stdbool.h>
#include <stddef.h>

enum EdgeMode {
    EDGE_RISING = 0,
    EDGE_FALLING = 1,
};

/**
 * @brief Initialize the PS/2 module.
 *
 */
void ps2_init(void);

/**
 * @brief Read PS/2 clock line status.
 *
 * @return true if high, false if low.
 */
bool ps2_clock(void);

/**
 * @brief Read PS/2 data line status.
 *
 * @return true if high, false if low.
 */
bool ps2_data(void);

/**
 * @brief Wait for a rising or falling clock edge.
 *
 * @param mode Mode of edge detection (rising or falling)
 *
 * @return true if detected, false if timed out.
 */
bool ps2_waitClock(enum EdgeMode mode);

/**
 * @brief Sample a bit from the PS/2 serial.
 *
 * @param[out] bit Value of sampled bit.
 * @return true if correctly samples, false if timed out.
 */
bool ps2_readBit(bool *bit);

/**
 * @brief Read a byte from the PS/2 serial line.
 *
 * @param[out] data Pointer to data location where to store the read byte.
 *
 * @return true if a byte has been read, false if timed out.
 */
bool ps2_read(uint8_t *data);

/**
 * @brief Read a PS/2 scan-code sequence.
 *
 * @param[out] sequence Pointer to buffer where to store the read sequence.
 * @param len Length of sequence buffer.
 *
 * @return Number of valid bytes in the output buffer.
 */
int ps2_readSequence(uint8_t *sequence, size_t len);

#endif // CEDA_KEYBOARD_PS2_H
