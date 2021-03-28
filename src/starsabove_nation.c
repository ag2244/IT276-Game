#pragma once

#include <SDL.h>
#include "gfc_types.h"
#include "gfc_vector.h"

#include "gf2d_sprite.h"

#include "simple_json.h"
#include "simple_logger.h"

#include "starsabove_nation.h"

#include "starsabove_resources.h"
#include "starsabove_entity.h"
#include "starsabove_ui_textbox.h"

Nation_List nation_list = { 0 };

int load_nations(SJson* game_json)
{
	Nation* nation = NULL;

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

		//Get the name
		name = sj_get_string_value(sj_object_get_value(currentNation, "name"));

		if (name == 0) {

			slog("Cannot use '0' name!"); return NULL;

		}

		nation = nation_add(name, resources_fromJson(sj_object_get_value(currentNation, "resources")));

		if (!nation)
		{
			slog("Could not add nation \"%s\"", name); return NULL;
		}


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

Nation* nation_new(Nation* nation, char* name, float* resources)
{

	gfc_line_cpy(nation->name, name);

	nation->resources_total = malloc(6 * sizeof(float));

	for (int i = 0; i < numresources; i++)
	{
		nation->resources_total[i] = resources[i];
	}

	nation->toJson = nation_toJson;

	return nation;
}

Nation* nation_add(char* name, float* resources)
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

		nation_new(&nation_list.nations[i], name, resources);

		return &nation_list.nations[i];
	}

	slog("No free nations available");
	return NULL;
}

void nation_onNewTurn(Nation* nation)
{
	int i;

	EntityManager entitymgr = *entity_manager_get();
	Entity* thisSystem;

	float* resources = malloc(6 * sizeof(float));

	for (i = 0; i < 6; i++) { resources[i] = 0; }

	for (i = 0; i < entitymgr.max_entities; i++)
	{

		if (entitymgr.entity_list[i]._inuse == 0) {continue; }

		thisSystem = &entitymgr.entity_list[i];

		if (thisSystem->owner == nation)
		{
			resources = resourcelist_add(thisSystem->onNewTurn(thisSystem), resources);
		}

	}

	resources = resourcelist_add(nation->resources_total, resources);

	for (i = 0; i < 6; i++)
	{
		nation->resources_total[i] = resources[i];
	}
}

void nations_list_onNewTurn()
{
	int i;

	//Go through each nation and perform their onNewTurn() function
	for (i = 0; i < nation_list.max_nations; i++)
	{
		if (nation_list.nations[i]._inuse)
		{
			nation_onNewTurn(&nation_list.nations[i]);
		}
	}
}

struct Menu_State* nation_menustate(Nation* nation, Bool _isPlayer)
{

	int i;

	char temp[128];

	UI_Element* system_textbox;
	UI_Element* resources_textbox;

	Menu_State* nation_menustate;

	nation_menustate = menu_state_new(
		NULL,
		textbox_init
		(
			vector2d(10, 10),
			vector2d(200, 50),
			nation->name,
			font_load("resources/fonts/futur.ttf", 20)
		),
		NULL,
		vector2d(10, 10),
		0,
		5
	);

	/*
	menu_addTo(
		nation_menustate->current_menu,
		textbox_init
		(
			vector2d(10, 10),
			vector2d(250, 50),
			"Owned Systems",
			font_load("resources/fonts/futur.ttf", 12)
		)
	);*/

	//Add resources

	resources_textbox = textbox_init
	(
		vector2d(10, 10),
		vector2d(250, 50),
		"Total Resources",
		font_load("resources/fonts/futur.ttf", 12)
	);

	resources_textbox->signal = new_gameevent(
		nation->name,
		NULL,
		"GETRESOURCES",
		NULL,
		0,
		resources_menustate_init(nation->resources_total, nation_menustate, "Total Resources"),
		0
	);

	menu_addTo(
		nation_menustate->current_menu,
		resources_textbox
	);

	menu_state_show(nation_menustate);

	return nation_menustate;
}

SJson* nation_toJson(Nation* self)
{
	int i = 0;
	SJson* nation_json = sj_object_new();

	sj_object_insert(nation_json, "name", sj_new_str(self->name));

	sj_object_insert(nation_json, "resources", resources_toJson(self->resources_total));

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