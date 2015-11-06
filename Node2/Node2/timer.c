/*
 * timer.c
 *
 * Created: 06.11.2015 16:57:01
 *  Author: ottokl
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ir.h"
#include "Drivers/adc_driver.h"
#include "Drivers/uart_driver.h"

void timer_init() {
	//CS3{2:0}   = 101  prescaler set to 1024
	TCCR4B |=  (1<<CS42) | (1<<CS40);
	TCCR4B &= ~(1<<CS41);
	
	TCCR4A &= ~(1<<WGM40);
	TCCR4A &= ~(1<<WGM41);
	TCCR4B |=  (1<<WGM42);
	TCCR4B &= ~(1<<WGM43);
	//TCCR3A |=  (1<<COM3A1) | (1<<COM3A0);
	
	OCR4A = 223; // 223 for 70 HZ, 7812 for 2 Hz
	
	//enable interrupt on OCR3A compare

	TIMSK4 |= (1<<OCIE4A);
	TIFR4 |= (1<<ICF1);
	
	
	sei();
	
}


ISR(TIMER4_COMPA_vect){
	//printf("read: %d\n",adc_read(0));
	//printf("edge: %d\n\n",ir_edge_detected());

	if(ir_edge_detected()){
		can_ir_transmit();
		
	}
	
	can_handle_message();
}