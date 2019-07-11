#include <URM37.h>

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
ISR(OVF_VECT)
{
    TMR_RB &= ~(1 << TMR_CS8); // Disable timer
    URM37::distance = -2;
}

ISR(ICP_VECT)
{
    if(URM37::edge){
        TMR_RB &= ~(1 << TMR_CS8); // Disable timer
        
        URM37::distance = (ICP_CNT - URM37::then) / URM37::divider / 50;
        if(URM37::distance > 204) URM37::distance = -2;
    } else {
        TMR_RB |= (1 << ICP_EDG); // Rising Edge
        URM37::then = ICP_CNT;
        URM37::edge = true;
    }
}

void URM37::init()
{
    ICP_DDR &= ~(1 << ICP_PIN); // ICP1 Input
    TRIGGER_DDR |= (1 << TRIGGER_PIN);  // Trigger output
    TRIGGER_PORT |= (1 << TRIGGER_PIN); // Trigger high
    distance = -2; // -2 means need retrigger
    then = 0;
    edge = false;

    TMR_CNT = 0; // Zero the timer;
    TMR_RB |= (1 << ICP_EDG); // Rising Edge to avoid unexpected interruption

    TMR_FLG &= ~((1 << ICP_FLG) | (1 << OVF_FLG)); // Clear flags
    TMR_INT |= (1 << ICP_INT) | (1 << OVF_INT); // Enable ICP int and COMPA int
}

void URM37::trigger()
{
    distance = -1; // -1 means polling
    TMR_CNT = 0;
    TMR_RB &= ~(1 << ICP_EDG); // Falling edge
    edge = false;

    // Low pulse triggers
    TRIGGER_PORT &= ~(1 << TRIGGER_PIN);
    _delay_us(5);   // Sync to a digitalWrite() timing
    TRIGGER_PORT |= (1 << TRIGGER_PIN);

    TMR_RB |= (1 << TMR_CS8); // Enable timer with /8 prescaler
}

int16_t URM37::get_distance()
{
    int16_t ret;
    ATOMIC_BLOCK(ATOMIC_FORCEON){
        ret = distance;
    }

    return ret;
}