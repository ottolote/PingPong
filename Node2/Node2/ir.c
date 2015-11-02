/*
 * ir.c
 *
 * Created: 01.11.2015 13:30:09
 *  Author: haakoneh
 */ 

#include <avr/io.h>
#include <stdio.h>
#include "ir.h"
#include "Drivers/adc_driver.h"
#include "Drivers/uart_driver.h"

#define FILTER_THRESHOLD 260
#define FILTER_BUFFER_SIZE 16

void ir_filter_init() {
	for (uint8_t i = 0; i < FILTER_BUFFER_SIZE; i++ ){
		ir_obstructed();
	}
}

uint8_t ir_obstructed(){
	static uint16_t ringbuffer_filter[FILTER_BUFFER_SIZE];
	static uint8_t ringbuffer_index;

	//return adc_read(IR_CHANNEL);

	if (ringbuffer_index >= FILTER_BUFFER_SIZE - 1){
		ringbuffer_index = 0;
	} else { ringbuffer_index++; }
			
	ringbuffer_filter[ringbuffer_index] = adc_read(IR_CHANNEL) / FILTER_BUFFER_SIZE;



	uint16_t avg = 0;
	for (uint8_t i = 0; i < FILTER_BUFFER_SIZE; i++) {
		avg += ringbuffer_filter[i] ;
	}
	
	/*printf("index: %d\nfilter content: [ %d", ringbuffer_index, ringbuffer_filter[0]);
	for (uint8_t i = 1; i < FILTER_BUFFER_SIZE; i++) {
		printf(", %d", ringbuffer_filter[i]);
	}
	printf(" ]\n");
	printf("avg: %d\n",avg);*/
	
	if (avg < FILTER_THRESHOLD) {
		return 1;
	}

	return 0;
}

uint8_t ir_edge_detected() {
	static uint8_t last;
	if (last != ir_obstructed()){
		last = ir_obstructed();
		return 1;
	}
	return 0;
}