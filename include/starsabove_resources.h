#pragma once

#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include "simple_logger.h"
#include "simple_json.h"

#include "gfc_types.h"

#include "starsabove_ui_textbox.h"

enum resources
{
	RES_FOOD = 0,
	RES_IRON,
	RES_URANIUM,
	RES_GOLD,
	RES_SILICON,
	RES_PLASTOIL
};

/*
* @brief Load a resources list from an SJSon
* @param resources_json The json resources array to load
* @return NULL on error, or an array of floats
*/
float* resources_fromJson(SJson* resources_json);

/*
* @brief Save resources array to an SJson object
* @param resources The resources array to save
* @return NULL on error, or an SJson object
*/
SJson* resources_toJson(float* resources);

/*
* @brief Make a menustate for a list of resources
* @param resources The array of resources
* @param planet_menustate The previous menustate to the one that will be returned
* @param name The name of the menu
* @return NULL on error, or a pointer to a new Menu_State
*/
Menu_State* resources_menustate_init(float* resources, Menu_State* previous_menustate, char* name);

/*
* @brief Get new array of resources
* @return NULL on error, or an array of floats, with keys being the enum values in resources
*/
float* resourcelist_new(float food, float iron, float uranium, float gold, float silicon, float plastoil);

#endif