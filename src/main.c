#include <avr/io.h>
#include "memory.h"
#include "servo.h"
#include "analog.h"
#include "timer.h"
#include "uart.h"
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>
#include "oled.h"

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

// solar panel current and voltage
#define SOLAR_I 4
#define SOLAR_V 5


// constants
// Photoresistors
#define PR_THR  50

// solar power measurement
#define R_H 1000
#define R_L 10000
#define CURR_CONST 9.76 // I(mA)/512
#define CURR_OFFSET 512  // VCC/2


// Global memory init
struct data propertires;
volatile bool measure_angle = false;
volatile bool measure_panel = false;


int main()
{
    servo_init();
    //servo_test();
    analog_init();

    propertires.angle_horitzontal = 0;
    propertires.angle_vertical = 0;

    uart_init(UART_BAUD_SELECT(115200, F_CPU));

    TIM0_ovf_16ms();
    TIM0_ovf_enable();

    TIM2_ovf_16ms();
    TIM2_ovf_enable();


    sei();
    

    uart_puts("Init - DONE \r\n");

    while(1) {
        if (measure_angle) {   
            cli();    
            uint16_t upper_left = analog_read(PR_UL); // upper - left
            uint16_t upper_right = analog_read(PR_UR); // upper - right
            uint16_t lower_left = analog_read(PR_LL); // lower - left
            uint16_t lower_right = analog_read(PR_LR); // lower - right     

            bool err = false;
            // average neighbours
            // higher number means less light
            uint16_t left    = (upper_left + lower_left) / 2; // >> 1 can be used for faster and somewhat accurate division
            uint16_t right   = (upper_right + lower_right) / 2;
            uint16_t top     = (upper_left + upper_right) / 2;
            uint16_t bottom  = (lower_left + lower_right) / 2;

            int16_t horizontal_diff = right - left;
            int16_t vertical_diff = top - bottom;

            if (horizontal_diff > PR_THR) // more light on the right
            {
                // change horizontal servo angle
                uart_puts("move right\r\n");
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
                err = turn_servo(false, --propertires.angle_vertical);
                if (err) {
                    propertires.angle_vertical--;
                    err = false;
                }
            }
            else if (vertical_diff < -PR_THR) // more light on the bottom
            {
                // change horizontal servo angle
                uart_puts("move down\r\n");
                err = turn_servo(false, ++propertires.angle_vertical);
                if (err) {
                    propertires.angle_vertical++;
                    err = false;
                }
            } 
            sei();
            measure_angle = false;
        }

        if(measure_panel)
        {
            propertires.voltage = analog_read(SOLAR_V) * 5000/1023 * ((R_L+R_H)/R_L);
            propertires.current = ( analog_read(SOLAR_I) - CURR_OFFSET ) * CURR_CONST;
            measure_panel = false;
        }
    }
    
    return 0;
}

/* Interupt Service Routines
--------------------------------------------------------*/
ISR(TIMER0_OVF_vect)
{
    static uint8_t TIM0_int_count_angle = 0;
    static uint8_t TIM0_int_count_panel = 0;
    ++TIM0_int_count_angle;
    ++TIM0_int_count_panel;
    
    // wait 13 interrupts (cca 200ms)
    if(TIM0_int_count_angle == 13)
    {
        measure_angle = true;

        // reset count value
        TIM0_int_count_angle = 0;
    }

    if(TIM0_int_count_panel == 35)
    {
        measure_panel = true;

        // reset count value
        TIM0_int_count_panel = 0;
    }
}

ISR(TIMER2_OVF_vect)
{
    static uint16_t n_ovfs = 0; // counter variable

    n_ovfs++;
    if (n_ovfs >= 62) // 62 interrupts = cca 1s
    { 
        n_ovfs = 0; // reset

        // display setup
        oled_init(OLED_DISP_ON);
        oled_clrscr();
        oled_charMode(NORMALSIZE);

        // set position and show text with values
        oled_gotoxy(0, 1);
        oled_puts(("Current: %u mA", propertires.current));

        oled_gotoxy(0, 2);
        oled_puts(("Voltage: %u mV", propertires.voltage));

        oled_gotoxy(0, 3);
        oled_puts(("Vertical angle: %u deg", propertires.angle_vertical));

        oled_gotoxy(0, 4);
        oled_puts(("Power: %u mW/m2", propertires.power));

        // copy buffer to display RAM
        oled_display();
    }
}