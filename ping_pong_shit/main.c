/*
 * Ping_Pong_Shit.c
 *
 * Created: 28.08.2015 09:59:06
 *  Author: haakoneh & ottokl
 */ 

#define UART_BAUD 9600

#define MYUBRR F_CPU/16/UART_BAUD-1

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "menu.h"
#include "test_code.h"
#include "Drivers/SRAM_driver.h"
#include "Drivers/joystick_driver.h"
#include "Drivers/oled_driver.h"
#include "Drivers/uart_driver.h"
#include "Drivers/mcp2515_driver.h"
#include "Drivers/can_driver.h"

int main(void) {
    uart_init(MYUBRR);
	printf("\nREBOOTED\n");
	SRAM_init();
	joystick_init();
	oled_init();
	//mcp2515_init();
	can_init();	

	printf("\nAll init done\n\n");
	//_delay_ms(1000);

	//mcp2515_test();
	can_test();

	while(1) { 
		//shell();
		//spi_test();

		menuitem* test_menu = MENU_create_menu();
		MENU_navigate(test_menu);
		
		//flash_diode();
    }
}
