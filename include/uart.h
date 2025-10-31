#ifndef _UART_H_
#define _UART_H_ 

#include "avr/io.h"


#define CPU_FREQ 16000000ul

#ifndef BAUD_RATE
    #define BAUD_RATE 9600ul
#endif

/// @brief initialise harware uart 0 for tx and rx 
void uart0_init()
{
    UCSR0A = 0;

    UCSR0B = 0;

    UCSR0B |= (1<<TXEN0) | (1<<RXEN0);

    UCSR0C = 0;

    UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);

    // set prescaler for baud rate  
    // UART_FREQ = CPU_FREQ/16 if U2Xn is 0
    UBRR0 = CPU_FREQ/(16*BAUD_RATE) - 1;

}

void uart0_write_bytes_blocking( char* ptr, uint32_t len )
{
    for ( uint32_t i = 0; i < len; i++ )
    {
        while( !(UCSR0A & (1<<UDRE0)) );
        UDR0 = *( ptr + i );
    }

    // last byte is still been shifted out here
    while( !(UCSR0A & (1<<UDRE0)) );
}

#endif
