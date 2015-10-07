/*
 * menu.h
 *
 * Created: 25.09.2015 10:30:20
 *  Author: haakoneh
 */ 


#ifndef MENU_H_
#define MENU_H_

#define MAX_TOTAL_MENUS 32
#define MAX_MENUITEMS 7

typedef struct menuitem {
	char name[32];
	void (*handlerFunc)(int, int);
	struct menu *child;
} menuitem;

typedef struct menu {
	struct menu *parent;
	struct menuitem items[MAX_MENUITEMS]; //Childen is a list of menuitems
} menu;


menu *MENU_create_menu();

void MENU_print(menu *currentMenu);



#endif /* MENU_H_ */