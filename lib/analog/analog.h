#ifndef ANALOG_H
#define ANALOG_H

#include <stdint.h>
#include <avr/io.h>

void analog_init();

uint16_t analog_read(uint8_t pin);

#endif