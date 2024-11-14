#ifndef SERVO_H
#define SERVO_H
#include <avr/io.h>
#include <stdbool.h>

#define REG_NAME DDRD

void servo_init();
bool servo_test();

#endif