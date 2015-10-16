/*
 * SRAM_driver.c
 *
 * Created: 11.09.2015 12:31:16
 *  Author: haakoneh
 */ 

#define F_CPU 4915200UL // 4.9152 MHz

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "SRAM_driver.h"
#include "oled_driver.h"

//Simple setup for SRAM, also tests for correctness
void SRAM_init(){
	MCUCR = 0x80;
	DDRA = 0xFF;
	SRAM_test();
}

void SRAM_write(unsigned int address_offset, unsigned int data){
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
	ext_ram[address_offset]       = data;
}

unsigned int SRAM_read(unsigned int address_offset){
	if (address_offset > 0x100) { return 0; }
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
	return ext_ram[address_offset];
}

void SRAM_test(void) {
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
	uint16_t ext_ram_size       = 0x800;
	uint16_t write_errors       = 0;
	uint16_t retrieval_errors   = 0;

	oled_clear_screen();
	oled_pos(3,2);
	oled_printf("TESTING SRAM");
	printf("\nStarting SRAM test...\n");

	// rand() stores some internal state, so calling this function in a loop will
	// yield different seeds each time (unless srand() is called before this function)
	uint16_t seed = rand();

	// Write phase: Immediately check that the correct value was stored
	srand(seed);
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		ext_ram[i] = some_value;
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
			write_errors++;
		}
	}

	// Retrieval phase: Check that no values were changed during or after the write phase
	srand(seed);    // reset the PRNG to the state it had before the write phase
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
			retrieval_errors++;
		}
	}
	if (retrieval_errors != 0 || write_errors != 0) { 
		oled_pos(5,0);
		oled_printf("R ERRORS:  %4d",retrieval_errors);
		oled_pos(6,0);
		oled_printf("W ERRORS:  %4d",write_errors);
		_delay_ms(4000);
	} else {
		oled_pos(6,4);
		oled_printf("NO ERRORS");
	}
	printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
}