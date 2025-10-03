#include "avr/io.h"
#include "util/delay.h"

#include <stdlib.h>

#include "screen.h"

int main ()
{
    init_screen();

    uint8_t number = 1;
    int8_t digit = 1;
    uint8_t up = 1;
    while(1)
    {
        write_screen( bcd_to_7_seg[number], (1<<digit) );

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

        number = (number+1)%10;
        
        _delay_ms(500);
    }
}





