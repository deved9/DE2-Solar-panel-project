#ifndef SERVO_H
#define SERVO_H
#include <avr/io.h>
#include <util/delay.h>
//#include <stdbool.h>

#define SERVO_PULSE_FREQ 50 //Frequency od servo pulses, equals to 20 ms
#define SERVO_PULSE_MAX 2.1e-3 //Maximum length of pulse od servo pulses, equals to 2,1 ms
#define SERVO_PULSE_MIN 9e-4 //Minimum length of servo pulses, equals to 0,9 ms 

#define COUNTER_CAPACITY 255 //Number that counter is able to count to

#define SERVO_PORT DDRD
#define HORIZONTAL_SERVO_PIN DDD5
#define VERTICAL_SERVO_PIN DDD6


void servo_init();
void servo_test();

#endif