/*
 * Node2.c
 *
 * Created: 28.10.2015 17:02:58
 *  Author: haakoneh
 */ 

#define UART_BAUD 9600
#define MYUBRR F_CPU/16/UART_BAUD-1

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "Drivers/can_driver.h"
#include "Drivers/uart_driver.h"
#include "Drivers/pwm_driver.h"

int main(void){
	uart_init(MYUBRR);
	can_init();
	pwm_init();
	//pwm_test();
	//can_test();
	while(1){
		can_read_joy_message();
		//pwm_set_value(150);
		//printf("%d\n", TCNT3);
		_delay_ms(8);
	}
	
	
	
	
/*
	DDRB = 0xff;
	while(1){
		PORTB = 0xff;
		_delay_ms(500);
		PORTB = 0;
		_delay_ms(500);
	}*/
}