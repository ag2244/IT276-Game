#pragma once

#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "simple_json.h"

#include "starsabove_resources.h"

enum buildableStatus
{
	BLD_CONSTRUCTING = -1,
	BLD_ACTIVE,
	BLD_DISABLED
};

typedef struct
{

	int status;

	char name[128];
	float* resource_input;
	float* resource_output;

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
Buildable* buildable_new(int status, char* name, float* input, float* output);

/*
* @brief Copy a buildable into another
* @param dst Destination
* @param src Source
*/
void buildable_copy(Buildable* dst, Buildable* src);

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

#endif