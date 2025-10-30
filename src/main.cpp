#include <avr/io.h>
#include <avr/interrupt.h>
#include "util/delay.h"

#define CHEAT 
#ifdef CHEAT
#include "Arduino.h"
#endif

/*
// TODO
    make adc start every 10ms 
    using timer0 interupt to start conversion
    
    configure adc ✅ 
    set prescaler of adc to 64 as that works 

    allowed to use Serial object 
    main loop waits 1.5 s then writes avg ✅ 
*/

const uint16_t print_delay_ms = 1500;

const uint8_t adc_amt = 10;
uint8_t current_adc = 0;

uint16_t adc_results[adc_amt];

int main(void)
{
    init();
    Serial.begin(9600);

    #pragma region adc setup
    // reset used registers after init call 
    ADCSRA = 0;
    ADCSRB = 0;
    ADMUX = 0;

    // enable adc 
    // enable adc interupt
    // set prescaler to 64 
    // enable autotrigger
    ADCSRA |= (1<<ADEN) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1);

    // // set to trigger on timer0 compare match A 
    // ADCSRB |= (1<<ADTS0) | (1<<ADTS1);

    // set ad mux to A0
    ADMUX &= ~( (1<<MUX0) | (1<<MUX1) | (1<<MUX2) | (1<<MUX3) );

    ADMUX |= (1<<REFS0);

    #pragma endregion


    #pragma region timer0 setup 
    // reset used registers after init call 
    TCCR0A = 0;
    TCCR0B = 0;
    TIMSK0  = 0;


    TCCR0A |= (1<<WGM01);

    // set prescaler to 1024 
    TCCR0B |= (1<<CS00) | (1<<CS02);

    OCR0A = 155;
    
    // // enable compA match interupt 
    TIMSK0 |= (1<<OCIE0A);
    
    #pragma endregion 

    ADCSRA |= (1<<ADSC); 

    while(1)
    {
        uint32_t sum = 0;
        for ( uint8_t i = 0; i < adc_amt; i++ )
        {
            sum += adc_results[i];
        }
        char buffer[20];

        int avg = sum/adc_amt;

        sprintf(buffer, "Value: %d\n", avg );
        Serial.print(buffer);
        _delay_ms(print_delay_ms);
    }
}

ISR(ADC_vect)
{
    // store result 
    adc_results[current_adc] = ADC;

    // increment to next position 
    current_adc = (current_adc+1)%adc_amt;
}

ISR(TIMER0_COMPA_vect)
{
    // start conversion
    ADCSRA |= (1<<ADSC);
}

