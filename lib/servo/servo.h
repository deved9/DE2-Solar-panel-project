#ifndef SERVO_H
#define SERVO_H
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h> 
#include "uart.h"

// Servo port and pin definitions
#define SERVO_PORT DDRB
#define HORIZONTAL_SERVO_PIN DDB1
#define VERTICAL_SERVO_PIN DDB2

// Defined for servo Hitec HS-485HB
#define TRANSFORM_CONST 20/3 //Constant of angle-to-compare level transform
#define TRANSFORM_OFFSET 900 //Offset of angle-to-compare level transform


/// @brief Servo initialization
void servo_init();


/// @brief DO NOT USE if other than Hitec HS-485HB servos are connected!
void servo_test();

/// @brief Funtion to control one specified servo
/// @param horizontal Boolean - set as TRUE to control horizontal servo, FALSE to control vertical
/// @param desired_angle Integer value of desired angle from 0 to 180
void turn_servo(bool horizontal, uint8_t desired_angle);

#endif