/*
 * test_node_2.c
 *
 * Created: 01.11.2015 16:59:06
 *  Author: haakoneh
 */ 

#include <stdio.h>
#include <avr/io.h>
#include "Drivers/uart_driver.h"
#include "test_node_2.h"

uint8_t ir_not_blocked = 1;
uint8_t game_score = 0;

void keep_score(){
	if (ir_not_blocked) {
		game_score++;
		ir_not_blocked = 0;
		printf("Score: %d\n\n", game_score);
	} 
}

void not_blocked(){
	ir_not_blocked = 1;
}