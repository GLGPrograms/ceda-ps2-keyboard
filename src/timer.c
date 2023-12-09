#include "timer.h"

#include "irq.h"

#include <avr/interrupt.h>

static volatile uint32_t ticks = 0;

ISR(TIMER1_COMPA_vect) {
    ++ticks;

    PINB |= (1 << PORTB0);

    TIMSK1 |= 0x2; // enable timer
}

void timer_init(void) {
    TCCR1A = 0x00;   // don't connect output pins to timer, CTC[1:0] mode
    TCCR1B = 0x0A;   // CTC[3:2] mode, prescaler = 8
    TCNT1 = 0x0;     // start counting from 0
    OCR1A = 100 - 1; // 100 us

    TIMSK1 |= 0x2; // enable timer

    // TEST
    DDRB |= (1 << PORTB0);
}

uint32_t timer_clock(void) {
    uint32_t now = 0;
    IRQ_DISABLE();
    now = ticks;
    IRQ_ENABLE();
    return now;
}
