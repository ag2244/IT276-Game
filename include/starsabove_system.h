#pragma once

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "starsabove_entity.h"

typedef struct System_Data_s
{
	char* name;

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
void system_onClick(struct Entity* self);

/*
* @brief Get the name belonging to this system
* @param The system who's name is requested
* @return The system's name, 0 if there is none, NULL if error.
*/
char* system_name(struct Entity* self);

/**
* @brief Spawn a star system entity
* @param position The screen position to spawn the system at
* @return NULL on error, or a pointer to a new system entity
*/
Entity* system_spawn(char* name, Vector2D position);

/**
* @brief Turn a star system to a json object
* @param self the entity to convert
* @return NULL on error, or a pointer to a json object
*/
SJson* system_toJson(Entity* self);

#endif
