#ifndef CEDA_KEYBOARD_TIMER_H
#define CEDA_KEYBOARD_TIMER_H

#include <stdint.h>

#define TIMER_TICK_TO_US(tick) (tick * 100)
#define TIMER_US_TO_TICK(us)   (us / 100)

/**
 * @brief Initialize the timer module.
 *
 */
void timer_init(void);

/**
 * @brief Get current time.
 *
 * @return Current time in ticks.
 */
uint32_t timer_clock(void);

#endif // CEDA_KEYBOARD_TIMER_H
