/*
 * test_code.c
 *
 * Created: 04.09.2015 10:00:17
 *  Author: ottokl
 */ 

#define UART_BAUD 9600

#define MYUBRR F_CPU/16/UART_BAUD-1

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "test_code.h"
#include "../Menu/menu.h"
#include "../Drivers/Uart_driver/uart_driver.h"
#include "../Drivers/SRAM_driver/SRAM_driver.h"
#include "../Drivers/Joystick_driver/joystick_driver.h"
#include "../Drivers/oled_driver.h"


//Flashes the LED
void flash_diode(){
	PORTB |= (1<<PINB0);
	_delay_ms(5);
	PORTB &= ~(1<<PINB0);
	//_delay_ms(50);
}

//Ex 1, task 9
void testCode1(){
	uart_sendChar('X');
	_delay_ms(1000);
}

//Text based shell, used before the joystick was added
void shell(){
	char cmd[256];
	printf("\n[root@skynet]#:");
	scanf("%s", cmd);
	printf( " %s\n", cmd);
	
	if(strcmp(cmd, "DIODE_test") == 0) {
		printf("testing diode...\n");
		for (int i = 0; i < 20; i++) {	flash_diode(); }
		printf("done testing diode\n");
	} 

	else if (strcmp(cmd, "SRAM_test") == 0) {
		SRAM_test();
	} 

	else if (strcmp(cmd, "ADC_test") == 0) {
		while(1){
			printf("Joystick x-pos: %003i \tJoystick y-pos: %003i \tLeft slider: %003d \tRight slider: %003d \n", \
					read_converted(JOYSTICK_X), read_converted(JOYSTICK_Y), joystick_read(SLIDE_L), joystick_read(SLIDE_R));
		}
	}  
	
	else if (strcmp(cmd, "OLED_test") == 0) {
		printf("oled_init(): done\n");
		oled_goto_page(0);
		oled_clear_screen();
		oled_print_arrow(0,0);
		_delay_ms(100);
		while(1){
			//oled_print_char('x');
			//oled_printf("#SWAG4LYFE");
			//oled_write_data(0b00000000);
			//oled_clear_line();
			//oled_clear_screen();
			signed int joy_Y = read_converted(JOYSTICK_Y); 
			if(joy_Y >= 100 || joy_Y <= -100){
				oled_move_arrow(joy_Y, 0, 7);
				_delay_ms(100);
			}
		}
		
	}
	else if (strcmp(cmd, "MENU_test") == 0) {
	//else if (1) {
		menuitem *test_menu = MENU_create_menu();
		//MENU_print(test_menu);
		//test_menu = test_menu->submenus[3];
		//printf(test_menu->submenus[0]->name);
		MENU_navigate(test_menu);
		/*for (unsigned int i = 0; i < test_menu->number_of_submenus; i++) {
			printf("%s\n",test_menu->submenus[i]->name);
		}
		*/
		
		
	}
	
	else if (strcmp(cmd, "clear") == 0){
		oled_clear_screen();
	}

	else {
		printf("unknown command \"%s\"\n", cmd);
	}

	
	
}

