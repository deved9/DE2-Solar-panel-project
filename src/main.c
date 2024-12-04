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
#define SOLAR_V 6


// constants
// Photoresistors
#define PR_THR  50


// Global memory init
struct data propertires;

//  Interrupt flags
volatile bool measure_angle = false;
volatile bool measure_panel = false;
volatile bool update_screen = false;


int main()
{
    // Init servo and analog library
    servo_init();
    //servo_test();
    analog_init();

    // Set default values to memory
    propertires.angle_horitzontal = 0;
    propertires.angle_vertical = 0;
    propertires.current = 0;
    propertires.voltage = 0;
    propertires.power = 0;

    // Init UART, I2C and OLED display
    uart_init(UART_BAUD_SELECT(115200, F_CPU));
    oled_init(OLED_DISP_ON);
    oled_clrscr();
    oled_charMode(NORMALSIZE);

    TIM0_ovf_16ms();
    TIM0_ovf_enable();

    /*
    TIM2_ovf_16ms();
    TIM2_ovf_enable();
    */

    // Enable interrupts
    sei();
    

    uart_puts("Init - DONE \r\n");

    while(1) {
        // Handle panel move interrupt
        if (measure_angle) {   
            // Disable interrupts
            cli();    

            // Read values of voltage dividers
            uint16_t upper_left = analog_read(PR_UL); // upper - left
            uint16_t upper_right = analog_read(PR_UR); // upper - right
            uint16_t lower_left = analog_read(PR_LL); // lower - left
            uint16_t lower_right = analog_read(PR_LR); // lower - right     

            bool err = false;
            // average neighbours
            // higher number means less light
            uint16_t left    = (upper_left + lower_left) / 2;
            uint16_t right   = (upper_right + lower_right) / 2;
            uint16_t top     = (upper_left + upper_right) / 2;
            uint16_t bottom  = (lower_left + lower_right) / 2;

            int16_t horizontal_diff = right - left;
            int16_t vertical_diff = top - bottom;

            if (horizontal_diff > PR_THR) // more light on the right
            {
                // change horizontal servo angle
                uart_puts("move right\r\n");
                err = turn_servo(true, --propertires.angle_horitzontal);
                if (err) {
                    propertires.angle_horitzontal++;
                    err = false;
                }
            }
            else if (horizontal_diff < -PR_THR) // more light on the left
            {
                // change horizontal servo angle
                uart_puts("move left\r\n");
                err = turn_servo(true, ++propertires.angle_horitzontal);
                if (err) {
                    propertires.angle_horitzontal--;
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

            // Reset interrupt flag and enable interrupts
            measure_angle = false;
            sei();
        }

        if(measure_panel)
        {
            cli();

            // Read solar panel voltage
            propertires.voltage = (int16_t)analog_read(SOLAR_V);
            propertires.voltage = (int16_t)((propertires.voltage *  4.88759));
            measure_panel = false;
            sei();
        }

        if (update_screen)
        {
            cli();
            char current[4];
            char voltage[4];
            char vertical_angle[3];
            char horizontal_angle[3];

            //oled_init(OLED_DISP_ON);
            //oled_clrscr();
            //oled_charMode(NORMALSIZE);

            // Different layout
            /*
            // set position and show text with values
            oled_gotoxy(0, 2);
            sprintf(current,"Current: %d mA", propertires.current);
            oled_puts(current);

            oled_gotoxy(0, 3);
            sprintf(voltage,"Voltage: %d mV", propertires.voltage);
            oled_puts(voltage);

            oled_gotoxy(0, 4);
            sprintf(vertical_angle,"Vertical ang: %d", propertires.angle_vertical);
            oled_puts(vertical_angle);

            oled_gotoxy(0, 5);
            sprintf(horizontal_angle,"horizontal_angle: %d mW/m2", propertires.horizontal_angle);
            oled_puts(p ower);
            */

            /*
            // row 1
            oled_charMode(NORMALSIZE);
            oled_gotoxy(0, 0);
            oled_puts("Current");

            oled_gotoxy(0, 1);
            oled_puts("[mA]");

            oled_charMode(DOUBLESIZE);
            oled_gotoxy(13, 0);
            sprintf(current,"%d", propertires.current);
            oled_puts("    ");
            oled_gotoxy(13, 0);
            oled_puts(current);
            */

            // row 2
            oled_charMode(NORMALSIZE);
            oled_gotoxy(0, 2);
            oled_puts("Voltage");

            oled_gotoxy(0, 3);
            oled_puts("[mV]");

            oled_charMode(DOUBLESIZE);
            oled_gotoxy(13, 2);
            sprintf(voltage,"%d", propertires.voltage);
            oled_puts("    ");
            oled_gotoxy(13, 2);
            oled_puts(voltage);

            // row 3
            oled_charMode(NORMALSIZE);
            oled_gotoxy(0, 4);
            oled_puts("Vertical ang");

            oled_gotoxy(0, 5);
            oled_puts("[deg]");

            oled_charMode(DOUBLESIZE);
            oled_gotoxy(13, 4);
            sprintf(vertical_angle,"%d", propertires.angle_vertical-90);
            oled_puts("   ");
            oled_gotoxy(13, 4);
            oled_puts(vertical_angle);

             // row 4
            oled_charMode(NORMALSIZE);
            oled_gotoxy(0, 6);
            oled_puts("Horiz. ang");

            oled_gotoxy(0, 7);
            oled_puts("[deg]");

            oled_charMode(DOUBLESIZE);
            oled_gotoxy(13, 6);
            sprintf(horizontal_angle,"%d", propertires.angle_horitzontal);
            oled_puts("   ");
            oled_gotoxy(13, 6);
            oled_puts(horizontal_angle);

            // copy buffer to display RAM and reset interrupt flag
            oled_display();
            update_screen = false;
            sei();
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
    static uint16_t screen_n_ovfs = 0; // counter variable
    ++TIM0_int_count_angle;
    ++TIM0_int_count_panel;
    screen_n_ovfs++;
    
    // wait 13 interrupts (cca 200ms)
    if(TIM0_int_count_angle == 13)
    {
        measure_angle = true;

        // reset count value
        TIM0_int_count_angle = 0;
    }

    if(TIM0_int_count_panel == 62)  // cca 1s
    {
        measure_panel = true;

        // reset count value
        TIM0_int_count_panel = 0;
    }

    if (screen_n_ovfs >= 62) // 62 interrupts = cca 1s
    { 
        screen_n_ovfs = 0; // reset
        update_screen = true;
    }
}

/*
ISR(TIMER2_OVF_vect)
{
    
}
*/