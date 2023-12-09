#ifndef IRQ
#define IRQ

#define IRQ_ENABLE()                                                           \
    do {                                                                       \
        sei();                                                                 \
    } while (0);

#define IRQ_DISABLE()                                                          \
    do {                                                                       \
        cli();                                                                 \
    } while (0);

#endif // IRQ
