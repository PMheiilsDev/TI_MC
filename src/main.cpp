#include <avr/io.h>
#include <avr/interrupt.h>
#include "util/delay.h"

int main(void)
{
    // LED on PC0
    DDRC |= (1 << PC0);

    // Turn off ADC (required for ACME!)
    ADCSRA &= ~(1 << ADEN);

    // Select ADC4 as inverting input for comparator
    ADMUX = (ADMUX & 0xF0) | 0x04;

    // Enable AC multiplexer
    ADCSRB |= (1 << ACME);

    // Disable digital input buffers on comparator pins
    DIDR1 |= (1 << AIN0D) | (1 << AIN1D);

    // Configure Comparator in ONE write:
    // ACD=0  (enable comparator)
    // ACBG=0 (use external AIN0)
    // ACIE=1 (interrupt enable)
    // ACIS1=0, ACIS0=0 (any edge)
    ACSR = (1<<ACIE) ;

    // Enable global interrupts
    sei();

    while (1)
    {
        // do nothing, ISR toggles LED
    }
}

ISR(ANALOG_COMP_vect)
{
    if ( ACSR >>ACO )
        PORTC |= (1 << PC0);
    else
        PORTC &= ~(1<<PC0);
}
#include "SPI.h"
