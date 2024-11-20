#include "analog.h"

#include <stdint.h>
#include <avr/io.h>


void analog_init()
{
    //DDRC |= (1 << DDC0) | (1 << DDC1) | (1 <<DDC2) | (1 <<DDC3);

    // enable ADC
    ADCSRA = ADCSRA | (1 << ADEN);

    // set prescaler to 2 (lowest value - fastest)
    ADCSRA = ADCSRA & ~(0b111 << ADPS0);

    // set analog reference to AVcc
    ADMUX = (ADMUX & ~(1 << REFS1)) | (1 << REFS0);
}

uint16_t analog_read(uint8_t pin)
{
    // select input pin into MUX
    ADMUX = (ADMUX & ~(0b1111 << MUX0)) | (pin & ~(0b1111 << 4));

    // start DAC
    ADCSRA = ADCSRA | (1 << ADSC);

    // wait for DAC
    while(ADCSRA & (1 << ADSC));
    //ADMUX |= (1 << ADLAR);
    // join result registers into single variable
    //uint16_t res = 0 | (ADCH << 8) | ADCL;
    /*
    uint16_t res_l = ADCL;
    uint16_t res_h = ADCH * 4;
    */
    uint16_t res = ADCL | (ADCH << 8);
    //res = (res *4) + ADCL;
    return res;
}