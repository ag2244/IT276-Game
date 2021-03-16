#pragma once

#ifndef __UI_TEXTBOX_H__
#define __UI_TEXTBOX_H__

#include "starsabove_entity.h"
#include "starsabove_ui_base.h"

//Data specific to a textbox
typedef struct
{

	char command[256];

} Textbox_Data;

//A link in a menu linked list
typedef struct UI_Node_s
{

	UI_Element* element;
	struct UI_Node_s* next;

} UI_Node;

//A linked list of related textboxes
typedef struct
{
	UI_Element title;

	UI_Node* beginning;
	Vector2D position;

	int spacing_x;
	int spacing_y;

	UI_Element previous_button;
	UI_Element next_button;

} Menu;

//A node in a stack of menus
typedef struct Menu_State_s
{

	Menu* current_menu;
	struct Menu_State_s* previous_menu_state;

} Menu_State;

/**
* @brief Initialize a textbox
* @param position The screen position to draw the textBox
* @param size The size of the textBox
* @return NULL on error, or a pointer to a new textbox
*/
UI_Element *textbox_init(Vector2D position, Vector2D size, char* text, Font* font);

/**
* @brief Initialize a menu state
* @param previous_menu_state (Optional) The previous menu to the current displayed one.
* 
* @param title The title text of the menu: will be converted to a text box
* @param beginning The textbox acting as the top- or left-most box in the menu.
* @param position The screen position to draw the menu
* 
* @param spacing_x The horizontal spacing between boxes (added to the x-size of the previous box)
* @param spacing_y The vertical spacing between boxes (added to the y-size of the previous box)
* 
* @return NULL on error, or a pointer to a new menu state
*/
Menu_State* menu_state_new(Menu_State* previous_menu_state, UI_Element* title, UI_Element* beginning, Vector2D position, int spacing_x, int spacing_y);


#endif
