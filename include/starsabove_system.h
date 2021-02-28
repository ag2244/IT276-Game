#pragma once

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "starsabove_entity.h"

typedef struct System_Data_s
{
	char* name;
	float test;

} System_Data;

/**
* @brief Allocate space for a system's data
* @return NULL on error, or a pointer to a new system data struct
*/
System_Data* system_data_new(char* name);

/**
* @brief Perform actions when a system is clicked
* @return NULL on error
*/
void system_onClick(struct Entity_s* self);

/**
* @brief Spawn a star system entity
* @param position The screen position to spawn the system at
* @return NULL on error, or a pointer to a new system entity
*/
Entity* system_spawn(char* name, Vector2D position);

#endif
