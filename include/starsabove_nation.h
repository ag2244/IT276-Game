#pragma once

#ifndef __NATION_H__
#define __NATION_H__

typedef struct Menu_State;

typedef struct Fleet;

typedef struct Nation_s
{
	Bool _inuse;
	
	char name[256];

	float* resources_total;

	struct Fleet* fleets;

	SJson* (*toJson)(struct Nation_s* self);

} Nation;

typedef struct
{
	Nation* nations;
	Uint32 max_nations;

} Nation_List;


/**
* @brief Load all the nations in a game
* @return 1 on success or NULL on error
*/
int load_nations(SJson* game_json);

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
* @brief Get a nation list
* @return NULL on error, or a pointer to a Nation List object
*/
Nation_List* nation_list_get();

/**
* @brief Free the nations list
* @return NULL on error
*/
void nations_list_free();

/**
* @brief Make a new nation
* @param name the name of the nation
* @param max_systems the maximum number of systems it can own
* @return NULL on error, or a pointer to a json object
*/
Nation* nation_new(Nation* nation, char* name, float* resources, struct Fleet* fleets);

/**
* @brief Add a nation to the nations list
* @param name the name of the nation
* @return NULL on error, or a pointer to a json object
*/
Nation* nation_add(char* name, float* resources, struct Fleet* fleets);

/*
* @brief Do new turn functions for a nation
* @param nation The nation to do new turn functions for
*/
void nation_onNewTurn(Nation* nation);

/*
* @brief Go through all nations and call nation_onNewTurn() for each
*/
void nations_list_onNewTurn();

/*
* @brief Get the menustate for a nation
* @param nation The nation to get the menustate for
* @param _isPlayer Is the nation the same as the player nation?
*/
struct Menu_State* nation_menustate(Nation* nation, Bool _isPlayer);

/**
* @brief Turn a nation to a json object
* @param self the nation to convert
* @return NULL on error, or a pointer to a json object
*/
SJson* nation_toJson(Nation* self);

/**
* @brief Turn all the nations in nation_list to a json object
* @return NULL on error, or a pointer to a json object
*/
SJson* allNations_toJson();

/**
* @brief Free a nation from the nations list
* @param self the nation to free
* @return NULL on error
*/
void nation_free(Nation* nation);

#endif