/*
 * Ping_Pong_Shit.c
 *
 * Created: 28.08.2015 09:59:06
 *  Author: haakoneh & ottokl
 */ 

#define F_CPU 4915200UL // 4.9152 MHz
#define F_OSC 4915200UL // 4.9152 MHz
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
#include "Drivers/spi_driver.h"
#include "Drivers/mcp2515_driver.h"
#include "Drivers/can_driver.h"
#include "Drivers/can_driver_test.h"

int main(void) {
    uart_init(MYUBRR);
	printf("\nREBOOTED\n");
	SRAM_init();
	joystick_init();
	oled_init();
	can_init();	

	printf("All init done\n");	

	can_test_loopback();

	while(1) { 
		//shell();
		//spi_test();

		menuitem* test_menu = MENU_create_menu();
		MENU_navigate(test_menu);
		
		flash_diode();
    }
}
