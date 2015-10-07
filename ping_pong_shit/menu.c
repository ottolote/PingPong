/*
 * menu.c
 *
 * Created: 25.09.2015 10:30:01
 *  Author: haakoneh
 */ 

#include <stdlib.h>
#include <stdio.h>

typedef struct menu {
	char name;
	struct menu *parent;
	struct menu *sub_item;
	int children, menu_min, menu_max;
	void *(*functionPointer)();
} menu;

menu *MENU_create_menu(char *name){
	menu *new_menu = (menu*)malloc(sizeof(menu));
	
	if (new_menu == NULL) {
		printf("Out of memory!\n");
		exit(1);
	}
	
	new_menu->name = name;
	new_menu->sub_item = NULL;
	new_menu->children = NULL;
	new_menu->menu_min = NULL;
	new_menu->menu_max = NULL;
	new_menu->functionPointer = NULL;
	
	return new_menu;	
}
