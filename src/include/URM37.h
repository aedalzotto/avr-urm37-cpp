#ifndef _URM37_H_
#define _URM37_H_

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>

extern "C" void TIMER1_CAPT_vect(void) __attribute__((signal));
extern "C" void TIMER1_OVF_vect(void) __attribute__((signal));

class URM37 {
public:
    static void init();
    static void trigger();
    static int16_t get_distance();

    friend void TIMER1_CAPT_vect(void);
    friend void TIMER1_OVF_vect(void);

private:
    static volatile int16_t distance;
    static volatile uint16_t then;
    static volatile bool edge;

    static const uint8_t divider;

};

#endif /* _URM37_H_ */