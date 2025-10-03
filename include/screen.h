#ifndef _SCREEN_H_
#define _SCREEN_H_

#include "stdlib.h"
#include "inttypes.h"

#include "avr/io.h"

// /// @brief only the lower 4 bits are used 0= most right digit on
// uint8_t digit_select = 0b0000; 

// 1 here means led off as common anode 
const uint8_t bcd_to_7_seg [] = 
{
    // ABCDEFG.
    0b11111100, // 0
    0b01100000, // 1
    0b11011010, // 2
    0b11110010, // 3
    0b01100110, // 4
    0b10110110, // 5
    0b10111110, // 6
    0b11100000, // 7
    0b11111110, // 8
    0b11110110, // 9
    0b11101110, // A
    0b00111110, // b
    0b10011100, // C
    0b01111010, // d
    0b10011110, // E
    0b10001110, // F
};


/// @brief init the screen pins 
inline void init_screen()
{
    // set SRCLK PIN to OUTPUT and LOW (PD7)
    PORTD &= ~(1<<PORTD7);
    DDRD |= 1 << DDD7;

    // set RCLK PIN to OUTPUT and LOW (PD4)
    PORTD &= ~(1<<PORTD4);
    DDRD |= 1 << DDD4;

    // set SDI PIN to OUTPUT and LOW (PB0)
    PORTB &= ~(1<<PORTB0);
    DDRB |= 1 << DDB0;
}


/// @brief set the SRCLK PIN to HIGH 
inline void set_screen_SRCLK()
{
    PORTD |= 1 << PORTD7;
}
/// @brief set the SRCLK PIN to LOW
inline void unset_screen_SRCLK()
{
    PORTD &= ~( 1 << PORTD7 );
}

/// @brief set the RCLK PIN to HIGH 
inline void set_screen_RCLK()
{
    PORTD |= 1 << PORTD4;
}
/// @brief set the RCLK PIN to LOW 
inline void unset_screen_RCLK()
{
    PORTD &= ~( 1 << PORTD4 );
}

/// @brief set the SDI PIN to HIGH
inline void set_screen_SDI()
{
    PORTB |= 1 << PORTB0;
}
/// @brief set the SDI PIN to LOW
inline void unset_screen_SDI()
{
    PORTB &= ~(1<<PORTB0);
}


void write_screen(uint8_t pic, uint8_t digit)
{
    pic = ~pic;
    // srclk rise to shift  
    // rclk rise to update the output

    unset_screen_RCLK();

    // write the picture bits 
    for( uint8_t i = 0; i < 8; i++ )
    {
        unset_screen_SRCLK();

        if ( pic & 1 ) 
            set_screen_SDI();
        else 
            unset_screen_SDI();
        
        set_screen_SRCLK();
        
        pic = pic >> 1;
    }

    // set the 4 unused bits
    unset_screen_SDI(); 
    for( uint8_t i = 0; i < 4; i++ )
    {
        unset_screen_SRCLK();
        set_screen_SRCLK();
    }

    // set the digits 
    for( uint8_t i = 0; i < 4; i++ )
    {
        unset_screen_SRCLK();

        if ( digit & 1 )
            set_screen_SDI();
        else
            unset_screen_SDI();

        set_screen_SRCLK();

        digit = digit >> 1;
    }

    set_screen_RCLK();

}

#endif
