#pragma once

#include <SDL.h>
#include "gfc_types.h"
#include "gfc_vector.h"

#include "gf2d_sprite.h"

#include "simple_json.h"

#include "starsabove_nation.h"

Nation_List nation_list = { 0 };

void nations_list_init(Uint32 max_nations)
{
	if (max_nations == 0) {
		slog("Canot allocate 0 entities!");
		return;
	}

	if (nation_list.nations != NULL) {
		nations_list_free();
	}

	nation_list.nations = (Entity*)gfc_allocate_array(sizeof(Nation), max_nations);

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

}

void nations_list_free()
{

}

void nation_init(char* name, Uint32 max_systems)
{
	
}

void nation_free(Nation* nation)
{

}