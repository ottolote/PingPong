/*
 * button_driver.c
 *
 * Created: 04.11.2015 16:56:14
 *  Author: ottokl
 */ 

#include <avr/io.h>

void button_init() {
	DDRB &= ~((1<<PINB1) | (1<<PINB2) | (1<<PINB3));
}

uint8_t button_read(uint8_t channel) {
	return (PINB&(1<<channel)) >> channel;
}

uint8_t button_rising_edge_detect(uint8_t channel){
	static last;
	static uint8_t current;
	current = button_read(channel);
	if (last != current){
		last = current;
		if(current == 1){
			return 1;
		}
	}
	return 0;
}