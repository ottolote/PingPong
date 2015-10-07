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
#include <stdlib.h>
#include "uart_driver.h"
#include "test_code.h"
#include "SRAM_driver.h"
#include "joystick_driver.h"
#include "oled_driver.h"
#include "menu.h"

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
		menu *testMenu;
		printf("\nTest1\n");
		testMenu = MENU_create_menu();
		printf("%s\n", testMenu[0].items[0].name);
		MENU_print(testMenu);
		/*while(1){
			
		}*/
	}

	else {
		printf("unknown command \"%s\"\n", cmd);
	}

	
	
}

