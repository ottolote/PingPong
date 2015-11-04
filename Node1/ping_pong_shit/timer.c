/*
 * timer.c
 *
 * Created: 01.11.2015 18:48:30
 *  Author: ottokl
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Drivers/can_driver.h"
#include "Drivers/button_driver.h"
#include "test_code.h"

void timer_init() {
		//CS3{2:0}   = 101  prescaler set to 1024
		TCCR3B |=  (1<<CS32) | (1<<CS30);
		TCCR3B &= ~(1<<CS31);
		
		TCCR3A &= ~(1<<WGM30);
		TCCR3A &= ~(1<<WGM31);
		TCCR3B |=  (1<<WGM32);
		TCCR3B &= ~(1<<WGM33);
		//TCCR3A |=  (1<<COM3A1) | (1<<COM3A0);
		
		OCR3A = 68; //ca 70Hz 
		
		//enable interrupt on OCR3A compare

		ETIMSK |= (1<<OCIE3A);
		TIFR |= (1<<ICF1);
		
		
		sei();
		
}

ISR(TIMER3_COMPA_vect){
	can_joystick_transmit();
	if(button_rising_edge_detect(1)){
		can_button_transmit(1);
		flash_diode();
	}
}