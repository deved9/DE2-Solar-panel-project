#include "memory.h"
#include "servo.h"
#include "analog.h"
#include "timer.h"
#include "uart.h"
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>

//#define debug

// CPU clock
#ifndef F_CPU
    #define F_CPU 16000000  // CPU frequency in Hz required for UART_BAUD_SELECT
#endif

// PIN assignmesnt
// Photoresistors (anaolg pins)
#define PR_UL 0
#define PR_UR 1
#define PR_LL 2
#define PR_LR 3


// constants
// Photoresistors
#define PR_THR  50

// Global memory init
struct data propertires;

int main()
{
    servo_init();
    servo_test();
    analog_init();

    propertires.angle_horitzontal = 0;
    propertires.angle_vertical = 0;

    TIM0_ovf_16ms();
    TIM0_ovf_enable();

    TIM2_ovf_16ms();
    TIM2_ovf_enable();


    sei();
    uart_init(UART_BAUD_SELECT(115200, F_CPU));

    uart_puts("Init - DONE");

    while(1) {
    }

    return 0;
}

/* Interupt Service Routines
--------------------------------------------------------*/
ISR(TIMER0_OVF_vect)
{
    static uint8_t TIM0_int_count = 0;
    ++TIM0_int_count;


    // wait 13 interrupts (cca 200ms)
    if(TIM0_int_count == 13)
    {
        
        bool err = false;
        // read photoresistor values
        uint16_t pr_ul = analog_read(PR_UL); // upper - left
        uint16_t pr_ur = analog_read(PR_UR); // upper - right
        uint16_t pr_ll = analog_read(PR_LL); // lower - left
        uint16_t pr_lr = analog_read(PR_LR); // lower - right

        // average neighbours
        // higher number means less light
        uint16_t left    = (pr_ul + pr_ll) / 2; // >> 1 can be used for faster and somewhat accurate division
        uint16_t right   = (pr_ur + pr_lr) / 2;
        uint16_t top     = (pr_ul + pr_ur) / 2;
        uint16_t bottom  = (pr_ll + pr_lr) / 2;
        
        #ifdef debug
        char str[16];
        itoa(pr_ul, str, 10);
        uart_puts("Upper left: ");
        uart_puts(str);
        uart_puts("\r\n");

        _delay_ms(10);

        itoa(pr_ur, str, 10);
        uart_puts("Upper right: ");
        uart_puts(str);
        uart_puts("\r\n");
        #endif

        int16_t horizontal_diff = right - left;
        int16_t vertical_diff = top - bottom;

        if (horizontal_diff > PR_THR) // more light on the right
        {
            // change horizontal servo angle
            err = turn_servo(true, ++propertires.angle_horitzontal);
            if (err) {
                propertires.angle_horitzontal--;
                err = false;
            }
        }
        else if (horizontal_diff < -PR_THR) // more light on the right
        {
            // change horizontal servo angle
            uart_puts("move left\r\n");
            err = turn_servo(true, --propertires.angle_horitzontal);
            if (err) {
                propertires.angle_horitzontal++;
                err = false;
            }
        }
        
        if (vertical_diff > PR_THR) // more light on the top
        {
            // change vertical servo angle
            uart_puts("move up\r\n");
            err = turn_servo(false, ++propertires.angle_vertical);
            if (err) {
                propertires.angle_vertical--;
                err = false;
            }
        }
        else if (vertical_diff < -PR_THR) // more light on the bottom
        {
            // change horizontal servo angle
            uart_puts("move down\r\n");
            err = turn_servo(false, --propertires.angle_vertical);
            if (err) {
                propertires.angle_vertical++;
                err = false;
            }
        } 

        // reset count value
        TIM0_int_count = 0;
    }
}

ISR(TIMER2_OVF_vect)
{


}