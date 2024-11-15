#include <servo.h>

void servo_init(void) {
    
    //Select
    SERVO_PORT |= (1 << VERTICAL_SERVO_PIN) ;
    
    // set PWM for 50% duty cycle
    OCR0A = 128;
    
    // set none-inverting mode
    TCCR0A |= (1 << COM0A1);
   
    // set fast PWM Mode
    TCCR0A |= (1 << WGM01) | (1 << WGM00);
    
    // set prescaler to 8 and starts PWM 
    TCCR0B |= (1 << CS01);
    
}

void servo_test() {
    /*
    double pulse_width = (SERVO_PULSE_MAX - SERVO_PULSE_MIN) / COUNTER_CAPACITY;
    
    for(int8_t i=0;i<16;i++){
        OCR0A = round(pulse_width);
        _delay_ms(1000);
    }
    */
}