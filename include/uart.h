#ifndef _UART_H_
#define _UART_H_ 

#include "avr/io.h"


#define CPU_FREQ 16000000

#ifndef BAUD_RATE
    #define BAUD_RATE 9600
#endif

/// @brief initialise harware uart 0 for tx and rx 
void uart0_init()
{
    // for this functionality all bits 0  
    UCSR0A = 0;

    UCSR0B = 0;

    UCSR0B |= (1<<TXEN0) | (1<<RXEN0);

    UCSR0C = 0;

    UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
    
}

void uart0_write_bytes_blocking( uint8_t* ptr, uint32_t len )
{
    for ( uint32_t i = 0; i < len; i++ )
    {
        while( UCSR0A & (1<<UDRE0) );
        UDR0 = *( ptr + i );
        i++; 
    }
    
    // last byte is still been shifted out here
    while( UCSR0A & (1<<UDRE0) );
}

#endif
