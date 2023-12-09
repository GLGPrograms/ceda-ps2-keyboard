#ifndef CEDA_KEYBOARD_MAPPER_H
#define CEDA_KEYBOARD_MAPPER_H

#include <stdbool.h>
#include <stdint.h>

#define CEDA_KEY_NONE (0xFF)

bool mapper_parse(const uint8_t *data, uint8_t len, uint8_t *key,
                  uint8_t *flags);

#endif // CEDA_KEYBOARD_MAPPER_H
