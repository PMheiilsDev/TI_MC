#ifndef _ADC_H_
#define _ADC_H_

#include "avr/io.h"
#include "avr/interrupt.h"

uint8_t* adc_result_ptr = 0;

// start the adc in free running mode with interrupt enabled
// 
inline void init_adc( uint8_t* ptr )
{
    adc_result_ptr = ptr;

    // select AVcc as referance voltage
    ADMUX &= ~(1<<REFS1);
    ADMUX |= (1<<REFS0); 

    // activate left alignment
    ADMUX |= (1<<ADLAR);

    // select ADC0 for Muliplexer
    ADMUX &= ~( (1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0) );

    // activate ADC Hardware
    ADCSRA |= (1<<ADEN);

    // enable Auto Trigger
    ADCSRA |= (1<<ADATE);

    // enable adc conversion finished interupt 
    ADCSRA |= (1<<ADIE);

    // set prescaler to 2 as thats the lowest possible 
    ADCSRA &= ~( (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0) );
    //128
    ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); 
    
    // set to freerunning mode as trigger
    ADCSRB &= ~( (1<<ADTS2) | (1<<ADTS1) | (1<<ADTS0) );

    // disable digital pin mode of PC0 (ADC0)
    DIDR0 |= (1<<ADC0D);

    // enable interrupts 
    sei();

    // start first conversion
    ADCSRA |= (1<<ADSC);

} 

uint8_t adc_read()
{
    // start conversion
    ADCSRA |= (1<<ADSC);

    // wait for conversion to complete 
    while( ADCSRA & (1<<ADSC) ); 
    
    // only the higher one as only 8 bit wanted 
    return ADCH;
}

ISR(ADC_vect)
{
    // only the higher one as only 8 bit wanted 
    *adc_result_ptr = ADCH;
}

#endif
