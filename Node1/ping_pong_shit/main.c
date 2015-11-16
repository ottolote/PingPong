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
#include "Menu/menu.h"
#include "Test_code/test_code.h"
#include "Drivers/SRAM_driver/SRAM_driver.h"
#include "Drivers/Button_driver/button_driver.h"
#include "Drivers/Joystick_driver/joystick_driver.h"
#include "Drivers/oled_driver.h"
#include "Drivers/Uart_driver/uart_driver.h"
#include "Drivers/MCP2515_driver/mcp2515_driver.h"
#include "Drivers/Can_driver/can_driver.h"
#include "Timer/timer.h"

int main(void) {
    uart_init(MYUBRR);
	printf("\nREBOOTED\n");
	SRAM_init();
	joystick_init();
	button_init();
	oled_init();
	//mcp2515_init();
	can_init();	
	timer_init();

	DDRB |= (1<<DDB0);

	printf("\nAll init done\n\n");
	
	menuitem* test_menu = MENU_create_menu();
	MENU_navigate(test_menu);

}
