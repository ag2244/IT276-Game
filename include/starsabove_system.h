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
void system_onClick(struct Entity* self, Game_Event* event_reciever, Bool playerowned);

/*
* @brief A system's unique update function
*/
void system_update(Entity_p* self);

/**
* @brief Perform actions when a system is clicked
* @return NULL on error
*/
float* system_onNewTurn(struct Entity* self);

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
Entity_p* system_spawn(char* name, Vector2D position, Nation* owner, System_Data* systemdata);

/*
* @brief Get a menustate consisting of all the system's neighbors
* @param self The system whose neighbors to get
* @param previous The previous men state
* @param title Title of the menu
* @param template The game event to use as a template for each button
* @return NULL on error, or a new menu_state.
*/
Menu_State* system_neighbors_menustate(Entity_p* self, Menu_State* previous, char title[128], Game_Event* template);

/**
* @brief get the number of planets from a system
* @param system The system to get the number of planets from
* @return NULL on error, or the number of planets
*/
int system_num_planets(Entity_p* system);

/*
* @brief Get a planet in the system by name
* @param system The system to get the planet for
* @param planet_name The planet to get
* @return NULL on error or the planet with the specified name
*/
Planet* system_planet_byname(Entity_p* system, char* planet_name);

#endif
