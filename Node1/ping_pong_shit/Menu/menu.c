/*
 * menu.c
 *
 * Created: 25.09.2015 10:30:01
 *  Author: haakoneh
 */ 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

#include "menu.h"
#include "../Test_code/test_code.h"
#include "../Timer/timer.h"
#include "../Game/game.h"
#include "../Drivers/Joystick_driver/joystick_driver.h"
#include "../Drivers/SRAM_driver/SRAM_driver.h"
#include "../Drivers/oled_driver.h"
#include "../Drivers/MCP2515_driver/mcp2515_driver.h"
#include "../Drivers/can_driver.h"

#define MENU_INDENT 2

//	Creates a new menuitem
menuitem* MENU_new_item(char *name, void (*handlerFunc)(void), uint8_t number_of_submenus) {
	menuitem* new_item = malloc(sizeof(menuitem));
	
	new_item->name = name;
	new_item->handlerFunc = handlerFunc;
	new_item->number_of_submenus = number_of_submenus;
	new_item->submenus = malloc(number_of_submenus * sizeof(menuitem));
	
	return new_item;
}

//	A recursive function assign current menu as parent to all its submenus 
void MENU_assign_parents(menuitem *current_menu){
	for (uint8_t n = 0; n < current_menu->number_of_submenus; n++) {
		current_menu->submenus[n]->parent = current_menu;
		if (current_menu->submenus[n]->number_of_submenus > 0) {
			MENU_assign_parents(current_menu->submenus[n]);
		}
	}
}

//	Hardcoded the entire system of menus 
menuitem* MENU_create_menu(){
	menuitem* root_menu = MENU_new_item("Main", NULL, 3);
	root_menu->parent = NULL;
	root_menu->submenus[0] = MENU_new_item("Play game", game_main, 0);
	root_menu->submenus[1] = MENU_new_item("Tetris", can_play_music, 0);
	root_menu->submenus[2] = MENU_new_item("Test functions", NULL, 2);
	root_menu->submenus[2]->submenus[0] = MENU_new_item("Flash diode", flash_diode, 0);
	root_menu->submenus[2]->submenus[1] = MENU_new_item("SRAM test", SRAM_test, 0);

	MENU_assign_parents(root_menu);
		
	return root_menu;
}

//	Prints the current menu system the user is in 
void MENU_print(menuitem *current_menu_item){
	oled_clear_screen();
	oled_print_arrow(1, 0);
	oled_pos(0,0);
	oled_printf(current_menu_item->name);
	
	for (unsigned int i = 0; i < current_menu_item->number_of_submenus; i++) {
		oled_pos(i + 1, MENU_INDENT);
		oled_printf(current_menu_item->submenus[i]->name);
	}
}

//Handles the navigation of the menu
void MENU_navigate(menuitem *current_menu){
	MENU_print(current_menu);
	while(1){
		if(joystick_direction_Y() != 0){
			oled_arrow_handler(joystick_direction_Y(), 1, current_menu->number_of_submenus);
		}
		
		if(joystick_direction_X() != 0){
			if (joystick_direction_X() == 1) {
				if (current_menu->submenus[oled_get_arrow_page() - 1]->number_of_submenus > 0) {
					current_menu = current_menu->submenus[oled_get_arrow_page() - 1];
					MENU_print(current_menu);
				} else  if (current_menu->submenus[oled_get_arrow_page() - 1]->handlerFunc != NULL) {
					current_menu->submenus[oled_get_arrow_page() - 1]->handlerFunc();
					MENU_print(current_menu);
				}
			} else if (joystick_direction_X() == -1 && current_menu->parent != NULL) {
				current_menu = current_menu->parent;
				MENU_print(current_menu);
			}
		
			_delay_ms(100);
		}
	}
}
