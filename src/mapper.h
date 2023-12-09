#ifndef CEDA_KEYBOARD_MAPPER_H
#define CEDA_KEYBOARD_MAPPER_H

#include <stdbool.h>
#include <stdint.h>

#define CEDA_KEY_NONE (0xFF)

/**
 * @brief Parse a PS/2 scan-code sequence into a (key, flags) pair for Sanco.
 *
 * @param data Pointer to sequence buffer.
 * @param len Length of sequence buffer.
 * @param[out] key Pointer to key data.
 * @param[out] flags Pointer to flags data.
 *
 * @return true if the sequence has been correctly parsed, false otherwise.
 */
bool mapper_parse(const uint8_t *data, uint8_t len, uint8_t *key,
                  uint8_t *flags);

#endif // CEDA_KEYBOARD_MAPPER_H
