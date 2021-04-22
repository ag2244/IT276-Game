#include "simple_logger.h"

#include "starsabove_ui_qtytextbox.h"

#include "starsabove_ui_textbox.h"
#include "starsabove_game_resources.h"

Qty_Textbox_Data* qty_textbox_data(UI_Element* qty_textbox)
{
	return (struct Qty_Textbox_Data*)qty_textbox->data;
}

Increment_Data* increment_data(UI_Element* incrementer)
{
	return (struct Increment_Data*)incrementer->data;
}

UI_Element* uparrow_init()
{

}

UI_Element* downarrow_init()
{

}

UI_Element* qty_textbox_init(Vector2D position, Vector2D size, char* text, Font* font)
{
	UI_Element* self = textbox_init(position, size, text, font);

	if (!self)
	{
		slog("Unable to create Qty Textbox"); return NULL;
	}

	return self;
}

void qty_textbox_draw(UI_Element* self)
{
	//Qty_Textbox_Data* data = qty_textbox_data(self);


}

void qty_textbox_free(UI_Element* self)
{

}