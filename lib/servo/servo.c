#include <servo.h>

void servo_init(void) {
    // Set Timer/Couter1 as PWM
    /// Assign output pins to port
    SERVO_PORT |= (1 << HORIZONTAL_SERVO_PIN) | (1 << VERTICAL_SERVO_PIN);
 

    /// Set Compare Output Mode - non-inverting
    TCCR1A |= (1 << COM1A1) | (1 << COM1B1);  
    //TCCR1A |= (1 << COM1A1);

    /// Set Phase Correct and Phase and Frequency Correct PWM with ICR1 as TOP
    /// and connect CLK with 1024 prescaler
    TCCR1B |= (1 << WGM13) | (1 << CS12) | (1 << CS10);

    // Define TOP (fixed)
    ICR1 = 156; // = f_CLK / (2 * prescaler * f_desired) = 16e6 / ( 2 * 1024 * 50)

    // Init value (corresponding to angle)
    OCR1A = 1;
    OCR1B = 1;
    
}

void servo_test() {
    
    //double pulse_width = (SERVO_PULSE_MAX - SERVO_PULSE_MIN) / COUNTER_CAPACITY;
    int8_t max = 100;
    int8_t min = 1;
    for(int8_t i=min;i<max;i++){
        OCR1A = i;
        OCR1B = i;
        _delay_ms(100);
    }
    for(int8_t i=max;i>=min;i--){
        OCR1A = i;
        OCR1B = i;
        _delay_ms(100);
    }
    
}