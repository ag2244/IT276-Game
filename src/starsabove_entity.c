#include <stdlib.h>
#include "gfc_types.h"
#include "gfc_vector.h"

#include "gf2d_sprite.h"
#include "simple_logger.h"

#include "starsabove_entity.h"

EntityManager entity_manager = { 0 };

void entity_manager_init(Uint32 max_entities)
{

	if (max_entities == 0) {
		slog("Canot allocate 0 entities!");
		return;
	}

	if (entity_manager.entity_list != NULL) {
		entity_manager_free();
	}

	entity_manager.entity_list = (Entity*)gfc_allocate_array(sizeof(Entity), max_entities);

	if (entity_manager.entity_list == NULL) {
		slog("Failed to allocate entity list!");
		return;
	}

	entity_manager.max_entities = max_entities;
	atexit(entity_manager_free);
	slog("Entity system initialized");
}

EntityManager* entity_manager_get() {

	if (entity_manager.entity_list == NULL) {
		slog("Entity system does not exist!");
		return;
	}

	return &entity_manager;

}

Entity* get_entity_by_name(char* name)
{
	int i;
	Entity* current_entity;

	if (entity_manager.entity_list == NULL) {
		slog("Entity system does not exist!");
		return;
	}

	if (name == NULL) {
		slog("Cannot get an entity with NULL name");
		return;
	}

	for (i = 0; i < entity_manager.max_entities; i++)
	{
		current_entity = &entity_manager.entity_list[i];

		if (current_entity->_inuse && current_entity->name)
		{

			//slog("%s == %s", current_entity->name, name); //For comparison

			if (strcmp(current_entity->name, name) == 0)
			{ 
				return &entity_manager.entity_list[i];
			}
				
		}
	}

	//slog("No Entity with the name: \"%s\"", name);
	return NULL;
}

void entity_manager_free() {

	if (entity_manager.entity_list != NULL) {
		free(entity_manager.entity_list);
	}
	memset(&entity_manager, 0, sizeof(EntityManager));
	slog("Entity system freed");
}

void entity_update(Entity* self) 
{
	if (!self) return;

	// Move
	vector2d_add(self->position, self->position, self->velocity);

	//Animate
	self->frame += self->frameRate;
	if (self->frame >= self->frameCount) self->frame = 0;

	// DO ANY GENERIC UPDATE CODE
	// IF THERE IS A CUSTOM UPDATE, DO THAT NOW
	if (self->update) self->update(self);
}

void entity_manager_update_entities() {

	int i;

	if (entity_manager.entity_list == NULL) {
		slog("Entity system does not exist!");
		return;
	}

	for (i = 0; i < entity_manager.max_entities; i++) 
	{
		if (entity_manager.entity_list[i]._inuse == 0) continue;

		entity_update(&entity_manager.entity_list[i]);
	}
}

void entity_manager_draw_entities() {

	int i;

	if (entity_manager.entity_list == NULL) {
		slog("Entity system does not exist!");
		return;
	}

	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (entity_manager.entity_list[i]._inuse == 0) continue;
		
		entity_draw(&entity_manager.entity_list[i]);
	}
}

Entity* entity_new() 
{

	int i;

	if (entity_manager.entity_list == NULL) {
		slog("Entity system does not exist!");
		return;
	}

	for (i = 0; i < entity_manager.max_entities; i++) 
	{

		if (entity_manager.entity_list[i]._inuse) continue; // Someone else using this one
		memset(&entity_manager.entity_list[i], 0, sizeof(Entity));

		entity_manager.entity_list[i]._inuse = 1;

		return &entity_manager.entity_list[i];
	}

	slog("No free entities available");
	return NULL;
}

Entity* entity_new_name(char* name)
{
	Entity* new_ent = NULL;

	//If there is an entity with this name
	if (get_entity_by_name(name) != NULL)
	{
		slog("Entity exists with the name \"%s\", cannot create!", name);
		return;
	}

	new_ent = entity_new();

	if (new_ent == NULL) return;

	gfc_line_cpy(new_ent->name, name);
	//new_ent->name = name;

	return new_ent;
}

void entity_free(Entity* ent) 
{
	if (!ent) 
	{
		slog("Cannot free a NULL entity");
		return;
	}

	if (ent->collider_box)
	{
		box_free(ent->collider_box);
	}

	if (ent->collider_circle)
	{
		box_free(ent->collider_circle);
	}

	gf2d_sprite_free(ent->sprite);
	ent->sprite = NULL;
	ent->_inuse = 0;
	//ent->name = NULL;
	ent->data = NULL;
}

void entity_draw(Entity* ent) 
{
	if (!ent)
	{
		slog("Cannot draw a NULL entity");
		return;
	}

	//If there's a custom draw
	if (ent->draw) ent->draw(ent);

	else {

		if (ent->sprite == NULL) return; //Nothing to draw

		gf2d_sprite_draw(
			ent->sprite,
			ent->position,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			(Uint32)ent->frame );

	}
}

Bool entity_clickable(Entity* ent, float mX, float mY) {

	Vector2D pos = vector2d(mX, mY);

	if (ent->collider_box != NULL)
	{
		if (box_clickable(ent->collider_box, pos) != 0)
		{
			return 1;
		}
	}

	else if (ent->collider_circle != NULL)
	{
		if (circle_clickable(ent->collider_circle, pos) != 0)
		{
			return 1;
		}
	}

	return 0;

}

void entity_onClick(Entity* ent, Game_Event* event_reciever)
{
	if (!ent)
	{
		slog("Cannot click on a NULL entity");
		return;
	}

	//If there's a custom click
	if (ent->onClick) { ent->onClick(ent, event_reciever); }

	else {

		//TODO

	}

}

void entity_reciever(Entity* self, Game_Event* event)
{

	if (!self)
	{
		slog("Cannot recieve for a NULL entity");
		return;
	}

	//If there's a custom draw
	if (self->reciever) self->reciever(self, event);

	else {

		//TODO

	}

}