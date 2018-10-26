#include <URM37.h>
#include <Serial.h>

#if F_CPU == 16000000UL
    const uint8_t URM37::divider = 2;
#elif F_CPU == 8000000UL
    const uint8_t URM37::divider = 1;
#else
    #error Frequency not supported
#endif

volatile int16_t URM37::distance;
volatile uint16_t URM37::then;
volatile bool URM37::edge;

// CHANGE TO COMP based on the divider
ISR(TIMER1_OVF_vect)
{
    TCCR1B &= ~(1 << CS11); // Disable timer
    URM37::distance = -2;
}

ISR(TIMER1_CAPT_vect)
{
    if(URM37::edge){
        TCCR1B &= ~(1 << CS11); // Disable timer
        
        URM37::distance = (ICR1 - URM37::then) / URM37::divider / 50;
        if(URM37::distance > 204) URM37::distance = -2;
    } else {
        TCCR1B |= (1 << ICES1); // Rising Edge
        URM37::then = ICR1;
        URM37::edge = true;
    }
}

void URM37::init()
{
    DDRB &= ~(1 << PB0); // ICP1 Input
    DDRB |= (1 << PB1);  // Trigger output
    PORTB |= (1 << PB1); // Trigger high
    distance = -2; // -2 means need retrigger
    then = 0;
    edge = false;

    TCNT1 = 0; // Zero the timer;
    TCCR1B |= (1 << ICES1); // Rising Edge to avoid unexpected interruption

    TIFR1 &= ~((1 << ICF1) | (1 << TOV1)); // Clear flags
    TIMSK1 |= (1 << ICIE1) | (1 << TOIE1); // Enable ICP int and COMPA int
}

void URM37::trigger()
{
    distance = -1; // -1 means polling
    TCNT1 = 0;
    TCCR1B &= ~(1 << ICES1); // Falling edge
    edge = false;

    // Low pulse triggers
    PORTB &= ~(1 << PB1);
    _delay_us(5);   // Sync to a digitalWrite() timing
    PORTB |= (1 << PB1);

    TCCR1B |= (1 << CS11); // Enable timer
}

int16_t URM37::get_distance()
{
    int16_t ret;
    ATOMIC_BLOCK(ATOMIC_FORCEON){
        ret = distance;
    }

    return ret;
}