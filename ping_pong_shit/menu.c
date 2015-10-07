/*
 * menu.c
 *
 * Created: 25.09.2015 10:30:01
 *  Author: haakoneh
 */ 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "SRAM_driver.h"
#include "oled_driver.h"
#include "menu.h"



menu new_menu[MAX_TOTAL_MENUS];


menu *MENU_create_menu(menu *new_menu){
	
	new_menu[0].parent = NULL;
	strcpy(new_menu[0].items[0].name, "Test item");
	new_menu[0].items[0].handlerFunc = &SRAM_test;
	new_menu[0].items[0].child = NULL;
	
	return new_menu;	
}


void MENU_print(menu *currentMenu){
	oled_pos(0,4);
	oled_printf(currentMenu[0].items[0].name);
}


/*
menu *MENU_fill_menuitem(string array_menuitem, unsigned int array_size) {
	//int i;
	for(int i = 0; i < array_size; i++){
		
	}
}

*/