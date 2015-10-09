/*
 * Ping_Pong_Shit.c
 *
 * Created: 28.08.2015 09:59:06
 *  Author: haakoneh & ottokl
 */ 

#define F_CPU 4915200UL // 4.9152 MHz
#define UART_BAUD 9600

#define MYUBRR F_CPU/16/UART_BAUD-1

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "uart_driver.h"
#include "test_code.h"
#include "SRAM_driver.h"
#include "joystick_driver.h"
#include "oled_driver.h"
#include "menu.h"

int main(void) {
    uart_init(MYUBRR);
	printf("\nREBOOTED\n");
	SRAM_init();
	joystick_init();
	oled_init();
	
	while(1) { 
		//shell();

		
		menuitem* test_menu = MENU_create_menu();
		MENU_navigate(test_menu);
		
		flash_diode();
    }
}
