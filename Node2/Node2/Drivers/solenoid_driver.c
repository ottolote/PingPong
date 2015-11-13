/*
 * solenoid_driver.c
 *
 * Created: 06.11.2015 11:33:06
 *  Author: ottokl
 */ 

#include <avr/io.h>
#include <avr/delay.h>

void solenoid_init(){
	DDRF |= (1<<PF2);
	PORTF |= (1<<PF2);
	
	//wtf
	TCCR4B &= ~((1<<CS40) | (1<<CS41) | (1<<CS42));
}

void solenoid_out(){
	//puts("Solenoid out!");
	PORTF &= ~(1<<PF2);
	//_delay_ms(15);
	
}

void solenoid_in(){
	PORTF |= (1<<PF2);
}