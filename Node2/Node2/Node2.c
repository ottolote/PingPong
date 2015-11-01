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
#include "ir.h"
#include "test_node_2.h"

int main(void){
	uart_init(MYUBRR);
	can_init();
	pwm_init();
	//ir_filter_init();
	//pwm_test();
	//can_test();
	while(1){
		can_read_joy_message();
		//pwm_set_value(150);
		//printf("%d\n", TCNT3);
		//_delay_ms(8);
		
	/*
		if (ir_obstructed()) {
			printf("IR is obstructed\n\n");
			//keep_score();
		} else {
			printf("IR is not obstructed\n\n");
			//not_blocked();
		}*/
		//_delay_ms(800);
	}
}
