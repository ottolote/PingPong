/*
 * solenoid_driver.c
 *
 * Created: 06.11.2015 11:33:06
 *  Author: ottokl
 */ 

#include <avr/io.h>
#include <util/delay.h>

//private variables
uint8_t solenoid_out_counter;

void solenoid_init(){
	DDRF |= (1<<PF2);
	PORTF |= (1<<PF2);
	
	solenoid_out_counter = 2;
	
}

void solenoid_out(){
	//puts("Solenoid out!");
	PORTF &= ~(1<<PF2);
	solenoid_out_counter = 3;
	//_delay_ms(15);
	
}

void solenoid_in(){
	if (solenoid_out_counter == 0) {
		PORTF |= (1<<PF2);
		solenoid_out_counter = 3;
	} 
	solenoid_out_counter--;
}