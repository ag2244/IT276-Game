#pragma once

#include <SDL.h>
#include "gfc_types.h"
#include "gfc_vector.h"

#include "gf2d_sprite.h"

#ifndef __ENTITY_H__
#define __ENTITY_H__

typedef struct {

	Bool _inuse;
	Vector2D position;
	Sprite* sprite;

} Entity;

/**
* @brief initialize internal entity management system
* @param max_entities how many concurrent entities to support
*/
void entity_manager_init(Uint32 max_entities);

/**
* @brief free all entities in the system, destroy entity manager
*/
void entity_manager_free();

/**
* @brief allocate an entiity, initialize to zero, return a pointer to it
* @return NULL on error (see logs) or a pointer to an initialized entity
*/
Entity* entity_new();

/**
* @brief free requested entity
* @param ent the entity to free
*/
void entity_free(Entity *ent);

/**
* @brief draw an entity to the current render frame
* @param ent the entity to draw
*/
void entity_draw(Entity* ent);

#endif