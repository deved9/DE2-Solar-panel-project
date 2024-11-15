#include <servo.h>

void servo_init(void) {
    // Set Timer/Couter1 as PWM
    /// Assign output pins to port
    SERVO_PORT |= (1 << HORIZONTAL_SERVO_PIN) | (1 << VERTICAL_SERVO_PIN);
 

    /// Set Compare Output Mode - non-inverting
    TCCR1A |= (1 << COM1A1) | (1 << COM1B1);  
    //TCCR1A |= (1 << COM1A1);

    /// Set Phase Correct and Phase and Frequency Correct PWM with ICR1 as TOP
    /// and connect CLK with CLK/8 prescaler
    TCCR1B |= (1 << WGM13) | (1 << CS11);

    // Define TOP (fixed)
    // WARNING: The input capture register is only 16-bit in size!
    ICR1 = 20000; // = f_CLK / (2 * prescaler * f_desired) = 16e6 / (2 * 50)

    // Init value (corresponding to angle)
    OCR1A = 900;
    OCR1B = 900;
    
}

void servo_test() {
    int16_t max = 2100;
    int16_t min = 900;

    for(int16_t i=min;i<=max;i++){
        OCR1A = i;
        _delay_ms(1);
    }
    for(int16_t i=min;i<=max;i++){
        OCR1B = i;
        _delay_ms(1);
    }
    for(int16_t i=max;i>=min;i--){
        OCR1A = i;
        _delay_ms(1);
    }
    for(int16_t i=max;i>=min;i--){
        OCR1B = i;
        _delay_ms(1);
    }
    for(int16_t i=min;i<=max;i++){
        OCR1A = i;
        OCR1B = i;
        _delay_ms(1);
    }
    for(int16_t i=max;i>=min;i--){
        OCR1A = i;
        OCR1B = i;
        _delay_ms(1);
    }
    
}

void turn_servo(bool horizontal, int8_t desired_angle) {
    // Transform from angle to PWM comparator level
    double transform = TRANSFORM_CONST * (double) desired_angle + TRANSFORM_OFFSET;

    int16_t comp_level = (int16_t) transform;

    if (horizontal) {
        OCR1A = comp_level;
    }
    else {
        OCR1B = comp_level;
    }
}