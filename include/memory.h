#ifndef MEMORY_H
#define MEMORY_H
#include <stdint.h>

struct data
{
    int16_t current; // current in mA
    int16_t voltage; // voltage in mV
    int16_t angle_horitzontal; // horizontal angle of solar panel
    int16_t angle_vertical; // vertical angle of solar panel
    int16_t power; // power in mW
};

#endif