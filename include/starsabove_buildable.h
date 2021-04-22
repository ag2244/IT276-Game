#pragma once

#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "simple_json.h"

#include "starsabove_resources.h"

#include "starsabove_ui_textbox.h"

typedef struct Planet;

static enum buildableStatus
{
	BLD_TEMPLATE,
	BLD_CONSTRUCTING,
	BLD_ACTIVE,
	BLD_DISABLED
};

static char buildable_status_names[4][128] =
{
	"Unbuilt",
	"Constructing",
	"Active",
	"Disabled"
};

typedef struct Buildable_s
{

	//Bool _inuse;

	int status;

	char name[128];

	float* resource_input;
	float* resource_output;

	float* costs;

	int buildtime;

	float* (*onNewTurn) (struct Buildable* self);

} Buildable;

/*
* @brief Load the dictionary of buildables from json
* @param buildabledict_json The json object holding all the buildables
*/
void buildabledict_load(SJson* buildabledict_json);

/*
* @brief Free the dictionary of buildables
*/
void buildabledict_free();

/*
* @brief Get the number of buildables in the game (buildabledict.num_buildables)
* @return An int representing the number of buildables
*/
int num_ingame_buildables();

/*
* @brief Get a buildable by name from the buildable_dict
* @param key The name of the buildable
* @return NULL on error, or a pointer to an existing buildable
*/
Buildable* buildable_get_byname(char* key);

/*
* @brief Load a Buildable from an SJson object
* @param buildable_json The SJson object to load
* @return NULL, or a pointer to a buildable
*/
Buildable* buildable_fromJson(SJson* buildable_json);

/*
* @brief Create a new buildable
* @param status The status of the buildable (enum)
* @param name The name of the buildable
* @param input The input resources
* @param output The output resources
* @return NULL, or a pointer to a buildable
*/
Buildable* buildable_new(int status, char* name, float* input, float* output, float* costs, int buildtime);

/*
* @brief Copy a buildable into another
* @param dst Destination
* @param src Source
*/
Buildable* buildable_copy(Buildable* src);

/*
* @brief Free a buildable object
*/
void buildable_free(Buildable* buildable);

/*
* @brief Turn a buildable into an SJson object
* @param buildable The buildable to save
* @return NULL or a pointer to a new SJson object
*/
SJson* buildable_toJson(Buildable* buildable);

/*
* @brief Initializes a buildable's Menu_State
* @param buildable The buildable to initialize a Menu_State for
* @param previous_menustate The previous (planet) menustate to this one
*/
Menu_State* buildable_menustate_init(Buildable* buildable, Menu_State* previous_menustate);

/*
* @brief Get a Menu_State for all buildings, including construction commands.
* @param previous_menustate The previous (planet) menustate to this one
* @param planet The planet to construct a buildable for
* @param system_name The name of the system the planet belongs to
* @return NULL on error, or a pointer to a new Menu_State
*/
Menu_State* buildable_construction_menustate_all(Menu_State* previous_menustate, struct Planet* planet, char* system_name);

/*
* @brief Do new turn processes for a buildable
* @param self The buildable to do new turn things for
*/
float* buildable_onNewTurn(Buildable* self);

#endif