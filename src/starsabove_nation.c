#pragma once

#include <SDL.h>
#include "gfc_types.h"
#include "gfc_vector.h"

#include "gf2d_sprite.h"

#include "simple_json.h"
#include "simple_logger.h"

#include "starsabove_nation.h"

Nation_List nation_list = { 0 };

int load_nations(SJson* game_json)
{
	//The nations in this game
	SJson* nations;
	SJson* currentNation;
	SJson* controlled_systems;

	char* name = NULL;
	
	int i;

	//game["Nations"]
	nations = sj_object_get_value(game_json, "Nations");

	if (!nations)
	{
		slog("Save file does not have any nations"); return NULL;
	}

	if (!sj_is_array(nations))
	{
		slog("Nations is not an array!"); return NULL;
	}

	//Go through the nations
	for (i = 0; i < nations->v.array->count; i++)
	{
		currentNation = sj_array_get_nth(nations, i);

		/*slog("ASD");
		//Get the name: string value of json object containing the value at currentSystem["name"]
		temp = sj_get_string_value(sj_object_get_value(currentSystem, "name"));
		slog("QWE");

		strcpy(name, temp);
		slog(name);*/

		name = sj_get_string_value(sj_object_get_value(currentNation, "name"));

		if (name == 0) {

			slog("Cannot use '0' name!"); return NULL;

		}

		nation_add(name);

	}

	slog("All nations have been loaded!");
	return 1;
}

void nations_list_init(Uint32 max_nations)
{
	if (max_nations == 0) {
		slog("Cannot allocate 0 nations!");
		return;
	}

	if (nation_list.nations != NULL) {
		nations_list_free();
	}

	nation_list.nations = (Nation*)gfc_allocate_array(sizeof(Nation), max_nations);

	if (nation_list.nations == NULL) {
		slog("Failed to allocate entity list!");
		return;
	}

	nation_list.max_nations = max_nations;

	atexit(nations_list_free);
	slog("Nations list initialized");
}

Nation* get_nation_by_name(char* name)
{
	int i;
	Nation* current_nation;

	if (nation_list.nations == NULL) {
		slog("Nations list does not exist!");
		return;
	}

	if (name == NULL) {
		slog("Cannot get an nation with NULL name");
		return;
	}

	for (i = 0; i < nation_list.max_nations; i++)
	{
		current_nation = &nation_list.nations[i];

		if (current_nation->_inuse && current_nation->name)
		{
			if (strcmp(current_nation->name, name) == 0)
				return &nation_list.nations[i];
		}
	}

	slog("No Nation with the name: \"%s\"", name);
	return NULL;
}

Nation_List* nation_list_get()
{

	if (nation_list.nations == NULL) {
		slog("Cannot get an empty nations list");
		return;
	}

	return &nation_list;
}

void nations_list_free()
{
	if (nation_list.nations != NULL) {
		free(nation_list.nations);
	}

	memset(&nation_list, 0, sizeof(Nation_List));
	slog("Nations List freed");
}

Nation* nation_new(Nation* nation, char* name)
{

	gfc_line_cpy(nation->name, name);

	nation->toJson = nation_toJson;

	return nation;
}

Nation* nation_add(char* name)
{
	int i;

	if (nation_list.nations == NULL) {
		slog("Nations list does not exist!");
		return;
	}

	for (i = 0; i < nation_list.max_nations; i++)
	{

		if (nation_list.nations[i]._inuse) continue; // Someone else using this one
		memset(&nation_list.nations[i], 0, sizeof(Nation));

		nation_list.nations[i]._inuse = 1;

		nation_new(&nation_list.nations[i], name, 100);

		return &nation_list.nations[i];
	}

	slog("No free nations available");
	return NULL;
}

SJson* nation_toJson(Nation* self)
{
	int i = 0;
	SJson* nation_json = sj_object_new();

	sj_object_insert(nation_json, "name", sj_new_str(self->name));

	return nation_json;
}

SJson* allNations_toJson()
{
	SJson* nations = sj_array_new();

	int i;
	Nation* thisNation;

	//Go through each nation in the nation list
	for (i = 0; i < nation_list.max_nations; i++)
	{
		//If the nation at index i is not in use, continue
		if (nation_list.nations[i]._inuse == 0) continue;

		thisNation = &nation_list.nations[i];

		sj_array_append(nations, thisNation->toJson(thisNation));
	}

	return nations;
}

void nation_free(Nation* nation)
{
	if (!nation)
	{
		slog("Cannot free a NULL nation");
		return;
	}

	//free(nation->controlled_systems);
	nation->_inuse = 0;
	//nation->name = NULL;
}