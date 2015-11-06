/*
 * motor.c
 *
 * Created: 06.11.2015 10:39:42
 *  Author: ottokl
 */ 

#include <avr/io.h>
#include "motor_driver.h"

void motor_init() {
	DDRF |= (1<<PF3) | (1<<PF4) | (1<<PF5) | (1<<PF6) | (1<<PF7);
}

