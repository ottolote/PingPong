/*
 * ir.c
 *
 * Created: 01.11.2015 13:30:09
 *  Author: haakoneh
 */ 

#include <avr/io.h>
#include <stdio.h>
#include "ir_driver.h"
#include "../ADC_driver/adc_driver.h"
#include "../Uart_driver/uart_driver.h"

#define FILTER_THRESHOLD 50
#define FILTER_BUFFER_SIZE 16

//	Fills the ringbuffer to avoid trash data
void ir_filter_init() {
	for (uint8_t i = 0; i < FILTER_BUFFER_SIZE; i++ ){
		ir_obstructed();
	}
}

//	Checks if IR is obstructed by using a ring buffer to filter 
//	out any disturbances
uint8_t ir_obstructed(){
	static uint16_t ringbuffer_filter[FILTER_BUFFER_SIZE];
	static uint8_t ringbuffer_index;

	if (ringbuffer_index >= FILTER_BUFFER_SIZE - 1){
		ringbuffer_index = 0;
	} else { ringbuffer_index++; }
			
	ringbuffer_filter[ringbuffer_index] = adc_read(IR_CHANNEL) / FILTER_BUFFER_SIZE;

	uint16_t avg = 0;
	for (uint8_t i = 0; i < FILTER_BUFFER_SIZE; i++) {
		avg += ringbuffer_filter[i] ;
	}

	if (avg < FILTER_THRESHOLD) {
		return 1;
	}

	return 0;
}

//	Checks if IR voltage changes
uint8_t ir_edge_detected() {
	static uint8_t current;
	current = ir_obstructed();
	if (last_ir_val != current){
		last_ir_val = current;
		return 1;
	}
	return 0;
}