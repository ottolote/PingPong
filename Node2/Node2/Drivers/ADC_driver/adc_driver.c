/*
 * adc_driver.c
 *
 * Created: 01.11.2015 15:00:22
 *  Author: haakoneh
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "adc_driver.h"

//Inits the ADC 
void adc_init(){
	ADCSRA	|= (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
	ADMUX	|= (1<<REFS0);
	ADMUX	&= ~(1<<REFS1); 
}

//Reads the ADC in the given channel
uint16_t adc_read(uint8_t channel){
	ADMUX	= (1<<REFS0) | (channel & 0x03);
	ADCSRA	|= (1 << ADSC);

	while((ADCSRA & (1<<ADSC))){ ; }

	return ADC;
}
