#pragma once

#include <SDL.h>
#include "gfc_types.h"
#include "gfc_vector.h"

#include "gf2d_sprite.h"

#include "simple_json.h"

#include "starsabove_game_resources.h"

#include "starsabove_collision.h"
#include "starsabove_ui_base.h"
#include "starsabove_nation.h"

#ifndef __ENTITY_H__
#define __ENTITY_H__

/* Entity_s is a temporary name, used so it can be called recursively*/
typedef struct Entity_s{

	Bool _inuse;

	char name[128];
	//char* name;

	Vector2D position;
	Vector2D velocity;
	Vector3D rotation;

	Sprite* sprite;

	Nation* owner;

	float frame;
	float frameRate;
	int frameCount;

	int num_neighbors;
	struct Entity_s* neighbors;

	int num_buttons;
	UI_Element* shortcut_buttons;

	/*Function pointers!*/
	void (*update)(struct Entity_s*self);
	void (*draw)(struct Entity_s* self);
	void (*free)(struct Entity_s* self);
	
	void (*onClick)(struct Entity_s* self, Game_Event* event_reciever, Bool playerowned);
	float* (*onNewTurn)(struct Entity_s* self);

	SJson* (*toJson)(struct Entity_s* self);
	
	void (*reciever)(struct Entity_s* self, Game_Event* event_reciever, Bool playerowned);

	void* data;

	Collider_Circle* collider_circle;
	Collider_Box* collider_box;

	Game_Event* clickEvent;

} Entity_p; //Finally naming it Entity


typedef struct
{
	Entity_p* entity_list;
	Uint32 max_entities;

} EntityManager;

/**
* @brief initialize internal entity management system
* @param max_entities how many concurrent entities to support
*/
void entity_manager_init(Uint32 max_entities);

/**
* @brief gets the active entity manager
* @return the active entity manager
*/
EntityManager* entity_manager_get();

/*
* @brief get an entity with a name specified
* @return The entity with a matching name, 0 if there is none, NULL if error.
*/
Entity_p* get_entity_by_name(char* name);

/**
* @brief calls update function on all entities
*/
void entity_manager_update_entities();

/**
* @brief calls draw function on all entities
*/
void entity_manager_draw_entities();

/**
* @brief free all entities in the system, destroy entity manager
*/
void entity_manager_free();

/**
* @brief allocate an entity, initialize to zero, return a pointer to it
* @return NULL on error (see logs) or a pointer to an initialized entity
*/
Entity_p* entity_new();

/**
* @brief allocate an entity with a name, initialize to zero, return a pointer to it
* @return NULL on error or a pointer to an initialized entity
*/
Entity_p* entity_new_name(char* name);

/**
* @brief free requested entity
* @param ent the entity to free
*/
void entity_free(Entity_p *ent);

/**
* @brief draw an entity to the current render frame
* @param ent the entity to draw
*/
void entity_draw(Entity_p* ent);

/**
* @brief check if the mouse is hovering over this entity
* @param ent the entity to check
* @param mX the x position of the mouse point
* @param mY the y position of the mouse point
*/
Bool entity_clickable(Entity_p* ent, float mX, float mY);

/**
* @brief what to do with an entity that has been clicked
* @param ent the entity that has been clicked
*/
void entity_onClick(Entity_p* ent, Game_Event* event_reciever, Bool playerowned);

/**
* @brief recieve a button transmission
* @param transmission The transmission struct to process
*/
//void entity_reciever(Entity* self, Game_Event* transmission);

#endif