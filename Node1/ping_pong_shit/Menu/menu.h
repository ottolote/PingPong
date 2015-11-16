/*
 * menu.h
 *
 * Created: 25.09.2015 10:30:20
 *  Author: haakoneh
 */ 


#ifndef MENU_H_
#define MENU_H_

/*
typedef struct menuitem {
	char name[32];
	void (*handlerFunc)(int, int);
	struct menu *child;
} menuitem;

typedef struct menu {
	struct menu *parent;
	struct menuitem items[MAX_MENUITEMS]; //Childen is a list of menuitems
} menu;*/ 

typedef struct menuitem {
	uint8_t number_of_submenus;
	struct menuitem **submenus;
	struct menuitem *parent;
	char *name;
	void (*handlerFunc)(void);
} menuitem;

menuitem* MENU_create_menu();
menuitem* MENU_new_item(char *name, void (*handlerFunc)(void), uint8_t number_of_submenus);
void MENU_print(menuitem *currentMenu);
void MENU_navigate(menuitem *current_menu);



#endif /* MENU_H_ */