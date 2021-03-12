#pragma once

#ifndef __NATION_H__
#define __NATION_H__

#include "starsabove_entity.h"
#include "starsabove_system.h"
#include "starsabove_nation.h"

typedef struct Nation_s
{
	Bool _inuse;
	char name[256];
	//Entity* controlled_systems;

} Nation;

typedef struct
{
	Nation* nations;
	Uint32 max_nations;

} Nation_List;

/**
* @brief Initialize the list of nations
* @param self the entity to convert
* @return NULL on error, or a pointer to a json object
*/
void nations_list_init(Uint32 max_entities);

/**
* @brief Get a nation by name from the nations list
* @param name the nation's name
* @return NULL on error, or a pointer to a Nation object
*/
Nation* get_nation_by_name(char* name);

/**
* @brief Free the nations list
* @return NULL on error
*/
void nations_list_free();

/**
* @brief Make a nation
* @param name the name of the nation
* @param max_systems the maximum number of systems it can own
* @return NULL on error, or a pointer to a json object
*/
void nation_new(char* name, Uint32 max_systems);

/**
* @brief Free a nation from the nations list
* @param self the nation to free
* @return NULL on error
*/
void nation_free(Nation* nation);

#endif