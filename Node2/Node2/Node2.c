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

int main(void){
	uart_init(MYUBRR);
	can_init();
	can_test();
	while(1){
		printf("test\n");
		_delay_ms(300);
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