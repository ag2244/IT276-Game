#pragma once

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "starsabove_entity.h"

typedef struct System_Data_s
{
	float placeholder;

} System_Data;

/**
* @brief Perform actions when a system is clicked
* @return NULL on error
*/
void system_onClick(struct Entity* self);

/**
* @brief Turn a star system to a json object
* @param self the entity to convert
* @return NULL on error, or a pointer to a json object
*/
SJson* system_toJson(struct Entity* self);

/**
* @brief Spawn a star system entity
* @param position The screen position to spawn the system at
* @return NULL on error, or a pointer to a new system entity
*/
Entity* system_spawn(char* name, Vector2D position);

#endif
