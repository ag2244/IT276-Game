#include <stdlib.h>
#include "gfc_types.h"
#include "gfc_vector.h"

#include "gf2d_sprite.h"
#include "simple_logger.h"

#include "starsabove_ui_base.h"

#include "starsabove_game_resources.h"

UI_Manager ui_manager = { 0 };

void ui_manager_init(Uint32 max_elements)
{

	if (max_elements == 0) {
		slog("Cannot allocate 0 UI Elements!");
		return;
	}

	if (ui_manager.element_list != NULL) {
		ui_manager_free();
	}

	ui_manager.element_list = (UI_Element*)gfc_allocate_array(sizeof(UI_Element), max_elements);

	if (ui_manager.element_list == NULL) {
		slog("Failed to allocate UI Element list!");
		return;
	}

	ui_manager.max_elements = max_elements;
	atexit(ui_manager_free);
	slog("UI system initialized");
}

UI_Manager* ui_manager_get() {

	if (ui_manager.element_list == NULL) {
		slog("UI Element system does not exist!");
		return;
	}

	return &ui_manager;

}

void ui_manager_free() {

	if (ui_manager.element_list != NULL) {
		free(ui_manager.element_list);
	}
	memset(&ui_manager, 0, sizeof(UI_Manager));
	slog("UI_Element system freed");
}

void ui_update(UI_Element* self)
{
	if (!self) return;

	//Animate
	self->frame += self->frameRate;
	if (self->frame >= self->frameCount) self->frame = 0;

	// DO ANY GENERIC UPDATE CODE
	// IF THERE IS A CUSTOM UPDATE, DO THAT NOW
	if (self->update) self->update(self);
}

void ui_manager_update() {

	int i;

	if (ui_manager.element_list == NULL) {
		slog("UI_Element system does not exist!");
		return;
	}

	for (i = 0; i < ui_manager.max_elements; i++)
	{
		if (ui_manager.element_list[i]._inuse == 0) continue;

		ui_update(&ui_manager.element_list[i]);
	}
}

void ui_manager_draw() {

	int i;

	if (ui_manager.element_list == NULL) {
		slog("UI_Element system does not exist!");
		return;
	}

	for (i = 0; i < ui_manager.max_elements; i++)
	{
		if (ui_manager.element_list[i]._inuse == 0) continue;

		ui_draw(&ui_manager.element_list[i]);
	}
}

UI_Element* ui_new()
{

	int i;

	if (ui_manager.element_list == NULL) {
		slog("UI_Element system does not exist!");
		return;
	}

	for (i = 0; i < ui_manager.max_elements; i++)
	{

		if (ui_manager.element_list[i]._inuse) continue; // Someone else using this one
		memset(&ui_manager.element_list[i], 0, sizeof(UI_Element));

		ui_manager.element_list[i]._inuse = 1;
		
		ui_manager.element_list[i].text_position_relative = vector2d(0, 0);
		ui_manager.element_list[i].text_color = gfc_color(1, 1, 1, 0);

		ui_manager.element_list[i].hidden = 0;

		return &ui_manager.element_list[i];
	}

	slog("No free UI elements available");
	return NULL;
}

void ui_free(UI_Element* element)
{

	if (!element)
	{
		slog("Cannot free a NULL UI_Element");
		return;
	}

	if (element->signal)
	{
		gameevent_free(element->signal);
		element->signal == NULL;
	}

	if (element->collider_box)
	{
		box_free(element->collider_box);
		element->collider_box == NULL;
	}

	if (element->collider_circle)
	{
		box_free(element->collider_circle);
		element->collider_circle == NULL;
	}


	gf2d_sprite_free(element->spriteMain);
	gf2d_sprite_free(element->spriteBorder);

	element->spriteBorder = NULL;
	element->spriteMain = NULL;

	element->_inuse = 0;
}

void ui_draw(UI_Element* element)
{
	if (!element)
	{
		slog("Cannot draw a NULL UI_Element");
		return;
	}

	if (element->hidden == 1)
	{
		return;
	}

	//If there's a custom draw
	else if (element->draw) element->draw(element);

	else {

		if (element->spriteMain == NULL) return; //Nothing to draw
		
		if (element->spriteBorder != NULL) //If there is a border sprite (assumes there is an offset if so)
		{
			Vector2D sum; //Sum of the position plus offset
			vector2d_add(sum, element->position, element->offset);

			gf2d_sprite_draw(
				element->spriteBorder,
				element->position,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				(Uint32)element->frame);

			gf2d_sprite_draw(
				element->spriteMain,
				sum,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				(Uint32)element->frame);
		}

		if ((element->font != NULL) && (element->text != NULL))
		{
			Vector2D textpos;

			vector2d_add(textpos, element->position, element->text_position_relative);

			font_render(element->font, element->text, textpos, element->text_color);
		}

		else gf2d_sprite_draw(
			element->spriteMain,
			element->position,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			(Uint32)element->frame);

	}
}

Bool ui_clickable(UI_Element* element, float mX, float mY) {

	//slog("UI_Element_clickable");

	Vector2D pos = vector2d(mX, mY);

	if (element->hidden == 1)
	{
		return 0;
	}

	if (element->collider_box != NULL)
	{
		if (box_clickable(element->collider_box, pos) != 0)
		{
			return 1;
		}
	}

	else if (element->collider_circle != NULL)
	{
		if (circle_clickable(element->collider_circle, pos) != 0)
		{
			return 1;
		}
	}

	return 0;

}

void ui_addevent(UI_Element* self, Game_Event* signal)
{
	self->signal = malloc(sizeof(Game_Event));

	strcpy(self->signal->target_id, signal->target_id);
	strcpy(self->signal->command, signal->command);
	strcpy(self->signal->descriptor, signal->descriptor);
	self->signal->qty = signal->qty;
	self->signal->menu_state = signal->menu_state;

	self->signal->_sent = 0;
}

void ui_onClick(UI_Element* element, Game_Event* event_reciever) 
{

	if (!element)
	{
		slog("Cannot click on a NULL UI element");
		return;
	}

	//If there's a custom onClick
	if (element->onClick)
	{
		element->onClick(element, event_reciever);
	}

	else 
	{

		//TODO

	}

}