#pragma once

#ifndef __UI_QTY_TEXTBOX_H__
#define __UI_QTY_TEXTBOX_H__

#include "starsabove_entity.h"
#include "starsabove_ui_base.h"

typedef struct 
{

	int qty;
	UI_Element* up;
	UI_Element* down;

} Qty_Textbox_Data;

typedef struct
{

	Bool can_increment;

} Increment_Data;

Qty_Textbox_Data* qty_textbox_data_new(UI_Element* qty_textbox);

Qty_Textbox_Data* qty_textbox_data(UI_Element* qty_textbox);

Increment_Data* increment_data_new(UI_Element* incrementer);

Increment_Data* increment_data(UI_Element* incrementer);

UI_Element* uparrow_init();

UI_Element* downarrow_init();

UI_Element* qty_textbox_init(Vector2D position, Vector2D size, char* text, Font* font);

void qty_textbox_draw(UI_Element* self);

void qty_textbox_free(UI_Element* self);

#endif