/*
 * test_code.c
 *
 * Created: 04.09.2015 10:00:17
 *  Author: ottokl
 */ 

#define F_CPU 4915200UL // 4.9152 MHz
#define UART_BAUD 9600

#define MYUBRR F_CPU/16/UART_BAUD-1

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "uart_driver.h"
#include "test_code.h"
#include <stdlib.h>

//Simple test, Ex 1, task 6
void flash_diode(){
	PORTB = 0xFF;
	_delay_ms(50);
	PORTB = 0x00;
	_delay_ms(50);
}

//Ex 1, task 9
void testCode1(){
	uart_sendChar('X');
	_delay_ms(1000);
}

//Ex 1, task 10 and 11
void shell(){
	char cmd[256];
	printf("\n[root@mainframe]#:");
	scanf("%s", cmd);
	printf( " %s\n", cmd);
	
	if(strcmp(cmd, "testdiode") == 0) {
		printf("testing diode...\n");
		for (int i = 0; i<20; i++) {	flash_diode(); }
		printf("done testing diode\n");
	} else if (strcmp(cmd, "SRAM_test") == 0) {
		SRAM_test();
	} else {
		printf("unknown command \"%s\"\n", cmd);
	}

	
	
}

void SRAM_test(void) {
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
	uint16_t ext_ram_size       = 0x800;
	uint16_t write_errors       = 0;
	uint16_t retrieval_errors   = 0;

	printf("Starting SRAM test...\n");

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
	printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
}
