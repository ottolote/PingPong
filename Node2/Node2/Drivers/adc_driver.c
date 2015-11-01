/*
 * adc_driver.c
 *
 * Created: 01.11.2015 15:00:22
 *  Author: haakoneh
 */ 

#include <avr/io.h>
#include "adc_driver.h"

void adc_init(){
	ADCSRA	|= (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
	ADMUX	|= (1 << REFS0);
	ADMUX	&= ~( 1<< REFS1); 
}

uint8_t adc_read(uint8_t channel){
	ADMUX	|= (channel & 0x03);
	ADCSRA	|= (1 << ADSC);

	while(!(ADCSRA & ADSC)){ ; }

	uint16_t result = ADCL | (ADCH << 8);

	return result;
}
