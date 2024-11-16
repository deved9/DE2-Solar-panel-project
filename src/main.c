#include "memory.h"
#include "servo.h"
#include "analog.h"
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>


int main()
{
 
  servo_init();
  analog_init();

  
  while(1) {
    servo_test();
    //turn_servo(true, 0);
    //turn_servo(false, 180);
  }
  
  return 0;
}

/* Interupt Service Routines
--------------------------------------------------------*/



