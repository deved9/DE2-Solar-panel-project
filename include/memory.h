#pragma once
#include <stdint.h>

struct data
{
    uint16_t current; // current in mA
    uint16_t voltage; // voltage in mV
    uint8_t angle; // angle of solar panel
    uint16_t power; // power in mW/m^2 
};
