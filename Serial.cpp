/**
 * @file serial.c
 * 
 * From https://github.com/tuupola/avr_demo/tree/master/blog/simple_usart
 */

#include <Serial.h>

#ifndef SERIAL_RX_BUF_SIZE
#define SERIAL_RX_BUF_SIZE 128
#endif

#ifndef SERIAL_TX_BUF_SIZE
#define SERIAL_TX_BUF_SIZE 128
#endif

Ringbuffer Serial::txbuf(SERIAL_TX_BUF_SIZE);
Ringbuffer Serial::rxbuf(SERIAL_RX_BUF_SIZE);

ISR(SERIAL_UDRE_vect)
{
    if(Serial::txbuf.is_empty()){
        UCSR0B &= ~(1 << UDRIE0);
        return;
    }

    UDR0 = Serial::txbuf.read();
    if(Serial::txbuf.is_empty())
        UCSR0B &= ~(1 << UDRIE0);
}

ISR(SERIAL_RX_vect)
{
    Serial::rxbuf.write(UDR0);
}

int Serial::uputc(char c, FILE *stream) {
    if(c == '\n')
        uputc('\r', stream);
    
    txbuf.write((uint8_t)c);
    
    UCSR0B |= (1 << UDRIE0);

    return 0;
}

int Serial::ugetc(FILE *stream)
{
    while(rxbuf.is_empty());
    return rxbuf.read();
}

bool Serial::init(uint32_t baud, bool doublespd)
{
    uint8_t divspd = 16;
    if(doublespd){
        UCSR0A |= (1 << U2X0);
        divspd = 8;
    }

    uint16_t ubrr_val = F_CPU / divspd / baud - 1;
    
    UBRR0H = (uint8_t)(ubrr_val >> 8);
    UBRR0L = (uint8_t)ubrr_val;

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
    UCSR0B = _BV(RXEN0) | _BV(TXEN0) | (1 << RXCIE0);   /* Enable RX and TX */

    stdout = fdevopen(&(Serial::uputc), NULL);
    stdin  = fdevopen(NULL, &(Serial::ugetc));

    sei();

    return true;
}

uint16_t Serial::available()
{
    return rxbuf.available();
}