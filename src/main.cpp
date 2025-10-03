#include "avr/io.h"
#include "avr/interrupt.h"
#include "util/delay.h"

#include <stdlib.h>

#include "screen.h"


int8_t digit = 1;
uint8_t up = 1;


int main ()
{
    init_screen();

    uint8_t number = 1;

    // enable interrups that are in PCMSK1 register (PORTC) 
    PCICR |= (1 << PCIE1);

    // enable the interupt PCINT9 (that is a gpio function of PC1 )
    PCMSK1 |= (1 << PCINT9);

    sei();

    while(1)
    {
        write_screen( bcd_to_7_seg[number], (1<<digit) );

        number = (number+1)%10;
        
        _delay_ms(500);
    }
}


// this is executed on all external interrupts from PORTC
ISR(PCINT1_vect)
{
    if ( PINC & (1<<PINC1) )
        return;

    if ( digit <= 0 )
    {
        up = 1;
    }
    else if ( digit >= 3 )
        up = 0;

    if ( up )
        digit++;
    else
        digit--;

}



