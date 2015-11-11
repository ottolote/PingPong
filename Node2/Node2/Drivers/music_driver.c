/*
 * music_driver.c
 *
 * Created: 11.11.2015 19:28:25
 *  Author: ottokl
 */ 

#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "../Astley/astley1.h"
#include "../Astley/astley2.h"
#include "../Astley/astley3.h"
#include "../Astley/astley4.h"
#include "../Astley/astley5.h"
#include "../Astley/astley6.h"
#include "../Astley/astley7.h"
#include "../Astley/astley8.h"
#include "music_driver.h"

#include <avr/interrupt.h>
#include <stdio.h>
#define SAMPLE_RATE 2666;

volatile uint16_t sample;
volatile uint8_t astleynr;
int sample_count;

//uint8_t* samplepointer;

/*
ISR(TIMER0_OVF_vect)
{
    
         sample_count--;
         if (sample_count == 0)
            {
             sample_count = 4;          
             OCR1A = pgm_read_byte(&pcm_samples[sample++]);
             if(sample>pcm_length)sample=0;
            }
}*/


void music_init(){
	//PWM output active (pin3)
	DDRH   |=  (1<<PH4);
	
	//CS3{2:0}   = 011  prescaler set to 64
/*
	TCCR3B |=  (1<<CS30) | (1 << CS31);
	TCCR3B &= ~(1<<CS32);*/

	//CS3{2:0}   = 010  prescaler set to 8
	TCCR4B |=   (1<<CS41);
	TCCR4B &= ~((1<<CS42) | (1<<CS40));
	
	//WGM3{3:0}  = 1110 mode set to fast PWM
	TCCR4A |=  (1<<WGM41);
	TCCR4A &= ~(1<<WGM40);
	TCCR4B |=  (1<<WGM43) | (1<<WGM42);
	
	//COM3A{1:0} = 10   clear bit on ICR3 compare
	TCCR4A |=  (1<<COM4B1);
	TCCR4A &= ~(1<<COM4B0);
	
	TIMSK4 |= (1<<OCIE4A);
	TIFR4 |= (1<<ICF1);
	
	//Setting ICR3 to DUTY_CYCLE_STEPS (20ms)
	ICR4 = 300;
	
	//music_set_sample_value(20000);
	//samplepointer = astley1_data;
}

void music_set_sample_value() {
	/*OCR4B = 64;*/
/*
	switch(astleynr){
		case 1:
			OCR4B = pgm_read_byte(&astley1_data[sample++]);
		case 2:
			OCR4B = pgm_read_byte(&astley2_data[sample++]);
		case 3:
			OCR4B = pgm_read_byte(&astley3_data[sample++]);
		case 4:
			OCR4B = pgm_read_byte(&astley4_data[sample++]);
		case 5:
			OCR4B = pgm_read_byte(&astley5_data[sample++]);
		case 6:
			OCR4B = pgm_read_byte(&astley6_data[sample++]);
		case 7:
			OCR4B = pgm_read_byte(&astley7_data[sample++]);
		case 8:
			OCR4B = pgm_read_byte(&astley8_data[sample++]);
	}
	if (sample>astley1_length) {
		sample = 0;
		astleynr++;
	}
	if (astleynr>8){
		astleynr = 0;
	}*/
}


ISR(TIMER4_COMPA_vect){
	music_set_sample_value();
}