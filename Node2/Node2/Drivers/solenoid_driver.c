/*
 * solenoid_driver.c
 *
 * Created: 06.11.2015 11:33:06
 *  Author: ottokl
 */ 

#include <avr/io.h>
#include <avr/delay.h>

void solenoid_init(){
	DDRH |= (1<<PH3);
	PORTH |= (1<<PH3);
}

void solenoid_shoot(){
	PORTH &= ~(1<<PH3);
	_delay_ms(15);
	PORTH |= (1<<PH3);
}
