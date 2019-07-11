#ifndef _URM37_H_
#define _URM37_H_

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>

#define TRIGGER_DDR  DDRL
#define TRIGGER_PORT PORTL
#define TRIGGER_PIN  PL1

#ifdef __AVR_ATmega2560__
    #define ICP_DDR DDRL
    #define ICP_PIN PL0

    #define TMR_CNT TCNT4 
    #define TMR_RB  TCCR4B
    #define ICP_EDG ICES4
    #define TMR_FLG TIFR4
    #define ICP_FLG ICF4
    #define OVF_FLG TOV4
    #define TMR_INT TIMSK4
    #define ICP_INT ICIE4
    #define OVF_INT TOIE4
    #define TMR_CS8 CS41
    #define ICP_CNT ICR4

    #define OVF_VECT TIMER4_OVF_vect
    #define ICP_VECT TIMER4_CAPT_vect
#elif defined __AVR_ATmega328p__
    #define ICP_DDR DDRB
    #define ICP_PIN PB0

    #define TMR_CNT TCNT1
    #define TMR_RB  TCCR1B
    #define ICP_EDG ICES1
    #define TMR_FLG TIFR1
    #define ICP_FLG ICF1
    #define OVF_FLG TOV1
    #define TMR_INT TIMSK1
    #define ICP_INT ICIE1
    #define OVF_INT TOIE1
    #define TMR_CS8 CS11
    #define ICP_CNT ICR1

    #define OVF_VECT TIMER1_OVF_vect
    #define ICP_VECT TIMER1_CAPT_vect
#else
    #error Platform not supported
#endif

extern "C" void ICP_VECT(void) __attribute__((signal));
extern "C" void OVF_VECT(void) __attribute__((signal));

class URM37 {
public:
    static void init();
    static void trigger();
    static int16_t get_distance();

    friend void ICP_VECT(void);
    friend void OVF_VECT(void);

private:
    static volatile int16_t distance;
    static volatile uint16_t then;
    static volatile bool edge;

    static const uint8_t divider;

};

#endif /* _URM37_H_ */