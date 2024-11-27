#ifndef MEMORY_H
#define MEMORY_H
#include <stdint.h>

struct data
{
    uint16_t current; // current in mA
    uint16_t voltage; // voltage in mV
    uint8_t angle_horitzontal; // horizontal angle of solar panel
    uint8_t angle_vertical; // vertical angle of solar panel
    uint16_t power; // power in mW/m^2 
};

#endif