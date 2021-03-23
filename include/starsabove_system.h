#pragma once

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "starsabove_entity.h"
#include "starsabove_planet.h"

#include "starsabove_ui_textbox.h"

typedef struct System_Data_s
{
	int num_planets;
	Planet* planets;

} System_Data;

/**
* @brief Allocate a system data struct
* @param numPlanets the number of planets this system will have
* @return NULL on error, or a pointer to a new system_data struct
*/
System_Data* system_data_new(Uint32 numPlanets);

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
* @brief Get the gameevent for this system
* @param ent The system to get the gameevent for
* @return NULL on error, or a pointer to a new Game_Event
*/
void system_gameevent_init(struct Entity* ent);

/**
* @brief Spawn a star system entity
* @param name the name of the system
* @param position The screen position to spawn the system at
* @param owner The nation that owns this system
* @return NULL on error, or a pointer to a new system entity
*/
Entity* system_spawn(char* name, Vector2D position, Nation* owner, System_Data* systemdata);

/**
* @brief get the number of planets from a system
* @param system The system to get the number of planets from
* @return NULL on error, or the number of planets
*/
int system_num_planets(Entity* system);

#endif
