#pragma once

#ifndef __PLANET_H__
#define __PLANET_H__

#include "simple_json.h"
#include "simple_logger.h"

#include "gfc_types.h"

#include "starsabove_buildable.h"

#include "starsabove_ui_textbox.h"
#include "starsabove_resources.h"

typedef struct Planet_s
{
	char name[128];
	
	float* resources_mining;

	int num_buildings;
	Buildable* buildings;

	float* (*onNewTurn) (struct Planet_s* planet);

} Planet;

/*
* @brief load a planet from json
* @param planetjson the json for the planet
* @return NULL on error or a pointer to the loaded planet
*/
Planet* planet_fromJson(SJson* planetjson);

/*
* @brief save a planet to a json
* @param planet The planet to save
* @return NULL on error or a pointer to the saved json object
*/
SJson* planet_toJson(Planet* planet);

/*
* @brief Construct a building on a planet
* @param planet The planet to construct it on
* @param building The building to construct
*/
void planet_construct(Planet* planet, Buildable* building);

/*
* @brief save a planet to a json
* @param planet The planet to save
* @return NULL on error or a pointer to the saved json object
*/
Menu_State* planet_menustate_init(Planet* planet, Menu_State* system_menustate, char* system_name, Bool playerOwned);

/*
* @brief Perform new turn operations for a planet
* @param planet The planet to do new turn operations for
*/
float* planet_onNewTurn(Planet* planet);

/*
* @brief creates a new planet
* @param name name of the planet
* @param resources_arr array of resource values
* @return NULL on error or a pointer to a new planet
*/
Planet* planet_new(char* name, float* resources_arr);

#endif
