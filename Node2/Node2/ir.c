/*
 * ir.c
 *
 * Created: 01.11.2015 13:30:09
 *  Author: haakoneh
 */ 

#include <avr/io.h>
#include "ir.h"
#include "Drivers/adc_driver.h"
#include "Drivers/uart_driver.h"

#define FILTER_THRESHOLD 10
#define FILTER_BUFFER_SIZE 4

void ir_filter_init() {
	for (uint8_t i = 0; i < FILTER_BUFFER_SIZE; i++ ){
		ir_obstructed();
	}
}

uint8_t ir_obstructed(){
	static uint16_t ringbuffer_filter[FILTER_BUFFER_SIZE];
	static uint8_t ringbuffer_index;

	ringbuffer_filter[ringbuffer_index] = adc_read(IR_CHANNEL);

	if (ringbuffer_index >= 8){
		ringbuffer_index = 0;
	} else { ringbuffer_index++; }

	uint16_t avg = 0;
	for (uint8_t i = 0; i < FILTER_BUFFER_SIZE; i++) {
		avg += ringbuffer_filter[i] / FILTER_BUFFER_SIZE;
	}

	//printf("Avg: %d", )

	if (avg > FILTER_THRESHOLD) {
		return 1;
	}

	return 0;
}
