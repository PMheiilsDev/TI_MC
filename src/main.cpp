#include "avr/io.h"
#include "avr/interrupt.h"
#include "util/delay.h"

#include <stdlib.h>

#include "screen.h"
#include "adc.h"


int8_t digit = 1;
uint8_t up = 1;

uint8_t adc_result = 0;

int main ()
{
    init_screen();
    init_adc(&adc_result);

    uint8_t number = 1;

    // enable interrups that are in PCMSK1 register (PORTC) 
    PCICR |= (1 << PCIE1);

    // enable the interupt PCINT9 (that is a gpio function of PC1 )
    PCMSK1 |= (1 << PCINT9) | (1<<PCINT10);

    sei();

    // set to output
    DDRB |= 1<<PB2;

    // configure as fast pwm 8 bit mode => WGM13=0,WGM12=1,WGM11=0,WGM10=1
    TCCR1A |= (1<<WGM10);
    TCCR1B |= (1<<WGM12);

    // inverting mode
    TCCR1A |= (1<<COM1B1) | (1<<COM1B0);
    
    // set prescaler to 8 
    TCCR1B |= (1<<CS11);

    // set duty cycle
    OCR1B = 0;

    while(1)
    {
        write_screen( bcd_to_7_seg[number], (1<<digit) );

        // OCR1B +=1;

        number = (number+1)%10;
        
        for( uint8_t i = 0; i < adc_result+1; i++ )
        {
            _delay_ms(1);
        }
    }
}


// this is executed on all external interrupts from PORTC
ISR(PCINT1_vect)
{
    if ( !(PINC & (1<<PINC1)) )
    {
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


    if ( !(PINC & (1<<PINC2)) )
    {
        if ( OCR1B > 1 )
            OCR1B = OCR1B * 10 / 11;
        else if ( OCR1B == 1 )
            OCR1B = 0;
        else
            OCR1B = 255;
    }
}



