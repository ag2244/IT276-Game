#pragma once

#include <SDL.h>
#include "gfc_types.h"
#include "gfc_vector.h"

#include "gf2d_sprite.h"

#include "simple_json.h"
#include "simple_logger.h"

#include "starsabove_nation.h"

Nation_List nation_list = { 0 };

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

	nation_list.nations = max_nations;
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
			if (strcmp(current_nation->name, name))
				return &nation_list.nations[i];
		}
	}

	slog("No Nation with the name: \"%s\"", name);
	return NULL;
}

void nations_list_free()
{
	/*//Comment out for testing
	if (nation_list.nations != NULL) {
		free(nation_list.nations);
	}
	//*/

	memset(&nation_list, 0, sizeof(Nation_List));
	slog("Nations List freed");
}

void nation_new(char* name, Uint32 max_systems)
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

		return &nation_list.nations[i];
	}

	slog("No free nations available");
	return NULL;
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