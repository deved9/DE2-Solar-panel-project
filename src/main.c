#include "memory.h"
#include "servo.h"
#include "analog.h"
#include "timer.h"
#include "uart.h"
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>

// PIN assignmesnt
// Photoresistors (anaolg pins)
#define PR_UL   0
#define PR_UR   1
#define PR_LL   2
#define PR_LR   3


// constants
// Photoresistors
#define PR_THR  50



int main()
{

    TIM0_ovf_16ms();
    TIM0_ovf_enable();
    
    sei();


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
ISR(TIMER0_OVF_vect)
{
    static uint8_t TIM0_int_count = 0;
    TIM0_int_count++;

    // wait 13 interrupts (cca 200ms)
    if(TIM0_int_count == 13)
    {
        // reset count value
        TIM0_int_count = 0;

        // read photoresistor values
        uint16_t pr_ul = analog_read(PR_UL); // upper - left
        uint16_t pr_ur = analog_read(PR_UR); // upper - right
        uint16_t pr_ll = analog_read(PR_LL); // lower - left
        uint16_t pr_lr = analog_read(PR_LR); // lower - right

        // average neighbours
        // higher number means less light
        int16_t left    = (pr_ul + pr_ll) / 2; // >> 1 can be used for faster and somewhat accurate division
        int16_t right   = (pr_ur + pr_lr) / 2;
        int16_t top     = (pr_ul + pr_ur) / 2;
        int16_t bottom  = (pr_ll + pr_lr) / 2;

        int16_t horizontal_diff = left - right;
        int16_t vertical_diff = top - bottom;

        if (horizontal_diff > PR_THR) // more light on the right
        {
            // change horizontal servo angle
            uart_puts("move right\r\n");

        }
        else if (horizontal_diff < -PR_THR) // more light on the right
        {
            // change horizontal servo angle
            uart_puts("move left\r\n");
        }
        
        if (vertical_diff > PR_THR) // more light on the top
        {
            // change horizontal servo angle
            uart_puts("move up\r\n");
        }
        else if (horizontal_diff < -PR_THR) // more light on the bottom
        {
            // change horizontal servo angle
            uart_puts("move down\r\n");
        }  
    }
}
