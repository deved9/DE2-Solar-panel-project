#include "analog.h"

void analog_init() {
    // Select voltage reference - Vcc
    ADMUX |= (1 << REFS0);
    ADMUX &= ~(1 << REFS1);

    // Enable ADC, set prescaler - needs 50 kHz to 200 kHz CLK
    // to get maximum resoultion
    // 16 MHz / 128 = 125 KHz
    ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 <<ADPS1) | (1 << ADPS0);

    // Trigger source - free running mode
    ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));

    // Disable power reduction
    PRR &= ~(1 << PRADC);
}

uint16_t analog_read(uint8_t pin) {
    switch (pin) {
    case 0:
        ADMUX &= ~((1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0));
        break;

    case 1:
        ADMUX &= ~((1<<MUX3) | (1<<MUX2) | (1<<MUX1));
        ADMUX |= (1 << MUX0);
        break;
  
    case 2:
        ADMUX &= ~((1<<MUX3) | (1<<MUX2) | (1<<MUX0));
        ADMUX |= (1 << MUX1); 
        break;
      
    case 3:
        ADMUX &= ~((1<<MUX3) | (1<<MUX2));
        ADMUX |= ((1<<MUX1) | (1 << MUX0));
        break;
    
    case 6:
        ADMUX &= ~((1<<MUX3) | (1<<MUX0));
        ADMUX |= ((1<<MUX2) | (1 << MUX1));
        break;

    case 7:
        ADMUX &= ~((1<<MUX3));
        ADMUX |= ((1<<MUX1) | (1 << MUX0)) | (1 << MUX2);
        break;
    
    default:
        return;
    }
    
    // Begin conversion
    ADCSRA |= (1 << ADSC);

    // ADSC is set as 0 once conversion is completed
    while (ADCSRA & (1<<ADSC)) {}

    // Return fetched data from registers
    // NOTE: ADCL has to be read first
    return ADCL | (ADCH << 8);
}
