#include <memory.h>
#include <servo.h>
#include <avr/interrupt.h>

int main()
{
 
  servo_init();

  while(1) {
    //servo_test();
  }
  return 0;
}

/* Interupt Service Routines
--------------------------------------------------------*/
ISR(TIMER1_OVF_vect) {
  
}


