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
#include "SRAM_driver.h"
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
	printf("\n[root@skynet]#:");
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

