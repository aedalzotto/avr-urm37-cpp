/**
 * @file serial.h
 * 
 * File from https://github.com/tuupola/avr_demo/tree/master/blog/simple_usart
 */

#ifndef _AVR_SERIAL_H_
#define _AVR_SERIAL_H_

#include <stdio.h>
#include <avr/io.h>
#include <Ringbuffer.h>
#include <avr/interrupt.h>

#if defined (__AVR_ATmega2560__)
    #define SERIAL_UDRE_vect USART0_UDRE_vect
    #define SERIAL_RX_vect USART0_RX_vect
#elif defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328__)
    #define SERIAL_UDRE_vect USART_UDRE_vect
    #define SERIAL_RX_vect USART_RX_vect
#else
    #error "MCU Not supported yet, please develop and push to git!"
#endif

extern "C" void SERIAL_UDRE_vect(void) __attribute__((signal));
extern "C" void SERIAL_RX_vect(void) __attribute__((signal));

class Serial {
public:
    static bool init(uint32_t baud, bool doublepsd = true);

    static int uputc(char c, FILE *stream);
    static int ugetc(FILE *stream);

    static uint16_t available();

    friend void SERIAL_UDRE_vect(void);
    friend void SERIAL_RX_vect(void);

private:
    static Ringbuffer rxbuf;
    static Ringbuffer txbuf;

};


#endif /* _AVR_SERIAL_H_ */