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

#include "SRAM_driver.h"
#include "oled_driver.h"
#include "menu.h"
#include "joystick_driver.h"
#include "test_code.h"

#define MENU_INDENT 2

unsigned int last_arrow_pos = 1;

/*
menu *MENU_create_menu(menu *new_menu){
	
	new_menu[0].parent = NULL;
	strcpy(new_menu[0].items[0].name, "Test item");
	new_menu[0].items[0].handlerFunc = &SRAM_test;
	new_menu[0].items[0].child = NULL;
	
	return new_menu;	
}*/

menuitem* MENU_new_item(char *name, void (*handlerFunc)(void), uint8_t number_of_submenus) {
	menuitem* new_item = malloc(sizeof(menuitem));
	
	new_item->name = name;
	new_item->handlerFunc = handlerFunc;
	new_item->number_of_submenus = number_of_submenus;
	new_item->submenus = malloc(number_of_submenus * sizeof(menuitem));
	
	return new_item;
}

void MENU_assign_parents(menuitem *current_menu){
	for (uint8_t n = 0; n < current_menu->number_of_submenus; n++) {
		current_menu->submenus[n]->parent = current_menu;
		if (current_menu->submenus[n]->number_of_submenus > 0) {
			MENU_assign_parents(current_menu->submenus[n]);
		}
	}
}


menuitem* MENU_create_menu(){
	menuitem* root_menu = MENU_new_item("Main", NULL, 4);
	root_menu->parent = NULL;
	root_menu->submenus[0] = MENU_new_item("Test1", SRAM_test, 0);
	root_menu->submenus[1] = MENU_new_item("Test2", SRAM_test, 0);
	root_menu->submenus[2] = MENU_new_item("Test3", SRAM_test, 0);
	root_menu->submenus[3] = MENU_new_item("Test4", NULL, 1);
	root_menu->submenus[3]->submenus[0] = MENU_new_item("Flash diode", flash_diode, 0);

	

	
	MENU_assign_parents(root_menu);
		
	return root_menu;
}

//fixed
void MENU_print(menuitem *current_menu_item){
	oled_clear_screen();
	oled_print_arrow(last_arrow_pos, 0);
	oled_pos(0,0);
	oled_printf(current_menu_item->name);
	//printf("%s\n", current_menu_item->name);
	
	for (unsigned int i = 0; i < current_menu_item->number_of_submenus; i++) {
		oled_pos(i + 1, MENU_INDENT);
		oled_printf(current_menu_item->submenus[i]->name);
		//printf("%s\n",current_menu_item->submenus[i]->name);
	}
}

void MENU_navigate(menuitem *current_menu){
	MENU_print(current_menu);
	while(1){
		if(joystick_direction_Y() != 0){
			last_arrow_pos = oled_arrow_handler(joystick_direction_Y(), 1, current_menu->number_of_submenus);
		}
		
		if(joystick_direction_X() != 0){
			if (joystick_direction_X() == 1) {
				if (current_menu->submenus[oled_get_arrow_page() - 1]->number_of_submenus > 0) {
					current_menu = current_menu->submenus[oled_get_arrow_page() - 1];
					printf(current_menu->submenus[0]->name);
					last_arrow_pos = 1;
					MENU_print(current_menu);
				} else  if (current_menu->submenus[oled_get_arrow_page() - 1]->handlerFunc != NULL) {
					current_menu->submenus[oled_get_arrow_page() - 1]->handlerFunc();
					last_arrow_pos = 1;
					MENU_print(current_menu);
				}
			} else if (joystick_direction_X() == -1 && current_menu->parent != NULL) {
				current_menu = current_menu->parent;
				last_arrow_pos = 1;
				MENU_print(current_menu);
			}
		
			_delay_ms(500);
		}
	}
}
