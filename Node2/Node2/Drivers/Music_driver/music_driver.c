/*
 * music_driver.c
 *
 * Created: 11.11.2015 19:28:25
 *  Author: ottokl
 */ 

#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "../../Astley/astley1.h"
#include "../../Astley/astley2.h"
#include "../../Astley/astley3.h"
#include "../../Astley/astley4.h"
#include "../../Astley/astley5.h"
#include "../../Astley/astley6.h"
#include "../../Astley/astley7.h"
#include "../../Astley/astley8.h"
//#include "../Astley/short_test.h"
#include "music_driver.h"

#include <avr/interrupt.h>
#include <stdio.h>


volatile int16_t sample;
volatile uint8_t sample_count;
volatile uint8_t astley_count;

#define SAMPLE_DIVIDER 4

//	Initialize the PWM 
void music_init(void)
{
    /* use OC1A pin as output */
    DDRH = _BV(PH3);

    /*
    * clear OC1A on compare match
    * set OC1A at BOTTOM, non-inverting mode
    * Fast PWM, 8bit
    */
    TCCR4A = _BV(COM4A1) | _BV(WGM40);
   
    /*
    * Fast PWM, 8bit
    * Prescaler: clk/1 = 8MHz
    * PWM frequency = 16MHz / (255 + 1) = 62.50kHz
    */
    TCCR4B = _BV(WGM42) | _BV(CS40); //presacler 8 = 010, 1 = 001
   
    /* set initial duty cycle to zero */
    OCR4A = 0;
   
    /* Setup Timer0 */
 
    TCCR0B|=(1<<CS00);
    TCNT0=0;
    TIMSK0|=(1<<TOIE0);
    sample_count = 8;
	astley_count = 0;
    sei(); //Enable interrupts
}

//Called at a 32kHz frequency
ISR(TIMER0_OVF_vect){
	sample_count--;
	if (sample_count == 0) {
		sample_count = 8;    
			      
			switch(astley_count){
				case 0:
					OCR4A = pgm_read_byte(&astley1_data[sample++]);
					break;
				case 1:
					OCR4A = pgm_read_byte(&astley2_data[sample++]);
					break;
				case 2:
					OCR4A = pgm_read_byte(&astley3_data[sample++]);
					break;
				case 3:
					OCR4A = pgm_read_byte(&astley4_data[sample++]);
					break;
				case 4:
					OCR4A = pgm_read_byte(&astley5_data[sample++]);
					break;
				case 5:
					OCR4A = pgm_read_byte(&astley6_data[sample++]);
					break;
				case 6:
					OCR4A = pgm_read_byte(&astley7_data[sample++]);
					break;
				case 7:
					OCR4A = pgm_read_byte(&astley8_data[sample++]);
					break;
				default: 
					break;
			}

            if(sample>astley1_length) {
				sample=0;
				astley_count++;
				if(astley_count>7) { astley_count = 0; }
			}
		}
} 
