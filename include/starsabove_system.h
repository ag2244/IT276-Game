#pragma once

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "starsabove_entity.h"

typedef struct System_Data_s
{
	float placeholder;

} System_Data;

typedef struct
{
	
	char message[128];

} System_Transmission;

/**
* @brief Allocate a system data struct
* @return NULL on error, or a pointer to a new system_data struct
*/
System_Data* system_data_new();

/**
* @brief Allocate a system transmission
* @return NULL on error, or a pointer to a new system_transmission struct
*/
System_Transmission* system_transmission_new();

/**
* @brief Load all the systems in a game
* @return NULL on error, 1 if successful
*/
int load_systems(SJson* game_json);

/**
* @brief Perform actions when a system is clicked
* @return NULL on error
*/
void system_onClick(struct Entity* self, Game_Event* event_reciever);

/**
* @brief Turn a star system to a json object
* @param self the entity to convert
* @return NULL on error, or a pointer to a json object
*/
SJson* system_toJson(struct Entity* self);

/**
* @brief Spawn a star system entity
* @param name the name of the system
* @param position The screen position to spawn the system at
* @param owner The nation that owns this system
* @return NULL on error, or a pointer to a new system entity
*/
Entity* system_spawn(char* name, Vector2D position, Nation* owner);

#endif
