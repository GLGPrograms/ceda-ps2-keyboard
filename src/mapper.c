#include "mapper.h"

#include "compiler.h"

static const uint8_t KEYTAB[] = {
    CEDA_KEY_NONE, // 00 (---)
    0x56,          // 01 (F9)
    CEDA_KEY_NONE, // 02 (---)
    0x52,          // 03 (F5)
    0x50,          // 04 (F3)
    0x4E,          // 05 (F1)
    0x4F,          // 06 (F2)
    0x59,          // 07 (F12)
    CEDA_KEY_NONE, // 08 (---)
    0x57,          // 09 (F10)
    0x55,          // 0A (F8)
    0x53,          // 0B (F6)
    0x51,          // 0C (F4)
    0x12,          // 0D (TAB)
    CEDA_KEY_NONE, // 0E (---)
    CEDA_KEY_NONE, // 0F (---)
    CEDA_KEY_NONE, // 10 (---)
    CEDA_KEY_NONE, // 11 (---)
    0x63,          // 12 (LEFT SHIFT)
    CEDA_KEY_NONE, // 13 (---)
    0x61,          // 14 (LEFT CTRL)
    0x20,          // 15 ('Q')
    0x03,          // 16 ('1')
    CEDA_KEY_NONE, // 17 (---)
    CEDA_KEY_NONE, // 18 (---)
    CEDA_KEY_NONE, // 19 (---)
    0x14,          // 1A ('Z')
    0x21,          // 1B ('S')
    0x13,          // 1C ('A')
    0x2E,          // 1D ('W')
    0x04,          // 1E ('2')
    CEDA_KEY_NONE, // 1F (---)
    CEDA_KEY_NONE, // 20 (---)
    0x30,          // 21 ('C')
    0x2F,          // 22 ('X')
    0x22,          // 23 ('D')
    0x15,          // 24 ('E')
    0x06,          // 25 ('4')
    0x05,          // 26 ('3')
    CEDA_KEY_NONE, // 27 (---)
    CEDA_KEY_NONE, // 28 (---)
    0x39,          // 29 (SPACE)
    0x31,          // 2A ('V')
    0x23,          // 2B ('F')
    0x17,          // 2C ('T')
    0x16,          // 2D ('R')
    0x07,          // 2E ('5')
    CEDA_KEY_NONE, // 2F (---)
    CEDA_KEY_NONE, // 30 (---)
    0x33,          // 31 ('N')
    0x32,          // 32 ('B')
    0x25,          // 33 ('H')
    0x24,          // 34 ('G')
    0x18,          // 35 ('Y')
    0x08,          // 36 ('6')
    CEDA_KEY_NONE, // 37 (---)
    CEDA_KEY_NONE, // 38 (---)
    CEDA_KEY_NONE, // 39 (---)
    0x29,          // 3A ('M')
    0x26,          // 3B ('J')
    0x19,          // 3C ('U')
    0x09,          // 3D ('7')
    0x0A,          // 3E ('8')
    CEDA_KEY_NONE, // 3F (---)
    CEDA_KEY_NONE, // 40 (---)
    CEDA_KEY_NONE, // 41 (---)
    0x27,          // 42 ('K')
    0x1A,          // 43 ('I')
    0x1B,          // 44 ('O')
    0x0C,          // 45 ('0')
    0x0B,          // 46 ('9')
    CEDA_KEY_NONE, // 47 (---)
    CEDA_KEY_NONE, // 48 (---)
    CEDA_KEY_NONE, // 49 (---)
    CEDA_KEY_NONE, // 4A (---)
    0x28,          // 4B ('L')
    CEDA_KEY_NONE, // 4C (---)
    0x1C,          // 4D ('P')
    CEDA_KEY_NONE, // 4E (---)
    CEDA_KEY_NONE, // 4F (---)
    CEDA_KEY_NONE, // 50 (---)
    CEDA_KEY_NONE, // 51 (---)
    CEDA_KEY_NONE, // 52 (---)
    CEDA_KEY_NONE, // 53 (---)
    CEDA_KEY_NONE, // 54 (---)
    CEDA_KEY_NONE, // 55 (---)
    CEDA_KEY_NONE, // 56 (---)
    CEDA_KEY_NONE, // 57 (---)
    CEDA_KEY_NONE, // 58 (SHIFT LOCK)
    0x64,          // 59 (RIGHT SHIFT)
    0x2B,          // 5A (ENTER)
    CEDA_KEY_NONE, // 5B (---)
    CEDA_KEY_NONE, // 5C (---)
    CEDA_KEY_NONE, // 5D (---)
    CEDA_KEY_NONE, // 5E (---)
    CEDA_KEY_NONE, // 5F (---)
    CEDA_KEY_NONE, // 60 (---)
    CEDA_KEY_NONE, // 61 (---)
    CEDA_KEY_NONE, // 62 (---)
    CEDA_KEY_NONE, // 63 (---)
    CEDA_KEY_NONE, // 64 (---)
    CEDA_KEY_NONE, // 65 (---)
    0x0F,          // 66 (BACKSPACE)
    CEDA_KEY_NONE, // 67 (---)
    CEDA_KEY_NONE, // 68 (---)
    0x46,          // 69 (NUM '1')
    CEDA_KEY_NONE, // 6A (---)
    0x42,          // 6B (NUM '4')
    0x3E,          // 6C (NUM '7')
    CEDA_KEY_NONE, // 6D (---)
    CEDA_KEY_NONE, // 6E (---)
    CEDA_KEY_NONE, // 6F (---)
    0x4A,          // 70 (NUM '0')
    0x49,          // 71 (NUM '.')
    0x47,          // 72 (NUM '2')
    0x43,          // 73 (NUM '5')
    0x44,          // 74 (NUM '6')
    0x3F,          // 75 (NUM '8')
    0x01,          // 76 (ESC)
    CEDA_KEY_NONE, // 77 (---)
    0x58,          // 78 (F11)
    CEDA_KEY_NONE, // 79 (NUM '+')
    0x48,          // 7A (NUM '3')
    CEDA_KEY_NONE, // 7B (NUM '-')
    CEDA_KEY_NONE, // 7C (NUM '*')
    0x40,          // 7D (NUM '9')
    CEDA_KEY_NONE, // 7E (---)
    CEDA_KEY_NONE, // 7F (---)
    CEDA_KEY_NONE, // 80 (---)
    CEDA_KEY_NONE, // 81 (---)
    CEDA_KEY_NONE, // 82 (---)
    0x54,          // 83 (F7)
    CEDA_KEY_NONE, // 84 (---)
    CEDA_KEY_NONE, // 85 (---)
    CEDA_KEY_NONE, // 86 (---)
    CEDA_KEY_NONE, // 87 (---)
    CEDA_KEY_NONE, // 88 (---)
    CEDA_KEY_NONE, // 89 (---)
    CEDA_KEY_NONE, // 8A (---)
    CEDA_KEY_NONE, // 8B (---)
    CEDA_KEY_NONE, // 8C (---)
    CEDA_KEY_NONE, // 8D (---)
    CEDA_KEY_NONE, // 8E (---)
    CEDA_KEY_NONE, // 8F (---)
    CEDA_KEY_NONE, // 90 (---)
    CEDA_KEY_NONE, // 91 (---)
    CEDA_KEY_NONE, // 92 (---)
    CEDA_KEY_NONE, // 93 (---)
    CEDA_KEY_NONE, // 94 (---)
    CEDA_KEY_NONE, // 95 (---)
    CEDA_KEY_NONE, // 96 (---)
    CEDA_KEY_NONE, // 97 (---)
    CEDA_KEY_NONE, // 98 (---)
    CEDA_KEY_NONE, // 99 (---)
    CEDA_KEY_NONE, // 9A (---)
    CEDA_KEY_NONE, // 9B (---)
    CEDA_KEY_NONE, // 9C (---)
    CEDA_KEY_NONE, // 9D (---)
    CEDA_KEY_NONE, // 9E (---)
    CEDA_KEY_NONE, // 9F (---)
    CEDA_KEY_NONE, // A0 (---)
    CEDA_KEY_NONE, // A1 (---)
    CEDA_KEY_NONE, // A2 (---)
    CEDA_KEY_NONE, // A3 (---)
    CEDA_KEY_NONE, // A4 (---)
    CEDA_KEY_NONE, // A5 (---)
    CEDA_KEY_NONE, // A6 (---)
    CEDA_KEY_NONE, // A7 (---)
    CEDA_KEY_NONE, // A8 (---)
    CEDA_KEY_NONE, // A9 (---)
    CEDA_KEY_NONE, // AA (---)
    CEDA_KEY_NONE, // AB (---)
    CEDA_KEY_NONE, // AC (---)
    CEDA_KEY_NONE, // AD (---)
    CEDA_KEY_NONE, // AE (---)
    CEDA_KEY_NONE, // AF (---)
    CEDA_KEY_NONE, // B0 (---)
    CEDA_KEY_NONE, // B1 (---)
    CEDA_KEY_NONE, // B2 (---)
    CEDA_KEY_NONE, // B3 (---)
    CEDA_KEY_NONE, // B4 (---)
    CEDA_KEY_NONE, // B5 (---)
    CEDA_KEY_NONE, // B6 (---)
    CEDA_KEY_NONE, // B7 (---)
    CEDA_KEY_NONE, // B8 (---)
    CEDA_KEY_NONE, // B9 (---)
    CEDA_KEY_NONE, // BA (---)
    CEDA_KEY_NONE, // BB (---)
    CEDA_KEY_NONE, // BC (---)
    CEDA_KEY_NONE, // BD (---)
    CEDA_KEY_NONE, // BE (---)
    CEDA_KEY_NONE, // BF (---)
    CEDA_KEY_NONE, // C0 (---)
    CEDA_KEY_NONE, // C1 (---)
    CEDA_KEY_NONE, // C2 (---)
    CEDA_KEY_NONE, // C3 (---)
    CEDA_KEY_NONE, // C4 (---)
    CEDA_KEY_NONE, // C5 (---)
    CEDA_KEY_NONE, // C6 (---)
    CEDA_KEY_NONE, // C7 (---)
    CEDA_KEY_NONE, // C8 (---)
    CEDA_KEY_NONE, // C9 (---)
    CEDA_KEY_NONE, // CA (---)
    CEDA_KEY_NONE, // CB (---)
    CEDA_KEY_NONE, // CC (---)
    CEDA_KEY_NONE, // CD (---)
    CEDA_KEY_NONE, // CE (---)
    CEDA_KEY_NONE, // CF (---)
    CEDA_KEY_NONE, // D0 (---)
    CEDA_KEY_NONE, // D1 (---)
    CEDA_KEY_NONE, // D2 (---)
    CEDA_KEY_NONE, // D3 (---)
    CEDA_KEY_NONE, // D4 (---)
    CEDA_KEY_NONE, // D5 (---)
    CEDA_KEY_NONE, // D6 (---)
    CEDA_KEY_NONE, // D7 (---)
    CEDA_KEY_NONE, // D8 (---)
    CEDA_KEY_NONE, // D9 (---)
    CEDA_KEY_NONE, // DA (---)
    CEDA_KEY_NONE, // DB (---)
    CEDA_KEY_NONE, // DC (---)
    CEDA_KEY_NONE, // DD (---)
    CEDA_KEY_NONE, // DE (---)
    CEDA_KEY_NONE, // DF (---)
    CEDA_KEY_NONE, // E0 (---)
    CEDA_KEY_NONE, // E1 (---)
    CEDA_KEY_NONE, // E2 (---)
    CEDA_KEY_NONE, // E3 (---)
    CEDA_KEY_NONE, // E4 (---)
    CEDA_KEY_NONE, // E5 (---)
    CEDA_KEY_NONE, // E6 (---)
    CEDA_KEY_NONE, // E7 (---)
    CEDA_KEY_NONE, // E8 (---)
    CEDA_KEY_NONE, // E9 (---)
    CEDA_KEY_NONE, // EA (---)
    CEDA_KEY_NONE, // EB (---)
    CEDA_KEY_NONE, // EC (---)
    CEDA_KEY_NONE, // ED (---)
    CEDA_KEY_NONE, // EE (---)
    CEDA_KEY_NONE, // EF (---)
    CEDA_KEY_NONE, // F0 (---)
    CEDA_KEY_NONE, // F1 (---)
    CEDA_KEY_NONE, // F2 (---)
    CEDA_KEY_NONE, // F3 (---)
    CEDA_KEY_NONE, // F4 (---)
    CEDA_KEY_NONE, // F5 (---)
    CEDA_KEY_NONE, // F6 (---)
    CEDA_KEY_NONE, // F7 (---)
    CEDA_KEY_NONE, // F8 (---)
    CEDA_KEY_NONE, // F9 (---)
    CEDA_KEY_NONE, // FA (---)
    CEDA_KEY_NONE, // FB (---)
    CEDA_KEY_NONE, // FC (---)
    CEDA_KEY_NONE, // FD (---)
    CEDA_KEY_NONE, // FE (---)
    CEDA_KEY_NONE, // FF (---)
};

struct ps22ceda_associator {
    uint8_t ps2_code;
    uint8_t ceda_code;
};

static const struct ps22ceda_associator ESCAPE_E0[] = {
    {0x11, 0x62},          // (RIGHT ALT)
    {0x14, 0x61},          // (RIGHT CTRL)
    {0x1F, CEDA_KEY_NONE}, // (LEFT SUPER)
    {0x27, CEDA_KEY_NONE}, // (RIGHT SUPER)
    {0x2F, CEDA_KEY_NONE}, // (MENU)
    {0x4A, CEDA_KEY_NONE}, // (NUM /)
    {0x5A, 0x4C},          // (NUM ENTER)
    {0x69, CEDA_KEY_NONE}, // (END)
    {0x6B, 0x3C},          // (CURSOR LEFT)
    {0x6C, CEDA_KEY_NONE}, // (BEGIN)
    {0x72, 0x3B},          // (CURSOR DOWN)
    {0x74, 0x3D},          // (CURSOR RIGHT)
    {0x75, 0x3A},          // (CURSOR UP)
    {0x70, CEDA_KEY_NONE}, // (INS)
    {0x71, 0x0F},          // (DELETE)
    {0x7A, CEDA_KEY_NONE}, // (PAGE DOWN)
    {0x7D, CEDA_KEY_NONE}, // (PAGE UP)
};

#define CEDA_KEY_FLAG_SHIFT     (1 << 0)
#define CEDA_KEY_FLAG_CAPS_LOCK (1 << 1)
#define CEDA_KEY_FLAG_ALT       (1 << 2)
#define CEDA_KEY_FLAG_CTRL      (1 << 3)

bool mapper_parse(const uint8_t *data, uint8_t len, uint8_t *key,
                  uint8_t *oflags) {

    // local status of flags
    // 0xC0 => default status, never changes
    static uint8_t iflags = 0xC0;

    if (len == 1) {
        *key = KEYTAB[data[0]];

        // some keys are "flag keys", so inspect them with a dedicated code path
        switch (data[0]) {
        case 0x12: // (LEFT SHIFT)
        case 0x59: // (RIGHT SHIFT)
            iflags |= CEDA_KEY_FLAG_SHIFT;
            break;
        case 0x58: // (SHIFT LOCK)
            iflags ^= CEDA_KEY_FLAG_CAPS_LOCK;
            break;
        }

        *oflags = iflags;

        return true;
    } else if (len == 2) {
        // extended keys
        if (data[0] == 0xE0) {
            // search association in escaped array
            bool found = false;
            for (uint8_t i = 0; i < countof(ESCAPE_E0); ++i) {
                const struct ps22ceda_associator *const associator =
                    &ESCAPE_E0[i];
                if (data[1] == associator->ps2_code) {
                    found = true;
                    *key = associator->ceda_code;
                    break;
                }
            }

            // some keys are "flag keys", so further inspect specific cases
            switch (data[1]) {
            case 0x11: // (RIGHT ALT)
                iflags |= CEDA_KEY_FLAG_ALT;
                break;
            case 0x14: // (RIGTH CTRL)
                iflags |= CEDA_KEY_FLAG_CTRL;
                break;
            }

            *oflags = iflags;
            return found;
        }

        // keyup (only "flag keys" are interesting)
        if (data[0] == 0xF0) {
            switch (data[1]) {
            case 0x12: // (LEFT SHIFT)
            case 0x59: // (RIGHT SHIFT)
                iflags &= ~CEDA_KEY_FLAG_SHIFT;
                break;
            }
            return false;
        }
    } else if (len == 3) {
        // escape sequence has been released
        if (data[0] == 0xE0 && data[1] == 0xF0) {
            // some keys are "flag keys", so we need to release them
            switch (data[2]) {
            case 0x11: // (RIGHT ALT)
                iflags &= ~CEDA_KEY_FLAG_ALT;
                break;
            case 0x14: // (RIGHT CTRL)
                iflags &= ~CEDA_KEY_FLAG_CTRL;
                break;
            }
            return false;
        }
    }

    return false;
}
